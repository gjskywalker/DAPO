import torch
import networkx as nx
from torch_geometric.data import Data, HeteroData
import numpy as np
import pandas as pd

graph_samples = []

def read_files(edge_file, node_file, type):
    edges = pd.read_csv(edge_file)
    nodes = pd.read_csv(node_file)
    src_nodes = edges["Source"].values

    tgt_nodes = edges["Target"].values

    edge_attributes = edges.iloc[:, 2:].values

    node_ids = nodes.iloc[0:, 0].values

    node_attributes = nodes.iloc[0:, 1:].values

    x = []
    edge_index = []
    edge_attri = []
    nodes = []
    graph = nx.DiGraph()
    
    for node_id, attributes in zip(node_ids, node_attributes):
        if type == "HGraph":
            nodes.append(attributes)
        elif type == "Graph":
            x.append(attributes)
        elif type == "NGraph":
            graph.add_node(node_id, nodeattr=np.array(attributes))
             
    edge1 = [[],[]]
    edge2 = [[],[]]
    edge3 = [[],[]]
    for source, target, attributes in zip(src_nodes, tgt_nodes, edge_attributes):
        if type == "HGraph":
            if attributes[0] == 1:
                edge1[0].append(source)
                edge1[1].append(target)
                
            elif attributes[1] == 1:
                edge2[0].append(source)
                edge2[1].append(target)
                
            else :
                edge3[0].append(source)
                edge3[1].append(target)
        elif type == "Graph":
            edge_index.append((source, target))
            edge_attri.append(attributes)
        elif type == "NGraph":
            graph.add_edge(source, target, edgeattr=np.array(attributes))
    
    if type == "Graph":
        x = torch.tensor(np.array(x), dtype=torch.int)
        edge_index = torch.tensor(np.array(edge_index), dtype=torch.int64)
        edge_attri = torch.tensor(np.array(edge_attri), dtype=torch.int)
        data = Data(node_attr=x, edge_index=edge_index.t().contiguous(), edge_attr=edge_attri)
    elif type == "HGraph":
        data = HeteroData()
        data["node"].x = torch.tensor(np.array(nodes, dtype=np.float32))
        data[("node", "edge1", "node")].edge_index = torch.tensor(np.array(edge1))
        data[("node", "edge2", "node")].edge_index = torch.tensor(np.array(edge2))
        data[("node", "edge3", "node")].edge_index = torch.tensor(np.array(edge3))
    elif type == "NGraph":
        data = graph
    return data

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(
                    usage="python Build_Dataset_Pytorch.py --Dataset Raw_Graphs_Random --Type Graph",
                    description="Build Pytorch Dataset, HGraph for HAN, Graph for other GNNs, NGraph for Networkx")
    
    parser.add_argument('-d','--Dataset',required=True, type=str)
    parser.add_argument('-t','--Type', required=True, type=str)
    args = parser.parse_args()
    dataset_path = args.Dataset
    for i in range(0, 40):
        edge_file = dataset_path + "/Edge_" + str(i) + ".csv"
        node_file = dataset_path + "/Node_Feature_" + str(i) + ".csv"
        data = read_files(edge_file, node_file, args.Type)
        graph_samples.append(data)
        
    import pickle
    import os
    if os.path.exists('Testingsets_O03'):
        pass
    else:
        os.system("mkdir Testingsets_O03")
    if args.Type == "HGraph":
        with open('Testingsets_O03/graphs_random_dataset_hanconv_pytorch.pkl', 'wb') as fp:
            pickle.dump(graph_samples, fp)
    elif args.Type == "Graph":
        with open('Testingsets_O03/graphs_random_dataset_pytorch.pkl', 'wb') as fp:
            pickle.dump(graph_samples, fp)
    elif args.Type == "NGraph":
        with open('Testingsets_O03/graphs_random_dataset_networkx.pkl', 'wb') as fp:
            pickle.dump(graph_samples, fp)



