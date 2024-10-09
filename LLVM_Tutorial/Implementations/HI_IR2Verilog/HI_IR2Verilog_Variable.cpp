#include "HI_IR2Verilog.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

void HI_IR2Verilog::variable(llvm::Module &M)
{
    int count = 0;
    int namecode = 0;
    std::string InstructionNameTemple = "UNNAME";
    for (Function &F : M)
        for (BasicBlock &B : F)
            for (Instruction &I : B)
            {
                count++;
                Instruction *Iptr = &I;
                if ((Iptr->getOpcode() == Instruction::Store) ||
                    (Iptr->getOpcode() == Instruction::Br) || (Iptr->getOpcode() == Instruction::Ret) || (Iptr->getOpcode() == Instruction ::Call) || (Iptr->getOpcode() == Instruction::Alloca))
                    continue;
                else
                {
                    if (Iptr->getName().str() == "")
                    {
                        std::string InstructionName = InstructionNameTemple + std::to_string(namecode);
                        VariablesName.insert(std::pair<llvm::Instruction *, std::string>(Iptr, InstructionName));
                        // VariablesName[InstructionName] = Iptr;
                        namecode++;
                    }
                    else
                    {
                        VariablesName.insert(std::pair<llvm::Instruction *, std::string>(Iptr, Standard_Name(Iptr->getName().str())));
                        // VariablesName[Standard_Name(Iptr->getName().str())] = Iptr;
                    }
                }
            }
    //*Verilogfile << count << "\n";
    for (auto it : VariablesName)
    {
        *Verilogfile << "   reg    "
                     << "[" << varwidth(it.first) << ":0]    " << it.second << ";"
                     << "\n";
    }
    // map<string, instruction/string>
    // *Verilogfile << "//define the variables"
    //              << "\n";
    // for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    // {
    //     Instruction *Iptr = &(*I);
    //     for (int i = 0; i < I->getNumOperands(); ++i)
    //     {
    //         if (Argument *arg = dyn_cast<Argument>(I->getOperand(i)))
    //             continue;
    //         if (Constant *InstTmp = dyn_cast<Constant>(I->getOperand(i)))
    //             continue;
    //         if (I->getOperand(i)->getType()->isLabelTy())
    //             continue;
    //         if (variableset.find(Standard_Name((*I->getOperand(i)).getName().str())) == variableset.end())
    //         {
    //             variableset.insert(Standard_Name((*I->getOperand(i)).getName().str()));
    //             *Verilogfile << "   reg    "
    //                          << "[" << varwidth(Iptr) << ":0]    " << Standard_Name((*I->getOperand(i)).getName().str()) << ";"
    //                          << "\n";
    //         }
    //     }
    // }
}

int HI_IR2Verilog::varwidth(Instruction *I)
{
    int bitwidth;
    if (ICmpInst *ICI = dyn_cast<ICmpInst>(I))
        return 0;
    if (I->getType()->isIntegerTy())
    {
        bitwidth = I->getType()->getIntegerBitWidth() - 1;
        return bitwidth;
    }
    else if (I->getType()->isFloatTy())
    {
        print_error("Unsupported variable type float\n");
        assert(0);
        return -1;
    }
    else if (I->getType()->isDoubleTy())
    {
        print_error("Unsupported variable type double\n");
        assert(0);
        return -1;
    }
}

/*
    The main idea of this kind of binding is to analysis the user of each instruction,
    and binding the instruction whose user is all within the same basicblock in different
    basicblocks.
*/
void HI_IR2Verilog::NaiveBinding(Function &F)
{
    for (BasicBlock &B : F)
    {
        BasicBlock *B_ptr = &B;
        for (Instruction &I : B)
        {
            Instruction *I_ptr = &I;
            if (JudgeVariable(B_ptr, I_ptr))
            {
                if (BranchInst *BI = dyn_cast<BranchInst>(&I))
                {
                    continue;
                }
                else if (CallInst *CT = dyn_cast<CallInst>(&I))
                {
                    continue;
                }
                else if (ReturnInst *RI = dyn_cast<ReturnInst>(&I))
                {
                    continue;
                }
                else
                {
                    BindingVariables.insert(I_ptr);
                }
            }
        }
    }
}

bool HI_IR2Verilog::BlockContain(BasicBlock *B, Instruction *I)
{
    //*Verilogfile << "BasicBlock : " << *B << " I->getParent() = " << *(I->getParent());
    return I->getParent() == B;
}
bool HI_IR2Verilog::JudgeVariable(BasicBlock *B, Instruction *I)
{
    int countNum = 0;
    int JudgeNum = 0;
    for (User *it : I->users())
    {
        countNum++;
    }
    for (User *it : I->users())
    {
        if (auto I_User = dyn_cast<Instruction>(it))
        {
            if (!(BlockContain(B, I_User)))
            {
                break;
            }
        }
        JudgeNum++;
    }
    //*Verilogfile << *I << " countNum = " << countNum << " JudgeNum = " << JudgeNum << "\n";
    return JudgeNum == countNum;
}
