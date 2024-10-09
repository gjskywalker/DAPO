import re
import subprocess
import os
from numpy import indices

# O3_str = "targetlibinfo tti tbaa scoped-noalias assumption-cache-tracker profile-summary-info forceattrs inferattrs ipsccp globalopt domtree mem2reg deadargelim domtree basicaa aa instcombine simplifycfg pgo-icall-prom basiccg globals-aa prune-eh inline functionattrs argpromotion domtree sroa early-cse speculative-execution lazy-value-info jump-threading correlated-propagation simplifycfg domtree basicaa aa instcombine libcalls-shrinkwrap tailcallelim simplifycfg reassociate domtree loops loop-simplify lcssa-verification lcssa basicaa aa scalar-evolution loop-rotate licm loop-unswitch simplifycfg domtree basicaa aa instcombine loops loop-simplify lcssa-verification lcssa scalar-evolution indvars loop-idiom loop-deletion loop-unroll mldst-motion aa memdep gvn basicaa aa memdep memcpyopt sccp domtree demanded-bits bdce basicaa aa instcombine lazy-value-info jump-threading correlated-propagation domtree basicaa aa memdep dse loops loop-simplify lcssa-verification lcssa aa scalar-evolution licm postdomtree adce simplifycfg domtree basicaa aa instcombine barrier elim-avail-extern basiccg rpo-functionattrs globals-aa float2int domtree loops loop-simplify lcssa-verification lcssa basicaa aa scalar-evolution loop-rotate loop-accesses lazy-branch-prob lazy-block-freq opt-remark-emitter loop-distribute loop-simplify lcssa-verification lcssa branch-prob block-freq scalar-evolution basicaa aa loop-accesses demanded-bits lazy-branch-prob lazy-block-freq opt-remark-emitter loop-vectorize loop-simplify scalar-evolution aa loop-accesses loop-load-elim basicaa aa instcombine scalar-evolution demanded-bits slp-vectorizer simplifycfg domtree basicaa aa instcombine loops loop-simplify lcssa-verification lcssa scalar-evolution loop-unroll instcombine loop-simplify lcssa-verification lcssa scalar-evolution licm alignment-from-assumptions strip-dead-prototypes globaldce constmerge domtree loops branch-prob block-freq loop-simplify lcssa-verification lcssa basicaa aa scalar-evolution branch-prob block-freq loop-sink instsimplify verify write-bitcode"
# opt_passes_str = "loweratomic loop-distribute sink correlated-propagation loop-unroll globalopt bdce deadargelim sccp loop-reduce lcssa verify simple-loop-unswitch forceattrs elim-avail-extern simplifycfg dse jump-threading indvars tailcallelim memcpyopt globaldce adce rpo-function-attrs break-crit-edges loop-load-elim loop-rotate licm inferattrs sroa instsimplify strip-nondebug lowerinvoke gvn instcombine loop-simplify strip loop-deletion alignment-from-assumptions loop-sink constmerge mem2reg ipsccp lowerswitch slp-vectorizer reassociate"
# new_opt_passes_str = "simple-loop-unswitch rpo-function-attrs forceattrs sccpglobalopt forceattrs alignment-from-assumptions globaldce correlated-propagation loop-sink instcombine verify memcpyopt loop-simplify tailcallelim deadargelim elim-avail-extern inferattrs bdce licm dse slp-vectorizer ipsccp constmerge instsimplify simplifycfg jump-threading loop-load-elim adce lcssa loop-distribute"  
opt_passes_str = "annotation2metdadata forceattrs inferattrs coro-early lower-expect simplifycfg sroa early-cse callsite-splitting openmp-opt ipsccp called-value-propagation globalopt typepromotion argpromotion instcombine aggressive-instcombine always-inline inliner-wrapper wholeprogramdevirt module-inline inline rpo-function-attrs openmp-opt-cgscc speculative-execution jump-threading correlated-propagation libcalls-shrinkwrap tailcallelim reassociate constraint-elimination loop-simplify lcssa loop-instsimplify loop-simplifycfg licm loop-rotate simple-loop-unswitch loop-idiom indvars loop-deletion loop-unroll-full vector-combine mldst-motion gvn sccp bdce adce memcpyopt dse move-auto-init coro-elide coro-split coro-cleanup deadargelim  elim-avail-extern recompute-globalsaa float2int lower-constant-intrinsics chr loop-distribute inject-tli-mappings loop-vectorize infer-alignment loop-load-elim slp-vectorizer loop-unroll alignment-from-assumptions loop-sink instsimplify div-rem-pairs constmerge cg-profile rel-lookup-table-converter annotation-remarks verify"
old_passes_str = "correlated-propagation scalarrepl lowerinvoke strip strip-nondebug sccp globalopt gvn jump-threading globaldce loop-unswitch scalarrepl-ssa loop-reduce break-crit-edges loop-deletion reassociate lcssa codegenprepare memcpyopt functionattrs loop-idiom lowerswitch constmerge loop-rotate partial-inliner inline early-cse indvars adce loop-simplify instcombine simplifycfg dse loop-unroll lower-expect tailcallelim licm sink mem2reg prune-eh functionattrs ipsccp deadargelim sroa loweratomic terminate"

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


def countPasses():
  """
  Examples :
    >>> print(countPasses())
    47

  Returns:
    Returns the number of passes that opt_passes_str contains (opt_passes_str is declared and assigned at the beginning of this class and contains 47 passes).
  """

  count=len(qw(opt_passes_str))
  return count
    
opt_passes = qw(opt_passes_str)

# Get a tuple of optimizations
def getPasses(opt_indice):
  """
  Examples :
    >>> print(getPasses([0,1]))
    (-correlated-propagation, -scalarrepl)

  Args:
    Opt_indice (list, optional): opt_indice is a list of integers where each element represents the index of the pass to grab from opt_passes list. 

  Returns:
    Returns a tuple of optimizations from opt_passes.
  """
  return map((lambda x: opt_passes[x]), opt_indice)

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

def light_hls_getHWCycles(pgm_name, pgm_path, opt_indice, run_path="."):
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
  valid = 1
  errs_log = ""
  ga_seq = getPasses(opt_indice) # in ga_seq are the new passes
  new_passes = list(ga_seq)
  IRfilePath = run_path+"/cycleIRfile/"
  rm_cmd = "rm -rf "+IRfilePath
  os.system(rm_cmd)
  mkdir_cmd = "mkdir "+IRfilePath
  os.system(mkdir_cmd)
  cp_cmd = "cp "+ pgm_path + pgm_name +".cc " +IRfilePath
  os.system(cp_cmd)
  # This instruction is used to disable clang's optimization, and get an initial bitcode file.
  clang_command = "clang -O0 -Xclang -disable-O0-optnone -emit-llvm -S "+IRfilePath+pgm_name+".cc "+"-o "+IRfilePath+pgm_name+".0.bc"
  result_0 = subprocess.run(clang_command, shell=True, capture_output=True)
  if result_0.returncode == 0:
    length = len(new_passes)
    if(length != 0):
      for pass_order in range(0, length):
        opt_command = "opt -passes="+new_passes[pass_order]+" "+IRfilePath+pgm_name+'.'+str(pass_order)+".bc -o "+IRfilePath+pgm_name+'.'+str(pass_order+1)+".bc"
        result_1 = subprocess.run(opt_command, shell=True, capture_output=True)
        if result_1.returncode != 0:
          pass_order = pass_order -1 
          break
      mv_command = "mv "+IRfilePath+pgm_name+'.'+str(pass_order+1)+".bc "+IRfilePath+pgm_name+"top.bc "
      os.system(mv_command)
      # os.system(cp1_command) 
    else:
      mv_command = "mv "+IRfilePath+pgm_name+'.'+"0.bc "+IRfilePath+pgm_name+"top.bc "
      os.system(mv_command)
      # os.system(cp1_command) 
    

  #TODO: Use this new pass to run opt and get the bitcode file, and transfer the bc file to our cycle estimator to get the cycle
  execute_command = " "
  proc = subprocess.Popen([execute_command], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
  (out, err) = proc.communicate()
  latency_match = re.search(r"latency is (\d+)", out.decode("utf-8"))
  valid_match = re.search(r"\b(invalid|valid)\b", out.decode("utf-8"))
  if latency_match:
    if valid_match:
      if valid_match.group() == "valid":
        hw_cycle = latency_match.group(1)
        hw_cycle = int(hw_cycle)
      elif valid_match.group() == "invalid":
        hw_cycle = 0
        valid = 0
  else:
    errs_log = "There is something wrong when getting cycles" + " Current program {}".format(pgm_name) + " Current rundir {}".format(run_path) + " Current passes {}".format(opt_indice)
    hw_cycle = 0
    valid = 0
    
  return hw_cycle, valid, errs_log
  
def get_Ox_Cycles(pgm_name, pgm_path, Opt_level, run_path="."):
  valid = 1
  errs_log = ""
  IRfilePath = run_path+"/cycleIRfile"
  rm_cmd = "rm -rf "+IRfilePath
  os.system(rm_cmd)
  mkdir_cmd = "mkdir "+IRfilePath
  os.system(mkdir_cmd)
  cp_cmd = "cp "+ pgm_path + pgm_name +".cc " +IRfilePath
  os.system(cp_cmd)
  # This instruction is used to disable clang's optimization, and get an initial bitcode file.
  clang_command = "clang -O"+str(Opt_level)+" -Xclang -disable-O0-optnone -emit-llvm -S "+IRfilePath+'/'+pgm_name+".cc "+"-o "+IRfilePath+'/'+pgm_name+".bc"
  result_0 = subprocess.run(clang_command, shell=True, capture_output=True)
  if result_0.returncode == 0:
      mv_command = "mv "+IRfilePath+'/'+pgm_name+".bc "+IRfilePath+'/'+"top.bc "
      os.system(mv_command)
      cp1_command = "cp "+IRfilePath+'/'+"top.bc "+"" 
      os.system(cp1_command)  
      #TODO: Use this new pass to run opt and get the bitcode file, and transfer the bc file to our cycle estimator to get the cycle
      execute_command = "cd  && ./AccelerationCycle dut ../config.txt top.bc"
      proc = subprocess.Popen([execute_command], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
      (out, err) = proc.communicate()
      latency_match = re.search(r"latency is (\d+)", out.decode("utf-8"))
      valid_match = re.search(r"\b(invalid|valid)\b", out.decode("utf-8"))
      if latency_match:
        if valid_match:
          if valid_match.group() == "valid":
            hw_cycle = latency_match.group(1)
            hw_cycle = int(hw_cycle)
          elif valid_match.group() == "invalid":
            hw_cycle = 0
            valid = 0
      else:
        errs_log = "There is something wrong when getting cycles" + " Current program {}".format(pgm_name) + " Current rundir {}".format(run_path)
        hw_cycle = 0
        valid = 0
    
  return hw_cycle, valid, errs_log
