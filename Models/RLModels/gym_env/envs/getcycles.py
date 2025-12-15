import os
import re
import shutil
import subprocess
from numpy import indices

passes_18 = "simplifycfg sroa early-cse ipsccp globalopt typepromotion instcombine speculative-execution jump-threading correlated-propagation reassociate loop-instsimplify loop-simplifycfg licm loop-rotate loop-idiom indvars loop-deletion mldst-motion gvn sccp bdce adce memcpyopt dse loop-vectorize loop-load-elim slp-vectorizer loop-unroll instsimplify"
passes_10 = "simplifycfg sroa early-cse ipsccp globalopt type-promotion instcombine speculative-execution jump-threading correlated-propagation reassociate loop-instsimplify loop-simplifycfg licm loop-rotate loop-idiom indvars loop-deletion mldst-motion gvn sccp bdce adce memcpyopt dse loop-vectorize loop-load-elim slp-vectorizer loop-unroll instsimplify"

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
DATASET_DIR = os.path.join(ROOT_DIR, "Dataset")
TRAININGSET_DIR = os.path.join(DATASET_DIR, "trainingset")
CONFIG_PATH = os.path.join(ROOT_DIR, "config.txt")
QOR_ESTIMATOR_BIN = os.path.join(ROOT_DIR, "Analysis_tools", "qor_estimator", "qor_estimator")
FEATURE_TESTS_DIR = os.path.join(os.path.dirname(__file__), "Feature_Cycles_Tests")

def qw(s):
  """
  Examples :
    >>> print(qw(“ -correlated-propagation -scalarrepl -lowerinvoke”))
    (-correlated-propagation, -scalarrepl, -lowerinvoke)

  Args:
    s (str):  s is a list of all the possible passes that can be used (the passes shoul dvbe separated by whitespace).

  Returns:
    Returns a tuple of strings where each element is a pass(used for optimization) from s.
  """
  return tuple(s.split())


def countPasses18():
  """
  Examples :
    >>> print(countPasses())
    47

  Returns:
    Returns the number of passes that opt_passes_str contains (opt_passes_str is declared and assigned at the beginning of this class and contains 47 passes).
  """

  count=len(qw(passes_18))
  return count

def countPasses10():
  """
  Examples :
    >>> print(countPasses())
    47

  Returns:
    Returns the number of passes that opt_passes_str contains (opt_passes_str is declared and assigned at the beginning of this class and contains 47 passes).
  """

  count=len(qw(passes_10))
  return count
    
opt_passes_18 = qw(passes_18)
opt_passes_10 = qw(passes_10)

# Get a tuple of optimizations
def getPasses18(opt_indice):
  """
  Examples :
    >>> print(getPasses([0,1]))
    (-correlated-propagation, -scalarrepl)

  Args:
    Opt_indice (list, optional): opt_indice is a list of integers where each element represents the index of the pass to grab from opt_passes list. 

  Returns:
    Returns a tuple of optimizations from opt_passes.
  """
  return map((lambda x: opt_passes_18[x]), opt_indice)

def getPasses10(opt_indice):
  """
  Examples :
    >>> print(getPasses([0,1]))
    (-correlated-propagation, -scalarrepl)

  Args:
    Opt_indice (list, optional): opt_indice is a list of integers where each element represents the index of the pass to grab from opt_passes list. 

  Returns:
    Returns a tuple of optimizations from opt_passes.
  """
  return map((lambda x: opt_passes_10[x]), opt_indice)
  

def passes2indice(passes):
  """
  Examples :
    >>> print(passes2indice(“ -correlated-propagation hi -scalarrepl -lowerinvoke blob”))
    (-correlated-propagation, -scalarrepl, -lowerinvoke)
                 
  Args:
    passes (str): string of passes separated by whitespaces.

  Returns:
    Returns a list of all the optimization passes given in the string parameter passes if they exist in opt_passes (which is the list of passes we defined in this class).
  """
  indices = []
  passes = qw(passes)
  for passs in passes:
    for i in range(len(opt_passes)):
      if passs == opt_passes[i]:
        indices.append(i)
        break
  return indices


def _ensure_run_path(path: str) -> str:
  abs_path = os.path.abspath(path)
  os.makedirs(abs_path, exist_ok=True)
  return abs_path


def _compile_bitcode(source_file: str, output_bc: str, opt_level: int = 0) -> subprocess.CompletedProcess:
  clang_command = [
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
  return subprocess.run(clang_command, capture_output=True, text=True)


def _run_estimator(bitcode_path: str, pgm_name: str, run_path: str, opt_indices) -> tuple:
  if not os.path.isfile(QOR_ESTIMATOR_BIN):
    return 0, 0, f"qor_estimator not found at {QOR_ESTIMATOR_BIN}"
  if not os.path.isfile(CONFIG_PATH):
    return 0, 0, f"config file not found at {CONFIG_PATH}"

  command = [
      QOR_ESTIMATOR_BIN,
      "dut",
      CONFIG_PATH,
      bitcode_path,
  ]
  proc = subprocess.run(command, capture_output=True, text=True)
  if proc.returncode != 0:
    return 0, 0, proc.stderr.strip() or "qor_estimator execution failed"

  output = proc.stdout
  latency_match = re.search(r"latency is (\d+)", output)
  valid_match = re.search(r"\b(invalid|valid)\b", output)
  if latency_match and valid_match:
    if valid_match.group() == "valid":
      return int(latency_match.group(1)), 1, ""
    return 0, 0, "qor_estimator reported invalid design"

  passes_desc = opt_indices if opt_indices is not None else []
  errs_log = (
      "There is something wrong when getting cycles"
      + f" Current program {pgm_name}"
      + f" Current rundir {run_path}"
      + f" Current passes {passes_desc}"
  )
  return 0, 0, errs_log


def light_hls_getHWCycles(pgm_name, pgm_path, opt_indice, run_path=None):
  """
  Examples :
    >>> print(getHWCycles(c_code, [“-correlated-propagation”, “-scalarrepl”, “-lowerinvoke”]))
    (55, True)

  Args:
    c_code (str): The file name of a code written in C programming language
    Opt_indice (list, optional): opt_indice is a list of integers where each element represents the index of the pass to grab from opt_passes list. 
    path (str): This parameter represents the path of the directory we are interested in. Defaults to current path.
    sim (bool, optional): sim should be True if you want the arguments used to launch the process to be “make clean p v -s”, or sim should be False 
      if you want the argument used to launch the process to be "make clean accelerationCycle -s". Defaults to False

  Returns:
    Returns a tuple where the first element is an integer that represents the number of cycle counts it took to run the synthesized circuit 
    (the second element doesn't matter).

  """
  run_root = _ensure_run_path(run_path or FEATURE_TESTS_DIR)
  valid = 1
  errs_log = ""
  ga_seq = getPasses18(opt_indice)
  new_passes = list(ga_seq)
  ir_dir = os.path.join(run_root, "cycleIRfile")
  if os.path.exists(ir_dir):
    shutil.rmtree(ir_dir)
  os.makedirs(ir_dir, exist_ok=True)

  source_file = os.path.join(os.path.abspath(pgm_path), f"{pgm_name}.cc")
  if not os.path.isfile(source_file):
    return 0, 0, f"Source file not found: {source_file}"

  target_source = os.path.join(ir_dir, f"{pgm_name}.cc")
  shutil.copy(source_file, target_source)

  initial_bc = os.path.join(ir_dir, f"{pgm_name}.0.bc")
  result_0 = _compile_bitcode(target_source, initial_bc, opt_level=0)
  if result_0.returncode != 0:
    return 0, 0, result_0.stderr.strip() or "clang failed to generate bitcode"

  last_bc = initial_bc
  for idx, pass_name in enumerate(new_passes):
    next_bc = os.path.join(ir_dir, f"{pgm_name}.{idx + 1}.bc")
    opt_command = [
        "opt",
        f"-passes={pass_name}",
        last_bc,
        "-o",
        next_bc,
    ]
    result_1 = subprocess.run(opt_command, capture_output=True, text=True)
    if result_1.returncode != 0:
      errs_log = result_1.stderr.strip() or "opt failed"
      valid = 0
      break
    last_bc = next_bc

  top_bc_path = os.path.join(ir_dir, f"{pgm_name}top.bc")
  if os.path.isfile(last_bc):
    shutil.move(last_bc, top_bc_path)
  else:
    shutil.copy(initial_bc, top_bc_path)

  hw_cycle, valid_flag, estimator_err = _run_estimator(top_bc_path, pgm_name, run_root, opt_indice)
  if estimator_err:
    errs_log = estimator_err
  return hw_cycle, valid_flag, errs_log
  
def get_Ox_Cycles(pgm_name, pgm_path, Opt_level, run_path=None):
  run_root = _ensure_run_path(run_path or FEATURE_TESTS_DIR)
  ir_dir = os.path.join(run_root, "cycleIRfile")
  if os.path.exists(ir_dir):
    shutil.rmtree(ir_dir)
  os.makedirs(ir_dir, exist_ok=True)

  source_file = os.path.join(os.path.abspath(pgm_path), f"{pgm_name}.cc")
  if not os.path.isfile(source_file):
    return 0, 0, f"Source file not found: {source_file}"

  target_source = os.path.join(ir_dir, f"{pgm_name}.cc")
  shutil.copy(source_file, target_source)

  output_bc = os.path.join(ir_dir, "top.bc")
  temp_bc = os.path.join(ir_dir, f"{pgm_name}.bc")
  result_0 = _compile_bitcode(target_source, temp_bc, opt_level=Opt_level)
  if result_0.returncode != 0:
    return 0, 0, result_0.stderr.strip() or "clang failed to generate bitcode"

  shutil.move(temp_bc, output_bc)
  hw_cycle, valid, errs_log = _run_estimator(output_bc, pgm_name, run_root, opt_indice=None)
  return hw_cycle, valid, errs_log

def main():
  # indice = [35] 
  # 35 : licm ; 66 : loop-unroll
  '''
  Indice = [35] : cycles = 91004
  Indice = [66, 35] : cycles = 91004
  Indice = [33, 26, 68, 49, 25, 36, 23, 4, 15, 19, 28, 8, 13, 8, 54, 25, 6, 52, 44, 58, 68, 39, 53, 69, 45, 47, 37, 64, 40, 51, 50, 12, 66, 4, 19, 34, 44, 12, 70, 28, 32, 38, 53, 53, 50, 14, 29, 52, 58, 44, 33, 22, 66] : cycles = 90491
  Indice = [33, 26, 68, 49, 25, 36, 23, 4, 15, 19, 28, 8, 13, 8, 54, 25, 6, 52, 44, 58, 68, 39, 53, 69, 45, 47, 37, 64, 40, 51, 50, 12, 66, 4, 19, 34, 44, 12, 70, 28, 32, 38, 53, 53, 50, 14, 29, 52, 58, 44, 33, 22, 66, 35] : cycles = 89883
  
  [36] 181244
  [15] 202844
  [25] 181244
  [6] 181242
  [44] 182277
  [66] 181244
  [66] 181244
  [35] 91004
  
  [36] 181244
  [36, 15] 202843
  [36, 15, 25] 202843
  [36, 15, 25, 6] 181244
  [36, 15, 25, 6, 44] 94842
  [36, 15, 25, 6, 44, 66] 94842
  [36, 15, 25, 6, 44, 66, 66] 92283
  [36, 15, 25, 6, 44, 66, 66, 35] 89883
  
  [36] 181244
  [36, 6] 181244
  [36, 6, 44] 181242
  [36, 6, 44, 66] 138042
  [36, 6, 44, 66, 66] 111290
  [36, 6, 44, 66, 66, 35] 87994
  
  loop-rotate
  jump-threading
  sroa
  gvn
  loop-unroll
  loop-unroll
  licm
  
  [36,  44, 6, 66, 66, 35] cycles : 95316
  [36,  6, 44, 66, 66, 35] cycles : 87994
  Order of 6 and 44 should be important, and [6, 44] should be before 66
  36 should be head of 66
  '''
  passes = []
  
  '''
  Motivation 1
  test1
  
  licm
  instcombine
  loop-unroll
  loop-vectorize
  loop-rotate
  instcombine
  gvn

  typepromotion
  
  689682
  
  [35] 881564
  [35, 15] 874523
  [35, 15, 62] 1317803
  [35, 15, 62, 36] 1318203
  [35, 15, 62, 36, 15] 1287662
  [35, 15, 62, 36, 15, 66] 1287642
  [35, 15, 62, 36, 15, 66, 13] 690662
  [35, 15, 62, 36, 15, 66, 13, 44] 689682
  [35, 74, 41, 35, 69, 24, 35, 18, 7, 8, 10, 53, 13, 50, 58, 8, 47, 72, 35, 29, 50, 52, 16, 15, 35, 69, 61, 44, 67, 69, 69, 72, 58, 4, 10, 7, 62, 36, 16, 43, 15, 57, 46, 60, 16, 22, 35, 18, 4, 44, 29, 22, 59, 57, 70, 69, 51, 20, 64, 28, 47, 69, 2, 51, 7, 15, 15, 66, 17, 12, 45, 46, 15, 13]
  
  Motivation 2
  GED(60, 37) : 385
  GED(60, 47) : 2513
  GED(47, 37) : 2553
  {P1 : 60, P2 : 37, P3 : 47}
  {S1 : [44, 66, 10, 6, 34, 66, 44, 36], S2 : [7, 36, 35, 62, 15, 6, 39], S3 : [15, 36, 44, 41]}
  Current Program:random47 -- Cycles:4316098 -- Passes:[7, 36, 35, 62, 15, 6, 39] early-cse loop-rotate licm loop-vectorize instcombine sroa indvars {'initial cycle': 4801645, 'O3 cycle': 4522643, 'rl cycle': 4299236, 'rl_60 cycle': 4531062}
  Current Program:random37 -- Cycles:3682522 -- Passes:[15, 36, 44, 66/41] instcombine loop-rotate gvn loop-unroll {'initial cycle': 3846362, 'O3 cycle': 3698906, 'rl cycle': 3682552, 'rl_60 cycle': 3748058, 'rl_47 cycle': 3813594, 'rl reverse loop-unroll&loop-rotate': 3813594}
  Current Program:random60 -- Cycles:5310996 -- Passes:[44, 66, 10, 6, 34, 66, 44, 36, 15] gvn loop-unroll ipsccp sroa loop-simplifycfg loop-unroll gvn loop-rotate instcombine {'initial cycle': 10251490, 'O3 cycle': 5858348, 'rl cycle': 5310996, 'rl reverse loop-unroll&loop-rotate': 6965896}
  
  '''
  import pickle
  O3_info = {}
  path = TRAININGSET_DIR
  run_dir = _ensure_run_path(FEATURE_TESTS_DIR)
  for i in range(0, 80):
    pgm = "random" + str(i)
    cycle, _, _ = get_Ox_Cycles(pgm_name=pgm, pgm_path=path, Opt_level=1 , run_path=run_dir)
    # cycle, _, _ = light_hls_getHWCycles(pgm_name=pgm.replace(".cc", ""), pgm_path=path, opt_indice=[], run_path=FEATURE_TESTS_DIR)
    cycles = {}
    cycles['cycle'] = cycle
    O3_info[pgm.replace(".cc", "")] = cycles
  with open("O1_info.pkl", "wb") as f:
    pickle.dump(O3_info, f)
  
  # cycle, _, _ = light_hls_getHWCycles(pgm_name=c_code, pgm_path=os.path.join(DATASET_DIR, "testsset", "test0"), opt_indice=indices, run_path=FEATURE_TESTS_DIR)
  # cycle, _, _ = get_Ox_Cycles(pgm_name=c_code, pgm_path=os.path.join(DATASET_DIR, "testsset", "test0"), Opt_level=3 , run_path=FEATURE_TESTS_DIR)
  # print(cycle)

def prune_passes(file_name: str) -> None:
  import pickle
  prune_passes_pgm = {}
  with open(file_name, "rb") as f:
    random_programs = pickle.load(f)
    for i in random_programs:
      passes = random_programs[i]['passes']
      init_cycle = random_programs[i]['cycle']
      print("Init_cycles: {}".format(init_cycle) + " Current_Program: {}".format(i))
      pass_len = len(passes)
      for n in reversed(range(pass_len)):
        buffer_pass = passes.pop(n)
        print(passes)
        cycle, _, _ = light_hls_getHWCycles(pgm_name=i, pgm_path=TRAININGSET_DIR, opt_indice=passes, run_path=_ensure_run_path(FEATURE_TESTS_DIR))
        print("Current Cycles: {}".format(cycle))
        if cycle > init_cycle or cycle == 0:
          passes.insert(n, buffer_pass)
        print(passes)
      c = {}
      c['passes'] = passes
      c['cycle'] = init_cycle
      prune_passes_pgm[i] = c
  with open("prune_passes_pgm.pkl", "wb") as f:
    pickle.dump(prune_passes_pgm, f)

if __name__ == "__main__":
  main()
 