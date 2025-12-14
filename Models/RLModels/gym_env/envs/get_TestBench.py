import os  
from os.path import isfile, join
from shutil import copyfile

def lsFiles(path, with_dir=True):
    """ 
    Examples :
        >>> print(lsFiles(path, False))
        [file1.txt, file2.txt, file3.txt]

        >>> print(lsFiles(path))
        [path/file1.txt, path/file2.txt, path/file3.txt]

    Args:
        path (str): The path of the directory we are interested in.
        with_dir (bool, optional): with_dir should be set to True if you want the returned list (files) to contain the files’ names from the given directory 
        	(the parameter path) concatenated with the given path (the parameter path), or with_dir should be set to False if you want the returned list (files) 
        	to only contain the files’ names .Defaults to True.

    Returns:
 		Returns a list of strings where each element is the name of a file in the given path (case when with_dir is False), or a list of strings where each 
 		element is the given path concatenated with a file name (case when with_dir is True).

    """

    path = os.path.abspath(path)
    files = [f for f in os.listdir(path) if isfile(join(path, f))]
    if with_dir:
      files = [join(path, f) for f in os.listdir(path) if isfile(join(path, f))]
    return files

def get_random(idx : int, pgm_num : int) -> list : 
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
  path="/home/eeuser/Desktop/GRL-HLS/Dataset/Cross_Validation/" + str(idx) + "/trainingset"
  random_list = []
  for i in range(0, pgm_num):
    random_list.append(("random" + str(i) + ".cc", path+"/"))
  return random_list 

def get_random_all() -> list:
  """
  Returns a list of all random files in the cross-validation training set directories.
  
  Returns:
    list: A list of tuples where each tuple contains a file name and its corresponding path.
  """
  path = "/home/eeuser/Desktop/GRL-HLS/Dataset/trainingset"
  random_list = []
  for i in range(0, 80):
    random_list.append(("random" + str(i) + ".cc", path+"/"))
  return random_list

def get_polybench(path = "/home/eeuser/Desktop/GRL-HLS/Dataset/polybench/", N=16, use_dir=True):
  """
  Returns a list of polybench benchmark files and their paths.
  
  Args:
    path (str, optional): Path to polybench directory. Defaults to "/home/eeuser/Desktop/GRL-HLS/Dataset/polybench/".
    N (int, optional): Number of benchmarks to select. Defaults to 16.
    use_dir (bool, optional): If True, returns path as string; if False, returns path as list. Defaults to True.

  Returns:
    list: List of tuples (filename, path) for polybench benchmarks.
  """
  polybench = [
    ("2mm","2mm"), 
    ("3mm","3mm"),
    ("adi","adi"), 
    ("atax","atax"),
    ("bicg","bicg"),  
    ("covariance","covariance"), 
    ("doitgen","doitgen"), 
    ("fdtd-2d","fdtd-2d"), 
    ("gemm","gemm"), 
    ("gemver","gemver"), 
    ("gesummv","gesummv"), 
    ("heat-3d", "heat-3d"),
    ("jacobi-1d","jacobi-1d"), 
    ("jacobi-2d", "jacobi-2d"), 
    ("mvt","mvt"), 
    ("seidel-2d","seidel-2d"), 
  ]
  polybench_list = []
  for key, value in polybench:
    if use_dir:
      polybench_list.append((value+".cc", path+key+"/"))
    else:
      files = lsFiles(path+key)
      polybench_list.append((value+".cc", files))
  
  return polybench_list[:N]

def get_test(idx : int, pgm_num : int) -> list : 
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
  path="/home/eeuser/Desktop/GRL-HLS/Dataset/Cross_Validation/" + str(idx) + "/testset"
  random_list = []
  for i in range(0, pgm_num):
    random_list.append(("test" + str(i) + ".cc", path+"/"))
  return random_list 
                            
def get_chstone(path = "/home/eeuser/Desktop/GRL-HLS/Dataset/chstone", N=12, use_dir=True):
  """
  Examples :
    >>> print(get_chstone())
    [("adpcm.c","path/adpcm/"), ("aes.c","path/aes/"), ("bf.c","path/blowfish/"), ("dfadd.c","path/dfadd/"),("dfdiv.c","path/dfdiv/"), 
    ("dfmul.c","path/dfmul/"), ("dfsin.c","path/dfsin/"), ("gsm.c","path/gsm/"), ("main.c","path/jpeg/"), ("mips.c","path/mips/"), 
    ("mpeg2.c","path/motion/"), ("sha_driver.c","path/sha/")]   
    >>> print(get_random(“path/to/dir/”))
    [("adpcm.c","path/to/dir/adpcm/"), ("aes.c","path/to/dir/aes/"), ("bf.c","path/to/dir/blowfish/"), ("dfadd.c","path/to/dir/dfadd/"), 
    ("dfdiv.c","path/to/dir/dfdiv/"),  ("dfmul.c","path/to/dir dfmul/"), ("dfsin.c","path/to/dir/dfsin/"), ("gsm.c","path/to/dir/gsm/"), 
    ("main.c","path/to/dir/jpeg/"), ("mips.c","path/to/dir/mips/"), ("mpeg2.c","path/to/dir/motion/"), ("sha_driver.c","path/to/dir/sha/")]   

    >>> print(get_random(“path/to/directory”, 6))
    [("adpcm.c","path/to/dir/adpcm/"), ("aes.c","path/to/dir/aes/"), ("bf.c","path/to/dir/blowfish/"), ("dfadd.c","path/to/dir/dfadd/"), 
    ("dfdiv.c","path/to/dir/dfdiv/"),  ("dfmul.c","path/to/dir dfmul/")]

    >>> print(get_random(“path/to/directory”, 6, False))
    [("adpcm.c",["path/to/dir/adpcm/"]), ("aes.c",["path/to/dir/aes/"]), ("bf.c",["path/to/dir/blowfish/"]), ("dfadd.c",["path/to/dir/dfadd/"]), 
    ("dfdiv.c",["path/to/dir/dfdiv/"]),  ("dfmul.c",["path/to/dir dfmul/"])]

  Args:
    path (str, optional): Path to the chstone_path directory that contains chstone benchmarks. Defaults to chstone_path.
    N (int, optional): N is the number of benchmarks to select from the chstone list. Defaults to twelve.
    use_dir(bool, optional): use_dir should be set to True if you want the tuple path (given path + “benchmark_name”) in the returned list (chstone_list) 
      to as a string, or use_dir should be set to False if you want the tuple path (given path + “benchmark_name”) in the returned list (chstone_list) to 
      be as a string in a list. Defaults to True.

  Returns:
      Returns a list of tuples where each tuple(“string”, [“string”] ) contains as the first element a file written in the C programming language(chstone_benchmark_name.c from the chstone list) and as the second element a list that contains the same chstone benchmark name (case when use_dir is False). However, for the case when use_dir is True, this function returns a list of tuples where each tuple(“string”, “string”) contains as the first element a file written in the C programming language (chstone_benchmark_name.c from the chstone list)  and as the second element the same chstone benchmark name concatenated with the given path.
  """

  chstone = [
 ( "adpcm","adpcm"),
 ( "aes","aes"),
 ( "bf","bf"),
 ( "dfadd","dfadd"),
 ( "dfdiv","dfdiv"),
 ( "dfmul","dfmul"),
 ( "dfsin","dfsin"),
 ( "gsm","gsm"),
 ( "jpeg","jpeg"),
 ( "mips","mips"),
 ( "motion","mpeg2"),
 ( "sha","sha")
  ]

  chstone_list = []
  for key, value in chstone:
    if use_dir: 
      chstone_list.append((value+".cc", path+key+"/"))
    else: 
      files = lsFiles(path+key) 
      chstone_list.append((value+".cc", files))
  return chstone_list[:N]

