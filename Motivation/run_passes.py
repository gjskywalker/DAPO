#!/usr/bin/env python3
"""
LLVM Pass Runner Script
Author: Generated script for running LLVM optimization passes
Date: July 2025

This script takes a C++ source file and a list of LLVM passes,
then applies each pass sequentially using LLVM opt tool,
generating intermediate IR files for analysis.
"""

import os
import sys
import subprocess
import argparse
from pathlib import Path
from typing import List, Union, Optional

class LLVMPassRunner:
    def __init__(self, cpp_file: Union[str, Path], output_dir: str = "./ir_outputs") -> None:
        self.cpp_file = Path(cpp_file)
        self.output_dir = Path(output_dir)
        self.base_name = self.cpp_file.stem
        
        # Create output directory
        self.output_dir.mkdir(exist_ok=True)
        
        # LLVM tool paths (adjust if needed)
        self.clang = "clang++"
        self.opt = "opt"
        
    def compile_to_ir(self) -> Path:
        """Compile C++ source to initial LLVM IR"""
        initial_ir = self.output_dir / f"{self.base_name}_initial.ll"
        
        cmd = [
            self.clang,
            "-O0",
            "-Xclang",
            "-disable-O0-optnone",
            "-emit-llvm",           # Emit LLVM IR instead of native assembly
            "-S",           
            str(self.cpp_file),
            "-o", str(initial_ir)
        ]
        
        print(f"🔨 Compiling {self.cpp_file} to initial IR...")
        print(f"Command: {' '.join(cmd)}")
        
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, check=True)
            print(f"✅ Initial IR generated: {initial_ir}")
            return initial_ir
        except subprocess.CalledProcessError as e:
            print(f"❌ Compilation failed:")
            print(f"Error: {e.stderr}")
            sys.exit(1)
    
    def run_single_pass(self, input_ir: Path, pass_name: str, output_ir: Path) -> bool:
        """Run a single LLVM pass"""
        cmd = [
            self.opt,
            f"-passes={pass_name}",        # Pass name with dash prefix
            "-S",                   # Output human-readable IR
            str(input_ir),
            "-o", str(output_ir)
        ]
        
        print(f"🔄 Running pass: {pass_name}")
        print(f"   Input:  {input_ir}")
        print(f"   Output: {output_ir}")
        
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, check=True)
            print(f"✅ Pass {pass_name} completed successfully")
            return True
        except subprocess.CalledProcessError as e:
            print(f"❌ Pass {pass_name} failed:")
            print(f"Error: {e.stderr}")
            return False
    
    def run_passes(self, passes: Union[str, List[str]]) -> Path:
        """Run a sequence of LLVM passes"""
        if isinstance(passes, str):
            passes = [p.strip() for p in passes.split(',')]
        
        print(f"🚀 Starting LLVM pass sequence for {self.cpp_file}")
        print(f"Passes to run: {passes}")
        print("=" * 60)
        
        # Step 1: Compile to initial IR
        current_ir = self.compile_to_ir()
        
        # Step 2: Apply each pass sequentially
        for i, pass_name in enumerate(passes):
            # Generate output filename
            output_ir = self.output_dir / f"{self.base_name}_{i}_after_{pass_name.replace('-', '_')}.ll"
            
            # Run the pass
            success = self.run_single_pass(current_ir, pass_name, output_ir)
            
            if success:
                current_ir = output_ir  # Use output as input for next pass
                print(f"📁 IR saved: {output_ir}")
            else:
                print(f"⚠️  Stopping at pass {pass_name} due to error")
                break
            
            print("-" * 40)
        
        print("🎉 Pass sequence completed!")
        print(f"📂 All IR files saved in: {self.output_dir}")
        return current_ir
    
    def generate_summary(self, passes: List[str]) -> None:
        """Generate a summary of all generated IR files"""
        summary_file = self.output_dir / f"{self.base_name}_summary.txt"
        
        with open(summary_file, 'w') as f:
            f.write(f"LLVM Pass Summary for {self.cpp_file}\n")
            f.write("=" * 50 + "\n\n")
            f.write(f"Generated IR files:\n")
            
            # List all .ll files in output directory
            ir_files = sorted(self.output_dir.glob("*.ll"))
            for ir_file in ir_files:
                f.write(f"  - {ir_file.name}\n")
            
            f.write(f"\nPasses applied: {', '.join(passes)}\n")
            f.write(f"Total files generated: {len(ir_files)}\n")
        
        print(f"📋 Summary saved: {summary_file}")

def main() -> None:
    parser = argparse.ArgumentParser(description="Run LLVM optimization passes on C++ source")
    parser.add_argument("cpp_file", help="Input C++ source file")
    parser.add_argument("passes", help="Comma-separated list of LLVM passes (e.g., 'mem2reg,instcombine,loop-unroll')")
    parser.add_argument("-o", "--output", default="./ir_outputs", help="Output directory for IR files")
    parser.add_argument("--summary", action="store_true", help="Generate summary file")
    
    args = parser.parse_args()
    
    # Check if input file exists
    if not Path(args.cpp_file).exists():
        print(f"❌ Error: File {args.cpp_file} not found")
        sys.exit(1)
    
    # Create runner and execute passes
    runner = LLVMPassRunner(args.cpp_file, args.output)
    passes_list = [p.strip() for p in args.passes.split(',')]
    
    final_ir = runner.run_passes(passes_list)
    
    if args.summary:
        runner.generate_summary(passes_list)
    
    print(f"\n🏁 Final optimized IR: {final_ir}")

if __name__ == "__main__":
    # Example usage when run directly
    if len(sys.argv) == 1:
        print("Example usage:")
        print("python run_passes.py source.cpp 'mem2reg,instcombine,loop-unroll'")
        print("python run_passes.py source.cpp 'mem2reg,sroa,instcombine' -o ./output --summary")
        print("\nCommon LLVM passes:")
        print("  - mem2reg: Promote memory to register")
        print("  - instcombine: Instruction combining")
        print("  - loop-unroll: Loop unrolling")
        print("  - sroa: Scalar replacement of aggregates")
        print("  - dce: Dead code elimination")
        print("  - licm: Loop invariant code motion")
        print("  - loop-vectorize: Loop vectorization")
        sys.exit(0)
    
    main()
