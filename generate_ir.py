import os
import subprocess

def generate_ir(source_file : str, output_file : str):
    command = [
        "clang",
        "-O1",
        "-emit-llvm",
        "-S",
        source_file,
        "-o",  
        output_file,
    ]
    subprocess.run(command, check=True)

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
        description="Generate LLVM IR from source code files"
    )

    parser.add_argument('-s','--Source', required=True, type=str)
    parser.add_argument('-o','--Output', required=True, type=str)
    args = parser.parse_args()
    if not os.path.isdir(args.Output):
        os.makedirs(args.Output, exist_ok=True)
    for j in os.listdir(args.Source):
        source_file = os.path.join(args.Source, j)
        output_file = os.path.join(args.Output, j.split('.')[0] + '.ll')
        generate_ir(source_file, output_file)