#include "HI_IR2Verilog.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <ios>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

bool HI_IR2Verilog::runOnModule(Module &M)
{
    // Array_Process(M);
    ParseSchedulingfromFile();
    ParseRAMfromFile();
    GenerateRAM(M);
    argument(M);
    stateslabel(M);
    variable(M);
    statesbody(M);
    // for (auto &it : AccessMap)
    // {
    //     std::cout << it.first << " accesses " << it.second << "\n";
    // }
    // for (Function &F : M)
    // {
    //     // NaiveBinding(F);
    //     // argument(F);
    //     // stateslabel(F);
    //     *Verilogfile << F.getName() << "\n";
    //     *Verilogfile << "Debug Information"
    //                  << "\n";
    //     // variable(F);
    //     //  statesbody(F);
    // }
    // for (auto tmp : BindingVariables)
    // {
    //     *Verilogfile << "The Instruction is " << *tmp << " It's in BasicBlock " << tmp->getParent()->getName() << "\n";
    //     for
    //     // *Verilogfile << "The results of the instruction is : "
    //     //              << "\n";
    //     // tmp->printAsOperand(*Verilogfile);
    //     // *Verilogfile << "\n";
    //     // for (int i = 0; i < tmp->getNumOperands(); ++i)
    //     // {
    //     //     *Verilogfile << "The operands of the instruction is : "
    //     //                  << "\n";
    //     //     tmp->getOperand(i)->printAsOperand(*Verilogfile);
    //     //     *Verilogfile << "\n";
    //     // }
    // }
    //
    // for (Function &F : M)
    //     for (BasicBlock &B : F)
    //     {
    //         *Verilogfile << B.getName().str() << " latency : " << SchedulingInfor[removeExtraSpace(B.getName().str())].latency << " timing : " << SchedulingInfor[removeExtraSpace(B.getName().str())].timing << "\n";
    //         for (Instruction &I : B)
    //         {
    //             *Verilogfile << getStringInstruction(I) << " latency : " << SchedulingInfor[removeExtraSpace(getStringInstruction(I))].latency << " timing : " << SchedulingInfor[removeExtraSpace(getStringInstruction(I))].timing << "\n";
    //         }
    //     }
    // *Verilogfile << "========================="
    //              << "\n";
    // for (Function &F : M)
    //     for (BasicBlock &B : F)
    //         for (Instruction &I : B)
    //         {
    //             for (User *U : I.users())
    //             {
    //                 *Verilogfile << "Instruction : " << I << " And its user : " << *U << "\n";
    //             }
    //             // for (User::op_iterator I_begin = I.op_begin(), I_end = I.op_end(); I_begin != I_end; ++I_begin)
    //             // {
    //             //     *Verilogfile << "Instruction : " << I << " And its user : " << **I_begin << "\n";
    //             // }
    //         }
    return false;
}

char HI_IR2Verilog::ID =
    0; // the ID for pass should be initialized but the value does not matter, since LLVM uses the
       // address of this variable as label instead of its value.

// introduce the dependence of Pass
void HI_IR2Verilog::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.setPreservesAll();
}
