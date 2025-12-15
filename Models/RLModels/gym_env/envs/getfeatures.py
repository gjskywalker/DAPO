import os
import re
import shutil
import subprocess
import numpy as np
import pandas as pd
import torch
from torch_geometric.data import Data
from gym_env.envs.get_TestBench import get_random

features = ["Number of BB where total args for phi nodes > 5", "Number of BB where total args for phi nodes is [1, 5]", "Number of BB's with 1 predecessor", "Number of BB's with 1 predecessor and 1 successor", "Number of BB's with 1 predecessor and 2 successors", "Number of BB's with 1 successor", "Number of BB's with 2 predecessors", "Number of BB's with 2 predecessors and 1 successor", "Number of BB's with 2 predecessors and successors", "Number of BB's with 2 successors", "Number of BB's with >2 predecessors", "Number of BB's with Phi node Number in range (0, 3]", "Number of BB's with more than 3 Phi nodes", "Number of BB's with no Phi nodes", "Number of Phi-nodes at beginning of BB", "Number of branches", "Number of calls that return an int", "Number of critical edges", "Number of edges", "Number of occurrences of 32-bit integer constants", "Number of occurrences of 64-bit integer constants", "Number of occurrences of constant 0", "Number of occurrences of constant 1", "Number of unconditional branches", "Binary operations with a constant operand", "Number of AShr insts", "Number of Add insts", "Number of Alloca insts", "Number of And insts", "Number of BB's with instructions between [15, 500]", "Number of BB's with less than 15 instructions", "Number of BitCast insts", "Number of Br insts", "Number of Call insts", "Number of GetElementPtr insts", "Number of ICmp insts", "Number of LShr insts", "Number of Load insts", "Number of Mul insts", "Number of Or insts", "Number of PHI insts", "Number of Ret insts", "Number of SExt insts", "Number of Select insts", "Number of Shl insts", "Number of Store insts", "Number of Sub insts", "Number of Trunc insts", "Number of Xor insts", "Number of ZExt insts", "Number of basic blocks", "Number of instructions (of all types)", "Number of memory instructions", "Number of non-external functions", "Total arguments to Phi nodes", "Unary"] 

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
ANALYSIS_TOOLS_DIR = os.path.join(ROOT_DIR, "Analysis_tools")
FEATURE_EXTRACTOR_BIN = os.path.join(ANALYSIS_TOOLS_DIR, "feature_exctractor", "feature_exctractor")
FEATURE_TESTS_DIR = os.path.join(os.path.dirname(__file__), "Feature_Cycles_Tests")
DEFAULT_CYCLE_IR_DIR = os.path.join(FEATURE_TESTS_DIR, "cycleIRfile")


def _ensure_cycle_ir_dir(base_path: str) -> str:
  abs_base = os.path.abspath(base_path)
  ir_dir = os.path.join(abs_base, "cycleIRfile")
  os.makedirs(ir_dir, exist_ok=True)
  return ir_dir


def _compile_bitcode(source_file: str, output_bc: str, opt_level: int = 0) -> subprocess.CompletedProcess:
  clang_cmd = [
    "clang",
    f"-O{opt_level}",
    "-Xclang",
    "-disable-O0-optnone",
    "-emit-llvm",
    "-S",
    source_file,
    "-o",
    output_bc,
  ]
  return subprocess.run(clang_cmd, capture_output=True, text=True)

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

def harp_get_feature(pgm_name, pre_graphs, path="."):
  from Program_Representation_Learning.HarpEncoder import IR2Graph, graph2tensor

  pregraph = pre_graphs
  ir_dir = _ensure_cycle_ir_dir(path)
  ll_path = os.path.join(ir_dir, f"{pgm_name}top.v10.ll")
  return graph2tensor(IR2Graph(ll_path))[0]
  
def ir2vec_get_feature(pgm_name, pre_graphs, path="."):
  pregraph = pre_graphs
  ir_dir = _ensure_cycle_ir_dir(path)
  embedding_path = os.path.join(ir_dir, "embedding.txt")
  top_bc = os.path.join(ir_dir, f"{pgm_name}top.bc")

  result = subprocess.run(
    [
      "ir2vec",
      "-fa",
      "-dim",
      "75",
      "-o",
      embedding_path,
      "-level",
      "p",
      top_bc,
    ],
    cwd=ir_dir,
    capture_output=True,
    text=True,
  )
  if result.returncode == 0 and os.path.isfile(embedding_path):
    with open(embedding_path, "r", encoding="utf-8") as file:
      line = file.readline().strip()
      embedding_vector = np.array([float(x) for x in line.split()])
    os.remove(embedding_path)
    return embedding_vector.reshape(1, -1)
  return pregraph

def gnn_get_feature(pgm_name, pre_graphs, path="."):
  pregraph = pre_graphs
  if not os.path.isfile(FEATURE_EXTRACTOR_BIN):
    return pregraph

  ir_dir = _ensure_cycle_ir_dir(path)
  top_bc = os.path.join(ir_dir, f"{pgm_name}top.bc")
  command = [
    FEATURE_EXTRACTOR_BIN,
    ir_dir,
    pgm_name,
    top_bc,
  ]
  result = subprocess.run(
    command,
    cwd=os.path.dirname(FEATURE_EXTRACTOR_BIN),
    capture_output=True,
    text=True,
  )
  if result.returncode == 0:
    edge_path = os.path.join(ir_dir, f"Edge_{pgm_name}.csv")
    node_path = os.path.join(ir_dir, f"Node_Feature_{pgm_name}.csv")
    if os.path.isfile(edge_path) and os.path.isfile(node_path):
      return read_file(edge_path, node_path)
  return pregraph

def main():
    training_set = get_random(idx=0, pgm_num=70)
    Embeddings_static = list()
    ir_dir = _ensure_cycle_ir_dir(FEATURE_TESTS_DIR)
    for pgm, pgm_path in training_set:
      source_file = os.path.join(pgm_path, pgm)
      if not os.path.isfile(source_file):
        continue

      target_source = os.path.join(ir_dir, pgm)
      shutil.copy(source_file, target_source)

      bitcode_name = pgm.replace(".cc", "") + "top.bc"
      bitcode_path = os.path.join(ir_dir, bitcode_name)
      compile_result = _compile_bitcode(target_source, bitcode_path, opt_level=0)
      if compile_result.returncode != 0:
        continue

      m = extractnum_run_stats(
        pgm_name=pgm.replace(".cc", ""),
        opt_indice=[],
        path=FEATURE_TESTS_DIR,
      )
      Embeddings_static.append(m)
      import pickle
      with open("Embeddings_static.pkl", "wb") as fp:
        pickle.dump(Embeddings_static, fp)

if __name__ == "__main__":
    main() 

