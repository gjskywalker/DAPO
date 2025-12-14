import os 
import pickle
import networkx as nx
import numpy as np
from multiprocessing import Process, Manager
from typing import List, Tuple, Dict, Any, Optional, Union

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
                         node_substitute: callable, edge_substitute: callable) -> Dict[Tuple[int, int], List[Optional[float]]]:
        return_dict = dict()
        with open("Graph_Info.txt", "w") as f:
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
        if CFG_distance and DFG_distance:
            return_dict[(left, right)] = [CFG_distance, DFG_distance]
        return return_dict

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
        if os.path.exists('HGED_Result_Dapo'):
            pass
        else:
            os.system("mkdir HGED_Result_Dapo")
        directory = "HGED_Result_Dapo/"
        fp = open(graphs_file, "rb")
        graphs = pickle.load(fp)
        print(len(graphs))
        print("Generating HGED for DAPO method...")
        print(self._compute_distance(0, 1, graphs, self._dapo_node_substitute, self._dapo_edge_substitute))
        # graph_idx = np.random.RandomState(0).randint(len(graphs)-1, size=(28800, 2))
        # targets = []
        # index = []

        # for i in range(0, 960):
        #     processes = []
        #     manager = Manager()
        #     return_dict = manager.dict()

        #     for left, right in graph_idx[i*30 : (i+1)*30]:
        #         p = Process(target=self._compute_distance, args=(left, right, graphs, self._dapo_node_substitute, self._dapo_edge_substitute, return_dict))
        #         p.start()
        #         processes.append(p)

        #     for p in processes:
        #         p.join()
                
        #     for key, distance in return_dict.items():
        #         index.append((key[0], key[1]))
        #         targets.append(distance)
            
        # with open(directory + log_file, "w") as result:
        #     for (left, right), distance in zip(index, targets):
        #         print(f"{left}, {right} : {distance}", file=result)

        # with open(directory+HGED_Result_file, "wb") as fp:
        #     pickle.dump(targets, fp)

        # with open(directory+indexes_file, "wb") as ind:
        #     pickle.dump(index, ind)

    def harp_generate_hged(self, graphs_file:str, HGED_Result_file:str, log_file:str, indexes_file:str) -> None: 
        if os.path.exists('HGED_Result_Harp'):
            pass
        else:
            os.system("mkdir HGED_Result_Harp")
        directory = "HGED_Result_Harp/"
        fp = open(graphs_file, "rb")
        graphs = pickle.load(fp)
        print(len(graphs))
        graph_idx = np.random.RandomState(0).randint(len(graphs) - 1, size=(28800, 2))
        targets = []
        index = []

        for i in range(0, 960):
            processes = []
            manager = Manager()
            return_dict = manager.dict()

            for left, right in graph_idx[i*30 : (i+1)*30]:
                p = Process(target=self._compute_distance, args=(left, right, graphs, self._harp_node_substitute, self._harp_edge_substitute, return_dict))
                p.start()
                processes.append(p)

            for p in processes:
                p.join()

            for key, distance in return_dict.items():
                index.append((key[0], key[1]))
                targets.append(distance)
            
        with open(directory+log_file, "w") as result:
            for (left, right), distance in zip(index, targets):
                print(f"{left}, {right} : {distance}", file=result)

        with open(directory+HGED_Result_file, "wb") as fp:
            pickle.dump(targets, fp)

        with open(directory+indexes_file, "wb") as ind:
            pickle.dump(index, ind)

def dapo_generate_hged(graphs_file: str, HGED_Result_file: str, log_file: str, indexes_file: str) -> None:
    """Standalone function to generate HGED results using DAPO method"""
    hged = HGED(graphs_file, HGED_Result_file, log_file, indexes_file)
    hged.dapo_generate_hged(graphs_file, HGED_Result_file, log_file, indexes_file)

def harp_generate_hged(graphs_file: str, HGED_Result_file: str, log_file: str, indexes_file: str) -> None:
    """Standalone function to generate HGED results using HARP method"""
    hged = HGED(graphs_file, HGED_Result_file, log_file, indexes_file)
    hged.harp_generate_hged(graphs_file, HGED_Result_file, log_file, indexes_file)

if __name__ == "__main__":
    # harp_generate_hged("/home/eeuser/Desktop/GRL-HLS/Program_Representation_Learning/graph_list.pkl", "Harp_HGED_Result.pkl", "Harp_log.txt", "Harp_indexes.pkl")
    dapo_generate_hged("//Users/jinmingge/Documents/HKUST_MPHIL/GRL-HLS/GNNRL/GNN_Model/Graph_Dapo/graphs_random_dataset_networkx.pkl", "Dapo_HGED_Result.pkl", "Dapo_log.txt", "Dapo_indexes.pkl")
