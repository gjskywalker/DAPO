#include "HI_ASAPScheduling.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"

#include <ios>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>

using namespace llvm;

bool HI_ASAPScheduling::runOnModule(Module &M)
{
    *Evaluating_log << "==================Moduel Begin========================"
                    << "\n";
    *Evaluating_log << M;
    *Evaluating_log << "==================Module End========================"
                    << "\n";
    AnalyzeFunctions(M);

    analyzeTopFunction(M);

    return false;
}

bool HI_ASAPScheduling::CheckDependencyFesilility(Function &F)
{
    for (auto &B : F)
        for (auto &I : B)
            if (CallInst *CI = dyn_cast<CallInst>(&I))
            {
                if (FunctionLatency.find(CI->getCalledFunction()) == FunctionLatency.end())
                {
                    if (CI->getCalledFunction()->getName().find("llvm.") != std::string::npos || CI->getCalledFunction()->getName().find("HIPartitionMux") != std::string::npos)
                    {
                        timingBase tmp(0, 0, 1, clock_period);
                        FunctionLatency[CI->getCalledFunction()] = tmp;
                    }
                    else
                        return false;
                }
            }
    return true;
}

char HI_ASAPScheduling::ID = 0;

void HI_ASAPScheduling::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.setPreservesAll();
    AU.addRequired<AAResultsWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequiredTransitive<ScalarEvolutionWrapperPass>();
    // AU.addRequired<LoopAccessLegacyAnalysis>();
    AU.addRequired<DominatorTreeWrapperPass>();
    AU.addRequired<OptimizationRemarkEmitterWrapperPass>();
}

std::string HI_ASAPScheduling::demangleFunctionName(std::string mangled_name)
{
    std::string demangled_name;

    // demangle the function
    if (mangled_name.find("_Z") == std::string::npos)
        demangled_name = mangled_name;
    else
    {
        std::stringstream iss(mangled_name);
        iss.ignore(1, '_');
        iss.ignore(1, 'Z');
        int len;
        iss >> len;
        while (len--)
        {
            char tc;
            iss >> tc;
            demangled_name += tc;
        }
    }
    return demangled_name;
}

void HI_ASAPScheduling::AnalyzeFunctions(Module &M)
{
    bool all_processed = 0;
    while (!all_processed)
    {
        all_processed = 1;
        for (auto &F : M)
        {
            *Evaluating_log << "CHECKING FUNCTION " << F.getName() << "\n";
            Evaluating_log->flush();
            if (FunctionLatency.find(&F) != FunctionLatency.end())
            {
                continue;
            }
            else
            {
                if (F.getName().find("llvm.") != std::string::npos ||
                    F.getName().find("HIPartitionMux") != std::string::npos)
                {
                    timingBase tmp(0, 0, 1, clock_period);
                    FunctionLatency[&F] = tmp;
                    continue;
                }
                all_processed = 0;
                if (CheckDependencyFesilility(F))
                {
                    // LAA = &getAnalysis<LoopAccessLegacyAnalysis>(F);
                    LI = &getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
                    SE = &getAnalysis<ScalarEvolutionWrapperPass>(F).getSE();
                    getLoopBlockMap(&F);
                    analyzeFunction(&F);
                }
            }
        }
    }
}

void HI_ASAPScheduling::analyzeTopFunction(Module &M)
{
    *Evaluating_log << "\n analyze Top Function \n";
    int state_total_num = getTotalStateNum(M);
    for (auto &F : M)
    {
        std::string mangled_name = F.getName().str();
        std::string demangled_name;
        demangled_name = demangleFunctionName(mangled_name);
        mangled_name = "find function " + mangled_name + "and its demangled name is : " + demangled_name;
        if (F.getName().find("llvm.") == std::string::npos && F.getName().find("HIPartitionMux") == std::string::npos)
        {
            print_info(mangled_name.c_str());
        }
        if (demangled_name == top_function_name && F.getName().find(".") == std::string::npos)
        {
            *Evaluating_log << "Top Function: " << F.getName() << " is found";
            topFunctionFound = 1;
            top_function_latency = analyzeFunction(&F).latency;
            std::string printOut("");
            // print out the information of top function in terminal
            printOut =
                "Done latency evaluation of top function: [" + demangled_name +
                "] and its latency is " + std::to_string(top_function_latency) +
                " the state num is: " + std::to_string(state_total_num);
            *Evaluating_log << printOut << "\n";
            print_info(printOut);
        }
    }
}

int HI_ASAPScheduling::getTotalStateNum(Module &M)
{
    *Evaluating_log << "================================\n              printing schedule "
                       "\n==================================\n";
    for (auto it : Inst_Schedule)
    {
        *Evaluating_log << "inst: [" << *it.first << "] in Block: [" << it.second.first->getName()
                        << "] #cycle: [" << it.second.second << "]\n";
    }
    Evaluating_log->flush();
    *Evaluating_log << "================================\n              counting stage num "
                       "\n==================================\n";

    int state_total = 0;
    for (auto &F : M)
    {
        if (F.getName().find("llvm.") != std::string::npos ||
            F.getName().find("HIPartitionMux") !=
                std::string::npos) // bypass the "llvm.xxx" functions..
            continue;
        BasicBlock *Func_Entry = &(F.getEntryBlock()); // get the entry of the function
        timingBase origin_path_in_F(0, 0, 1, clock_period);
        tmp_BlockCriticalPath_inFunc.clear(); // record the block level critical path in the loop
        tmp_LoopCriticalPath_inFunc
            .clear(); // record the critical path to the end of sub-loops in the loop
        Func_BlockVisited.clear();
        state_total += getFunctionStageNum(origin_path_in_F, &F, Func_Entry);
    }
    return state_total + 2; // TODO: check +2 is for function or module (reset/idle)
}