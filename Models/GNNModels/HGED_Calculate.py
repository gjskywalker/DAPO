import os
import pickle
import random
import networkx as nx
import numpy as np
from itertools import combinations
from multiprocessing import get_context
from typing import List, Tuple, Dict, Any, Optional, Union

_SHARED_GRAPHS: Optional[List[nx.DiGraph]] = None
_SHARED_HGED: Optional["HGED"] = None


def _worker_init(graphs: List[nx.DiGraph], verbose: bool) -> None:
    """Initialize shared state for each worker process."""
    global _SHARED_GRAPHS, _SHARED_HGED
    _SHARED_GRAPHS = graphs
    _SHARED_HGED = HGED("", "", "", "", verbose=verbose)


def _worker_compute(pair: Tuple[int, int]) -> Tuple[Tuple[int, int], Optional[float]]:
    """Compute distance for a single pair using shared graphs."""
    if _SHARED_GRAPHS is None or _SHARED_HGED is None:
        return pair, None
    left, right = pair
    distance = _SHARED_HGED._compute_distance(
        left,
        right,
        _SHARED_GRAPHS,
        _SHARED_HGED._dapo_node_substitute,
        _SHARED_HGED._dapo_edge_substitute,
    )
    return pair, distance

class HGED():
    
    def __init__(self, graphs_file: str, HGED_Result_file: str, log_file: str, indexes_file: str, verbose: bool = True) -> None:
        self.graphs_file = graphs_file
        self.HGED_Result_file = HGED_Result_file
        self.log_file = log_file
        self.indexes_file = indexes_file
        self.backedges: List[Tuple[Any, Any]] = list()
        self.verbose = verbose

    def _find_back_edges(self, graph: nx.DiGraph) -> List[Tuple[Any, Any]]:
        """
        Find back edges in a control flow graph using DFS.
        Back edges are edges that point from a node to one of its ancestors in the DFS tree.
        
        Args:
            graph (nx.DiGraph): The control flow graph
            
        Returns:
            List[Tuple[Any, Any]]: List of back edges as tuples (source, target)
        """
        # Clear previous results
        self.backedges.clear()
        
        # DFS state tracking
        WHITE = 0  # Unvisited
        GRAY = 1   # Currently being processed (in DFS stack)
        BLACK = 2  # Finished processing
        
        # Initialize node colors
        color = {node: WHITE for node in graph.nodes()}
        
        def _dfs_visit(node):
            """Recursive DFS to find back edges"""
            color[node] = GRAY
            
            # Check all outgoing edges
            for neighbor in graph.successors(node):
                if color[neighbor] == WHITE:
                    # Tree edge or forward edge - continue DFS
                    _dfs_visit(neighbor)
                elif color[neighbor] == GRAY:
                    # Back edge found! neighbor is an ancestor of node
                    self.backedges.append((node, neighbor))
                    print(f"Back edge found: {node} -> {neighbor}")
                # If color[neighbor] == BLACK, it's a forward/cross edge, ignore
            
            color[node] = BLACK
        
        # Start DFS from all unvisited nodes (handles disconnected components)
        for node in graph.nodes():
            if color[node] == WHITE:
                _dfs_visit(node)
        
        # Create a set for faster lookup
        self.backedges_set = set(self.backedges)
        
        print(f"Total back edges found: {len(self.backedges)}")
        return self.backedges
    
    def _generate_ged(self) -> None:
        raise NotImplementedError("This method should be implemented by subclasses")

    def _dapo_node_substitute(self, node1: Dict[str, Any], node2: Dict[str, Any]) -> int:
        if (np.array_equal(node1['nodeattr'],node2['nodeattr'])):
            return 0
        else:
            return 1
        
    def _dapo_edge_substitute(self, edge1: Dict[str, Any], edge2: Dict[str, Any]) -> int:
        if (np.array_equal(edge1['edgeattr'],edge2['edgeattr'])):
            return 0
        else:
            return 1

    def _harp_node_substitute(self, node1: Dict[str, Any], node2: Dict[str, Any]) -> int:
        if (np.array_equal(node1['nodeattr'],node2['nodeattr'])):
            return 0
        else:
            return 1

    def _harp_edge_substitute(self, edge1: Dict[str, Any], edge2: Dict[str, Any]) -> int:
        if (np.array_equal(edge1['edgeattr'],edge2['edgeattr'])):
            return 0
        else:
            return 1

    def _compute_distance(self, left: int, right: int, graphs: List[nx.DiGraph],
                         node_substitute: callable, edge_substitute: callable) -> Optional[float]:
        total_distance: Optional[float] = None
        os.makedirs("HGED_Result_Dapo", exist_ok=True)
        info_path = os.path.join("HGED_Result_Dapo", f"Graph_Info_{left}_{right}.txt")
        with open(info_path, "w") as f:
            f.write(f"==== CDFG LEFT (index {left}) ====" + "\n")
            for node in graphs[left].nodes(data=True):
                f.write(f"Node: {node[0]}, Attr: {str(node[1]['nodeattr'])}\n")
            for edge in graphs[left].edges(data=True):
                f.write(f"Edge: {edge[0]}->{edge[1]}, Attr: {str(edge[2]['edgeattr'])}\n")
            f.write(f"==== CDFG RIGHT (index {right}) ====" + "\n")
            for node in graphs[right].nodes(data=True):
                f.write(f"Node: {node[0]}, Attr: {str(node[1]['nodeattr'])}\n")
            for edge in graphs[right].edges(data=True):
                f.write(f"Edge: {edge[0]}->{edge[1]}, Attr: {str(edge[2]['edgeattr'])}\n")

            CFG_left = self._generate_control_flow_graph(graphs[left])
            DFG_left = self._generate_data_flow_graph(graphs[left])
            CFG_right = self._generate_control_flow_graph(graphs[right])
            DFG_right = self._generate_data_flow_graph(graphs[right])

            f.write(f"==== CFG LEFT (index {left}) ====" + "\n")
            for node in CFG_left.nodes(data=True):
                f.write(f"Node: {node[0]}, Attr: {str(node[1]['nodeattr'])}\n")
            for edge in CFG_left.edges(data=True):
                f.write(f"Edge: {edge[0]}->{edge[1]}, Attr: {str(edge[2]['edgeattr'])}\n")
            f.write(f"==== CFG RIGHT (index {right}) ====" + "\n")
            for node in CFG_right.nodes(data=True):
                f.write(f"Node: {node[0]}, Attr: {str(node[1]['nodeattr'])}\n")
            for edge in CFG_right.edges(data=True):
                f.write(f"Edge: {edge[0]}->{edge[1]}, Attr: {str(edge[2]['edgeattr'])}\n")

            f.write(f"==== DFG LEFT (index {left}) ====" + "\n")
            for node in DFG_left.nodes(data=True):
                f.write(f"Node: {node[0]}, Attr: {str(node[1]['nodeattr'])}\n")
            for edge in DFG_left.edges(data=True):
                f.write(f"Edge: {edge[0]}->{edge[1]}, Attr: {str(edge[2]['edgeattr'])}\n")
            f.write(f"==== DFG RIGHT (index {right}) ====" + "\n")
            for node in DFG_right.nodes(data=True):
                f.write(f"Node: {node[0]}, Attr: {str(node[1]['nodeattr'])}\n")
            for edge in DFG_right.edges(data=True):
                f.write(f"Edge: {edge[0]}->{edge[1]}, Attr: {str(edge[2]['edgeattr'])}\n")

        CFG_distance = nx.graph_edit_distance(CFG_left, CFG_right, node_subst_cost=node_substitute, edge_subst_cost=edge_substitute, timeout=60)
        DFG_distance = nx.graph_edit_distance(DFG_left, DFG_right, node_subst_cost=node_substitute, edge_subst_cost=edge_substitute, timeout=60)

        if CFG_distance is not None and DFG_distance is not None:
            total_distance = CFG_distance + DFG_distance
        elif CFG_distance is not None:
            total_distance = CFG_distance
        elif DFG_distance is not None:
            total_distance = DFG_distance

        return total_distance

    def _generate_control_flow_graph(self, graph: nx.DiGraph) -> nx.DiGraph:
        CFG = nx.DiGraph()
        for node in graph.nodes(data=True):
            if node[1]['nodeattr'][10] == 1:
                CFG.add_node(node[0], **node[1])
        for edge in graph.edges(data=True):
            if edge[2]['edgeattr'][1] == 1:
                CFG.add_edge(edge[0], edge[1], **edge[2])
        self._find_back_edges(CFG)
        return CFG

    def _generate_data_flow_graph(self, graph: nx.DiGraph) -> nx.DiGraph:
        DFG = nx.DiGraph()
        for node in graph.nodes(data=True):
            if node[1]['nodeattr'][10] == 1:
                DFG.add_node(node[0], nodeattr=np.zeros(12, dtype=np.int32))
            else:
                DFG.add_node(node[0], **node[1])
        for edge in graph.edges(data=True):
            if (edge[0], edge[1]) in self.backedges or (edge[1], edge[0]) in self.backedges_set:
                if self.verbose:
                    print(f"Found back edge in data flow graph: {edge[0]} -> {edge[1]}")
                continue
            DFG.add_edge(edge[0], edge[1], **edge[2])
        return DFG

    def dapo_generate_hged(self, graphs_file:str, HGED_Result_file:str, log_file:str, indexes_file:str) -> None:
        os.makedirs('HGED_Result_Dapo', exist_ok=True)
        with open(graphs_file, "rb") as fp:
            graphs = pickle.load(fp)

        try:
            with open(indexes_file, "rb") as idx_fp:
                index_pairs = pickle.load(idx_fp)
        except FileNotFoundError:
            index_pairs = None

        normalized_pairs: List[Tuple[int, int]] = []
        if isinstance(index_pairs, (list, tuple)):
            for pair in index_pairs:
                if isinstance(pair, (list, tuple)) and len(pair) == 2:
                    left, right = pair
                    try:
                        normalized_pairs.append((int(left), int(right)))
                    except (TypeError, ValueError):
                        continue
        if not normalized_pairs:
            all_pairs = list(combinations(range(len(graphs)), 2))
            random.shuffle(all_pairs)
            normalized_pairs = all_pairs

        results: Dict[Tuple[int, int], Optional[float]] = {}
        workers = min(16, max(1, len(normalized_pairs)))
        ctx = get_context("spawn")

        with ctx.Pool(processes=workers, initializer=_worker_init, initargs=(graphs, self.verbose)) as pool:
            for pair, distance in pool.imap_unordered(_worker_compute, normalized_pairs):
                results[pair] = distance

        with open(os.path.join('HGED_Result_Dapo/',self.HGED_Result_file), "wb") as result_fp:
            pickle.dump(results, result_fp)

        index_output = [pair for pair in normalized_pairs if pair in results]

        with open(os.path.join('HGED_Result_Dapo/', self.indexes_file), "wb") as idx_out:
            pickle.dump(index_output, idx_out)
        
def dapo_generate_hged(graphs_file: str, HGED_Result_file: str, log_file: str, indexes_file: str) -> None:
    """Standalone function to generate HGED results using DAPO method"""
    hged = HGED(graphs_file, HGED_Result_file, log_file, indexes_file)
    hged.dapo_generate_hged(graphs_file, HGED_Result_file, log_file, indexes_file)

if __name__ == "__main__":
    dapo_generate_hged("Graph_Dapo/graphs_random_dataset_networkx.pkl", "Dapo_HGED_Result.pkl", "Dapo_log.txt", "Dapo_indexes.pkl")
