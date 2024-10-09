#ifndef HI_TEST
#define HI_TEST

#include "HI_StringProcess.h"
#include "HI_print.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AliasSetTracker.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/LoopVersioning.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include <algorithm>
#include <bits/stl_map.h>
#include <cctype>
#include <fstream>
#include <ios>
#include <iostream>
#include <set>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>

using namespace llvm;

class HI_Test : public ModulePass
{
public:
    std::error_code ErrInfo;
    raw_ostream *DebugInfo;

    HI_Test() : ModulePass(ID)
    {
        DebugInfo = new raw_fd_ostream("DebugInfo", ErrInfo, sys::fs::OF_None);
    }
    ~HI_Test()
    {
        DebugInfo->flush();
        delete DebugInfo;
    }
    void getAnalysisUsage(AnalysisUsage &AU) const;
    bool runOnModule(llvm::Module &M);
    virtual bool doInitialization(llvm::Module &M)
    {
        print_status("Initializing HI_Test pass.");
        return false;
    }
    int getOriginalBitwidth(Value *Val);

    static char ID;

    // Timer
    struct timeval tv_begin;
    struct timeval tv_end;
};
#endif