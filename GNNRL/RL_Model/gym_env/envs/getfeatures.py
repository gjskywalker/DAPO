import subprocess
import numpy as np
import pandas as pd
# import networkx as nx
import torch
from torch_geometric.data import Data
# import stellargraph as sg
from subprocess import call
# from tensorflow import keras
# from gym_env.envs.getcycles import *
# from get_TestBench import get_random
import os
import re 

features = ["Number of BB where total args for phi nodes > 5", "Number of BB where total args for phi nodes is [1, 5]", "Number of BB's with 1 predecessor", "Number of BB's with 1 predecessor and 1 successor", "Number of BB's with 1 predecessor and 2 successors", "Number of BB's with 1 successor", "Number of BB's with 2 predecessors", "Number of BB's with 2 predecessors and 1 successor", "Number of BB's with 2 predecessors and successors", "Number of BB's with 2 successors", "Number of BB's with >2 predecessors", "Number of BB's with Phi node Number in range (0, 3]", "Number of BB's with more than 3 Phi nodes", "Number of BB's with no Phi nodes", "Number of Phi-nodes at beginning of BB", "Number of branches", "Number of calls that return an int", "Number of critical edges", "Number of edges", "Number of occurrences of 32-bit integer constants", "Number of occurrences of 64-bit integer constants", "Number of occurrences of constant 0", "Number of occurrences of constant 1", "Number of unconditional branches", "Binary operations with a constant operand", "Number of AShr insts", "Number of Add insts", "Number of Alloca insts", "Number of And insts", "Number of BB's with instructions between [15, 500]", "Number of BB's with less than 15 instructions", "Number of BitCast insts", "Number of Br insts", "Number of Call insts", "Number of GetElementPtr insts", "Number of ICmp insts", "Number of LShr insts", "Number of Load insts", "Number of Mul insts", "Number of Or insts", "Number of PHI insts", "Number of Ret insts", "Number of SExt insts", "Number of Select insts", "Number of Shl insts", "Number of Store insts", "Number of Sub insts", "Number of Trunc insts", "Number of Xor insts", "Number of ZExt insts", "Number of basic blocks", "Number of instructions (of all types)", "Number of memory instructions", "Number of non-external functions", "Total arguments to Phi nodes", "Unary"] 

def extractnum_run_stats(pgm_name, opt_indice, path="."):
  IRfilePath = path+"/cycleIRfile"
  execute_cmd = "cd  && ./ExtraNum " + IRfilePath + "/" + pgm_name + "top.bc" 
  proc = subprocess.Popen([execute_cmd], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
  (out, err) = proc.communicate()
  # print (err)
  m = get_static_features_str(err)
  return m

def get_static_features_str(out):
    """ 
    Args:
        out (str): is an error message from Popen.communicate()

    Returns:
        Returns a list of all the features extracted from the bitcode file, which is the compiled version of program we need to run, that exist in our features 
        list(This is a helper function for run_stats and both functions return the same list).

    """

    feat_ids = []
    for feature in features:
      my_regex = r"(?i){}: (\d+)".format(re.escape(feature))
      #my_regex = r"\s*(\d+) instcount - " + re.escape(feature)
      p = re.compile(my_regex) 
    #   print("P:",end='')
    #   print(p)
      m = p.findall(out.decode("utf-8"))
    #   print(out.decode("utf-8"))
    #   print("m:",end = '')
    #   print(m)
      if len(m):
        feat_ids.append(int(m[0]))
      else:
        feat_ids.append(0) 
      
    return feat_ids


#parse_static_features("static_features.txt")
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
    
    x = np.array(x)
    x = torch.tensor(x, dtype=torch.int)
    edge_index = np.array(edge_index)
    edge_index = torch.tensor(edge_index, dtype=torch.int64)
    edge_attri = np.array(edge_attri)
    edge_attri = torch.tensor(edge_attri, dtype=torch.int)
    
    data = Data(node_attr=x, edge_index=edge_index.t().contiguous(), edge_attr=edge_attri)
    
    return data


def gnn_get_feature(pgm_name, pre_graphs, path="."):
  pregraph = pre_graphs
  IRfilePath = path + "/cycleIRfile/"

  execute_cmd = "cd  && ./FeatureExtractor_V5 " + IRfilePath + " " +pgm_name + " " + IRfilePath + pgm_name + "top.bc" 
  result = subprocess.run(execute_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
  if result.returncode == 0:
    graph = read_file(IRfilePath + "Edge_"+pgm_name+".csv", IRfilePath + "Node_Feature_"+pgm_name+".csv")
    return graph
  else:
    return pregraph

def main():
    training_set = get_random(idx=0, pgm_num=70)
    Embeddings_static = list()
    for (pgm, path) in training_set:
      os.system("cp " + path + pgm + "  ")
      os.system("cd  && clang -O0 -Xclang -disable-O0-optnone -emit-llvm -S " + pgm + " -o " + pgm.replace(".cc", "") + "top.bc")
      m = extractnum_run_stats(pgm_name=pgm.replace(".cc", ""), opt_indice=[], path="")
      Embeddings_static.append(m)
    import pickle
    with open("Embeddings_static.pkl", "wb") as fp:
      pickle.dump(Embeddings_static, fp)

if __name__ == "__main__":
    main() 

