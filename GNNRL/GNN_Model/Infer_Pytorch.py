import torch
import pickle
import os 
import torch.nn as nn
from torch_geometric.loader import DataLoader
from torch_geometric.nn import PNAConv, global_mean_pool
import numpy as np
import pandas as pd
import torch.nn.functional as F
from torch_geometric.data import Data
import subprocess
import math
from PNA_Pytorch import GraphDataset
import PNA_Pytorch
import RGCN_Pytorch
import GCN_Pytorch
import HAN_Pytorch
import SAGE_Pytorch
import GATV2_Pytorch

def read_file(edge_file, node_file):
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

    for node_id, attributes in zip(node_ids, node_attributes):
        attributes = np.array(attributes) 
        x.append(attributes)

    for source, target, attributes in zip(src_nodes, tgt_nodes, edge_attributes):
        edge_index.append((source, target))
        edge_attri.append(attributes)
    
    x = torch.tensor(x, dtype=torch.int)
    edge_index = torch.tensor(edge_index, dtype=torch.int64)
    edge_attri = torch.tensor(edge_attri, dtype=torch.int)
    
    data = Data(node_attr=x, edge_index=edge_index.t().contiguous(), edge_attr=edge_attri)
    
    return data

def gnn_get_feature(c_code, pre_graphs, path="."):
  pregraph = pre_graphs
  IRfilePath = path + "/cycleIRfile"
  cp_cmd = "cp " + IRfilePath + "/" + "top.bc /home/eeuser/Desktop/GRL-HLS/LLVM_Tutorial/Tests/FeatureExtractor/build"
  os.system(cp_cmd)
  execute_cmd = "cd /home/eeuser/Desktop/GRL-HLS/LLVM_Tutorial/Tests/FeatureExtractor/build && ./FeatureExtractor_V2 0 top.bc" 
  result = subprocess.run(execute_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
  if result.returncode == 0:
    graph = read_file("/home/eeuser/Desktop/GRL-HLS/LLVM_Tutorial/Tests/FeatureExtractor/build/Edge_0.csv", "/home/eeuser/Desktop/GRL-HLS/LLVM_Tutorial/Tests/FeatureExtractor/build/Node_Feature_0.csv")
    return graph
  else:
    return pregraph

def print_all_model():
  checkpoint1 = torch.load('models/Embedding_model_RGCNConv.pth')
  print(f"Loss for RGCNConv : {checkpoint1['loss']}")
  checkpoint1 = torch.load('models/Embedding_model_HAN.pth')
  print(f"Loss for HAN : {checkpoint1['loss']}")
  checkpoint1 = torch.load('models/Embedding_model_PNAConv.pth')
  print(f"Loss for PNAConv : {checkpoint1['loss']}")
  checkpoint1 = torch.load('models/Embedding_model_SAGEConv.pth')
  print(f"Loss for SAGEConv : {checkpoint1['loss']}")
  checkpoint1 = torch.load('models/Embedding_model_GATV2.pth')
  print(f"Loss for GATv2Conv : {checkpoint1['loss']}")
  checkpoint1 = torch.load('models/Embedding_model_GCNConv.pth')
  print(f"Loss for GCNConv : {checkpoint1['loss']}")


# indices=[]
# c_code="random42"
# input = gnn_get_feature(c_code,indices,path="/home/eeuser/Desktop/GRL-HLS/GNNRL/RL_Model/run_0_p407502")


# checkpoint = torch.load("Embedding_model.pth")
# gna.load_state_dict(checkpoint)
# gna.eval()
# with torch.no_grad():
#     output = gna(input)
#     print(output)

if __name__ == "__main__":
  # print_all_model()
  with open('GED_Result_testingO03/Index.pkl', 'rb') as fp:
    graph_pair_idx = pickle.load(fp)
  with open('GED_Result_testingO03/GED_Result.pkl', 'rb') as fp:
    targets = pickle.load(fp)
  with open("Testingsets_O03/graphs_random_dataset_pytorch.pkl", "rb") as fp:
    graph_data = pickle.load(fp)
  
  edge_dim = 3 
  in_channels = 12
  out_channels = 32
  checkpoint = torch.load("models/Embedding_model_RGCNConv.pth")
  dummy_loader = DataLoader(graph_data, batch_size=1)  # Dummy loader for histogram computation
  # deg = PNAConv.get_degree_histogram(dummy_loader)
  dataset = GraphDataset(graph_data, graph_pair_idx, targets)
  testset = DataLoader(dataset, batch_size=1)  
  gnn = RGCN_Pytorch.GCCGraphInfer(in_channels, out_channels, num_relations=3)
  gnn.load_state_dict(checkpoint['model_state_dict'])
  gnn.eval()
  embeddings = list()
  L2s = list()
  targets = list()
  MAE = 0.0
  count = 0
  with torch.no_grad():
    for data1, data2, target in testset:
      out1 = gnn(data1)
      out2 = gnn(data2)
      L2 = (out1 - out2).pow(2).sum(1).pow(1/2)
      print("L2:{}, Target:{}".format(L2, target/100))
      MAE = (math.fabs(L2-(target/100)) / (target/100))
      L2s.append(L2)
      targets.append(target/100)
      count += 1
      print("Count:{}".format(count))
  # print(MAE / (count))
  # print(L2s / count)

  import matplotlib.pyplot as plt
  L2s = np.asarray(L2s).reshape(-1,)
  targets = np.asarray(targets).reshape(-1,)
  # coef = np.polyfit(L2s, targets, 1)
  # poly1d_fn = np.poly1d(coef)
  plt.scatter(L2s, targets, color='blue', alpha=0.7)
  # plt.plot(L2s, poly1d_fn(L2s), color='red', alpha=0.7)
  plt.savefig("rgcn.pdf")
  # with open("pna_training.pkl", "wb") as fp:
  #   pickle.dump(L2s, fp)
  # with open("targets.pkl", "wb") as fp:
  #   pickle.dump(targets, fp)
  # a, residuals, _, _, _ = np.polyfit(L2s, targets, 1, full=True)
  # e = a[0]
  # r = a[1]
  # y_fit = e * L2s + r
  # print("e:{}".format(e))
  # print("r:{}".format(r))
  # plt.scatter(y_fit, targets, color='blue', alpha=0.7, edgecolor='k')
  # plt.scatter(L2s, targets, color='red', alpha=0.7)
  # plt.plot(L2s, y_fit, color='red')
  # # 添加标题和标签
  # plt.title('Scatter Plot Example')
  # plt.xlabel('X-axis')
  # plt.ylabel('Y-axis')
  # # 显示图形
  # plt.savefig("a.png",dpi=800)

  # less10 = 0
  # less15 = 0
  # less30 = 0
  # larger30 = 0
  # for a, b in zip(y_fit, targets):
  #   if math.fabs(a - b)/b < 0.1:
  #     less10 += 1
  #   elif math.fabs(a - b)/b < 0.15:
  #     less15 += 1
  #   elif math.fabs(a - b)/b < 0.3:
  #     less30 += 1
  #   else:
  #     larger30 += 1
  # print("less10:{}".format(less10))
  # print("less15:{}".format(less15))
  # print("less30:{}".format(less30))
  # print("larger30:{}".format(larger30))
   
  '''
  y_fit:
  less10:21
  less15:3
  less30:24
  larger30:36

  l2-norms:
  Count:84
  less10:18
  less15:1
  less30:20
  larger30:45
  '''
    