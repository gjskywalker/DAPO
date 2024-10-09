#include "HI_Test.h"
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

bool HI_Test::runOnModule(Module &M)
{
    int Bitwidth;
    for (Function &F : M)
        for (BasicBlock &B : F)
        {
            for (Instruction &I : B)
            {
                if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(i)))
                        {
                            continue;
                        }
                        else
                        {
                            auto l_Pred = dyn_cast<Instruction>(I.getOperand(i));
                            Bitwidth = getOriginalBitwidth(l_Pred);
                        }
                    }
                    *DebugInfo << "Opcode: " << I.getOpcodeName() << ", "
									<< "Datatype: " << Bitwidth;
                }
            }
        }

    return false;
}

int HI_Test::getOriginalBitwidth(Value *Val)
{
    if (Instruction *I = dyn_cast<Instruction>(Val))
    {
        if (I->getOpcode() == Instruction::Trunc || I->getOpcode() == Instruction::SExt ||
            I->getOpcode() == Instruction::ZExt)
            return getOriginalBitwidth((I->getOperand(0)));
        else
            return I->getType()->getIntegerBitWidth();
    }

    else
        return Val->getType()->getIntegerBitWidth();
}

char HI_Test::ID = 0;

void HI_Test::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.setPreservesAll();
    AU.addRequired<AAResultsWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequiredTransitive<ScalarEvolutionWrapperPass>();

    // AU.addRequired<ScalarEvolutionWrapperPass>();
    // AU.addRequired<LoopInfoWrapperPass>();
    // AU.addPreserved<LoopInfoWrapperPass>();

    // AU.addRequired<LoopAccessLegacyAnalysis>();
    AU.addRequired<DominatorTreeWrapperPass>();
    // AU.addPreserved<DominatorTreeWrapperPass>();
    AU.addRequired<OptimizationRemarkEmitterWrapperPass>();
}