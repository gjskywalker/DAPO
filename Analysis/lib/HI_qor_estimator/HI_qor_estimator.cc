#include "HI_qor_estimator.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include <ios>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

bool HI_qor_estimator::runOnModule(
    Module &M) // The runOnFunction declaration will overide the virtual one in ModulePass, which
               // will be executed for each Function.
{
    
    *Evaluating_log << " ======================= the module begin =======================\n";
    *Evaluating_log << M;
    *Evaluating_log << " ======================= the module end =======================\n";

    TraceMemoryDeclarationinModule(M);

    AnalyzeFunctions(M);

    analyzeTopFunction(M);

    getSchedulingResult();

    getRAMResult();

    *Evaluating_log << " =======================  the information of FF_Assign ====================\n";
    for (auto *it : Instruction_FFAssigned)
    {
        *Evaluating_log << *it << "\n";
    }

    return false;
}

void HI_qor_estimator::getSchedulingResult()
{
    for (auto &it : BlockLatency)
    {
        *Scheduling_log << it.first->getName().str() << "$" << it.second << "\n";
    }
    for (auto &ii : InstructionCP)
    {
        *Scheduling_log << *(ii.first) << "$" << ii.second << "\n";
    }
}

void HI_qor_estimator::getRAMResult()
{
    for (auto it : Access2TargetMap)
    {
        *Ram_log << *it.first << "$";
        for (auto tmpI : it.second)
        {
            *Ram_log << " {" << tmpI->getName() << "} ";
        }
        *Ram_log << "\n";
    }
}

bool HI_qor_estimator::CheckDependencyFesilility(Function &F)
{
    for (auto &B : F)
        for (auto &I : B)
            if (CallInst *CI = dyn_cast<CallInst>(&I))
            {
                if (FunctionLatency.find(CI->getCalledFunction()) == FunctionLatency.end())
                {
                    /*
                        Actually this is very unaccurate in new llvm IR, since there will be case like:
                        %112 = call double @llvm.fmuladd.f64(double %neg84, double 1.000000e-02, double %111)
                    */
                    if (CI->getCalledFunction()->getName().find("llvm.") != std::string::npos ||
                        CI->getCalledFunction()->getName().find("HIPartitionMux") !=
                            std::string::npos || F.getName().find("sqrtf") != std::string::npos)
                    {
                        timingBase tmp(0, 0, 1, clock_period);
                        FunctionLatency[CI->getCalledFunction()] = tmp;
                        return false;
                    }
                    else
                        return false;
                }
            }
    return true;
}

char HI_qor_estimator::ID =
    0; // the ID for pass should be initialized but the value does not matter, since LLVM uses the
       // address of this variable as label instead of its value.

// introduce the dependence of Pass
void HI_qor_estimator::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.setPreservesAll();
    AU.addRequired<AAResultsWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequiredTransitive<ScalarEvolutionWrapperPass>();

    // AU.addRequired<ScalarEvolutionWrapperPass>();

    // AU.addRequired<LoopAccessAnalysis>();
    AU.addRequired<DominatorTreeWrapperPass>();
    // AU.addPreserved<DominatorTreeWrapperPass>();
    AU.addRequired<OptimizationRemarkEmitterWrapperPass>();
    // AU.addRequiredTransitive<polly::DependenceInfoWrapperPass>();
    // AU.addRequiredTransitive<polly::ScopInfoWrapperPass>();
    // AU.addRequired<polly::PolyhedralInfo>();
    // AU.addPreserved<GlobalsAAWrapperPass>();
}

std::string HI_qor_estimator::demangleFunctionName(std::string mangled_name)
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

void HI_qor_estimator::AnalyzeFunctions(Module &M)
{
    bool all_processed = 0;
    while (!all_processed)
    {
        all_processed = 1;
        for (auto &F : M)
        {
            *Evaluating_log << "CHECKING FUNCTION " << F.getName() << "\n";
            Evaluating_log->flush();
            for(auto it = F.arg_begin(), ie = F.arg_end(); it != ie; ++it)
                funargs.insert(&(*it));
            if (FunctionLatency.find(&F) != FunctionLatency.end())
            {
                continue;
            }
            else
            {
                if (F.getName().find("llvm.") != std::string::npos ||
                    F.getName().find("HIPartitionMux") != std::string::npos || F.getName().find("sqrtf") != std::string::npos)
                {
                    timingBase tmp(0, 0, 1, clock_period);
                    FunctionLatency[&F] = tmp;
                    continue;
                }
                all_processed = 0;
                if (CheckDependencyFesilility(F))
                {
                        // std::cout << F.getName().str() << std::endl;
                        LI = &getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
                        SE = &getAnalysis<ScalarEvolutionWrapperPass>(F).getSE();
                        getLoopBlockMap(&F);
                        analyzeFunction(&F);
                }
            }
        }
    }
}

void HI_qor_estimator::analyzeTopFunction(Module &M)
{
    *Evaluating_log << "======================================\n                    analyze Top "
                       "Function \n======================================\n";
    int state_total_num = getTotalStateNum(M);
    int LUT_needed_by_FSM = LUT_for_FSM(state_total_num);
    int FF_needed_by_FSM = state_total_num;
    for (auto &F : M)
    {
        std::string mangled_name = F.getName().str();
        std::string demangled_name;
        demangled_name = demangleFunctionName(mangled_name);
        mangled_name =
            "find function " + mangled_name + "and its demangled name is : " + demangled_name;
        if (F.getName().find(".") == std::string::npos &&
            F.getName().find("HIPartitionMux") == std::string::npos)
            print_info(mangled_name.c_str());
        if (demangled_name == top_function_name && F.getName().find(".") == std::string::npos)
        {
            *Evaluating_log << "Top Function: " << F.getName() << " is found";
            topFunctionFound = 1;
            top_function_latency = analyzeFunction(&F).latency;

            std::string printOut("");
            FunctionResource[&F] = FunctionResource[&F] + BRAM_MUX_Evaluate();
            // print out the information of top function in terminal
            printOut =
                "Done latency evaluation of top function: [" + demangled_name +
                "] and its latency is " + std::to_string(top_function_latency) +
                validorinvalid(valid) + 
                " the state num is: " + std::to_string(state_total_num) +
                " and its resource cost is [DSP=" + std::to_string(FunctionResource[&F].DSP) +
                ", FF=" + std::to_string(FunctionResource[&F].FF + FF_needed_by_FSM) +
                ", LUT=" + std::to_string(FunctionResource[&F].LUT + LUT_needed_by_FSM) +
                ", BRAM=" + std::to_string(FunctionResource[&F].BRAM) + "]";
            *Cycle_Result << std::to_string(top_function_latency) << "\n";
            *Evaluating_log << printOut << "\n";
            
            print_info("Done latency evaluation of top function: [" + demangled_name +
                "] and its latency is " + std::to_string(top_function_latency) +
                validorinvalid(valid) + 
                " the state num is: " + std::to_string(state_total_num));
        }
    }
}

void HI_qor_estimator::TraceMemoryDeclarationinModule(Module &M)
{
    for (auto &it : M.global_values())
    {
        if (auto GV = dyn_cast<GlobalVariable>(&it))
        {
            *Evaluating_log << it << " is a global variable\n";
            TraceAccessForTarget(&it, &it);
        }
    }
    for (auto &F : M)
    {
        if (F.getName().find("llvm.") != std::string::npos ||
            F.getName().find("HIPartitionMux") !=
                std::string::npos || F.getName().find("sqrt") != std::string::npos) // bypass the "llvm.xxx" functions..
            continue;
        std::string mangled_name = F.getName().str();
        std::string demangled_name;
        demangled_name = demangleFunctionName(mangled_name);
        findMemoryDeclarationin(&F, demangled_name == top_function_name &&
                                        F.getName().find(".") == std::string::npos);
    }
    for (auto &F : M)
    {
        if (F.getName().find("llvm.") != std::string::npos ||
            F.getName().find("HIPartitionMux") !=
                std::string::npos) // bypass the "llvm.xxx" functions..
            continue;
        // Alias Analysis is a class of techniques which attempt to determine whether or not
        // two pointers ever can point to the same object in memory.
        // AliasAnalysis &AA = getAnalysis<AAResultsWrapperPass>(F).getAAResults();
    }
}

int HI_qor_estimator::getTotalStateNum(Module &M)
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
                std::string::npos || F.getName().find("sqrt") != std::string::npos) // bypass the "llvm.xxx" functions..
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

// I don't think this is a proper way to caculate LUT for FSM
int HI_qor_estimator::LUT_for_FSM(int stateNum)
{
    double x = stateNum;
    double y = -0.44444444444444475 * x * x + 10.555555555555559 * x - 14.11111111111112;
    return round(y);
}

std::string HI_qor_estimator::validorinvalid(bool valid)
{
    if (valid)
    return " (valid result)";
    else
    return " (invalid result loop trip count unreadable)";
}
