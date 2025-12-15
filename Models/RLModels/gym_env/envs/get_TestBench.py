import os
from os.path import isfile, join
from shutil import copyfile

_ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
_DATASET_DIR = os.path.join(_ROOT_DIR, "Dataset")
_CROSS_VALIDATION_DIR = os.path.join(_DATASET_DIR, "Cross_Validation")
_TRAININGSET_DIR = os.path.join(_DATASET_DIR, "trainingset")
_POLYBENCH_DIR = os.path.join(_DATASET_DIR, "polybench")
_CHSTONE_DIR = os.path.join(_DATASET_DIR, "chstone")

def get_random(idx: int, pgm_num: int) -> list:
  """
  Examples :
    >>> print(get_random())
    [file1.txt, file2.txt, file3.txt, file4.txt, file5.txt, file6.txt, file7.txt, file8.txt, file9.txt, file10.txt]
    >>> print(get_random(“path/to/directory”))
    [fl1.txt, fl2.txt, fl3.txt, fl4.txt, fl5.txt, fl6.txt, fl7.txt, fl8.txt, fl9.txt, fl10.txt]
    >>> print(get_random(“path/to/directory”, 4))
    [fl1.txt, fl2.txt, fl3.txt, fl4.txt]

  Args:
    path (str, optional): The path of the directory we are interested in. Defaults to "/home/eeuser/Desktop/GRL-HLS/Dataset/Training_pre".
    N (int, optional): N is the number of benchmarks to pick from the given path. Defaults to ten.

  Returns:
    Returns a list of N strings where each element is the path to a benchmark file.
  """
  path = os.path.join(_CROSS_VALIDATION_DIR, str(idx), "trainingset")
  random_list = []
  for i in range(pgm_num):
    random_list.append(("random" + str(i) + ".cc", os.path.join(path, "")))
  return random_list

def get_random_all() -> list:
  """
  Returns a list of all random files in the cross-validation training set directories.
  
  Returns:
    list: A list of tuples where each tuple contains a file name and its corresponding path.
  """
  path = _TRAININGSET_DIR
  random_list = []
  for i in range(80):
    random_list.append(("random" + str(i) + ".cc", os.path.join(path, "")))
  return random_list

def get_test(idx: int, pgm_num: int) -> list:
  """
  Examples :
    >>> print(get_random())
    [file1.txt, file2.txt, file3.txt, file4.txt, file5.txt, file6.txt, file7.txt, file8.txt, file9.txt, file10.txt]
    >>> print(get_random(“path/to/directory”))
    [fl1.txt, fl2.txt, fl3.txt, fl4.txt, fl5.txt, fl6.txt, fl7.txt, fl8.txt, fl9.txt, fl10.txt]
    >>> print(get_random(“path/to/directory”, 4))
    [fl1.txt, fl2.txt, fl3.txt, fl4.txt]

  Args:
    path (str, optional): The path of the directory we are interested in. Defaults to "/home/eeuser/Desktop/GRL-HLS/Dataset/Training_pre".
    N (int, optional): N is the number of benchmarks to pick from the given path. Defaults to ten.

  Returns:
    Returns a list of N strings where each element is the path to a benchmark file.
  """
  path = os.path.join(_CROSS_VALIDATION_DIR, str(idx), "testset")
  random_list = []
  for i in range(pgm_num):
    random_list.append(("test" + str(i) + ".cc", os.path.join(path, "")))
  return random_list
                            
