import subprocess
import os

def run_llvm_optimizer(input_file, passes, keep_ir=False, output_prefix=None, clang_path=None, opt_path=None):
    """
    Run LLVM optimizer on the specified input file with given passes.
    
    Args:
        input_file (str): Path to the input file to optimize.
        passes (str): Comma-separated list of optimization passes.
        output_prefix (str): Prefix for output files.
        clang_path (str): Path to the clang executable.
        opt_path (str): Path to the opt executable.
    
    Returns:
        bool: True if optimization was successful, False otherwise.
    """
    if not os.path.exists(input_file):
        print(f"Input file {input_file} does not exist!")
        return False
    
    # Prepare command
    command = []
    if clang_path:
        command.append(clang_path)
    else:
        command.append("clang")
        command.append("-S")
        command.append("-emit-llvm")
        command.append("-O0")
        command.append("-Xclang")
        command.append("-disable-O0-optnone")
    
    command.append(input_file)
    
    if output_prefix:
        command.append("-o")
        command.append(f"{output_prefix}.bc")
    
    # Run the optimizer
    try:
        result = subprocess.run(command, check=True, capture_output=True, text=True)
        print(f"C++2IR completed successfully: {result.stdout.strip()}")
        
        # Split passes and apply them one by one
        pass_list = [p.strip() for p in passes.split(',')]
        current_input = f"{output_prefix}.bc"
        
        for i, pass_name in enumerate(pass_list):
            print(f"Applying pass {i+1}/{len(pass_list)}: {pass_name}")
            
            # Determine output file name
            if i == len(pass_list) - 1:  # Last pass
                current_output = "top.ll"
            else:
                current_output = f"{output_prefix}_temp_{i}.bc"
            
            try:
                result = subprocess.run(
                    [opt_path or "opt", "-S", f"-passes={pass_name}", current_input, "-o", current_output],
                    check=True,
                    capture_output=True,
                    text=True
                )
                print(f"  ✓ Pass '{pass_name}' completed successfully")
                
                # Update input for next pass
                current_input = current_output
                
            except subprocess.CalledProcessError as e:
                print(f"  ✗ Error during pass '{pass_name}': {e.stderr.strip()}")
                return False
        
        print(f"All optimization passes completed successfully!")
        
        if not keep_ir:
            # Clean up temporary files
            for i in range(len(pass_list) - 1):
                temp_file = f"{output_prefix}_temp_{i}.bc"
                if os.path.exists(temp_file):
                    os.remove(temp_file)
        
        return True
        
    except subprocess.CalledProcessError as e:
        print(f"Error running clang: {e.stderr.strip()}")
        return False
    
if __name__ == "__main__":
    run_llvm_optimizer(
        input_file="vecNormTrans.cc",
        passes="gvn,dse,loop-rotate,jump-threading,loop-unroll,speculative-execution,simplifycfg",
        output_prefix="original")