#ifndef _HI_ASAPScheduling
#define _HI_ASAPScheduling

#define AggressiveSchedule

#ifdef AggressiveSchedule
#define LoadStore_Thredhold 2
#define CertaintyRatio (7.2 / 8.0)
#else
#define LoadStore_Thredhold 3.25
#define CertaintyRatio (7.0 / 8.0)
#endif

#define Strict_LoadStore_Thredhold 3.25
#define Strict_CertaintyRatio (7.0 / 8.0)

#include "ClockInfo.h"
#include "HI_InstructionFiles.h"
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
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/LoopVersioning.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include <algorithm>
#include <map>
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

// Pass for simple evaluation of the latency of the top functiob, without considering HLS directives

class HI_ASAPScheduling : public ModulePass
{
public:
    static char ID;

    class timingBase;

    //////////////////////////////////////// Variables for ASAPScheduling ////////////////////////////////////////////

    int Loop_Counter;

    std::map<Loop *, int> Loop_id;

    // the latency of each loop
    std::map<BasicBlock *, timingBase> LoopLatency;

    // the latency of each block
    std::map<BasicBlock *, timingBase> BlockLatency;

    // the latency of each function
    std::map<Function *, timingBase> FunctionLatency;

    // Record whether the component is evaluated
    std::set<BasicBlock *> BlockEvaluated;
    std::set<BasicBlock *> Func_BlockEvaluated;
    std::set<BasicBlock *> LoopEvaluated;
    std::set<Function *> FunctionEvaluated;
    std::set<Instruction *> InstructionEvaluated;
    std::set<Instruction *> Instruction_FFAssigned;
    std::set<BasicBlock *> BlockVisited;
    std::set<BasicBlock *> Func_BlockVisited;
    std::set<Value *> ValueVisited;

    // Record the information of the processing
    raw_ostream *Evaluating_log;

    std::error_code ErrInfo;
    std::ifstream *config_file;

    // This pass requires a specified top_function name
    std::string top_function_name;

    // Record the relations between loops and blocks
    std::map<Loop *, std::vector<BasicBlock *> *> Loop2Blocks;
    std::map<BasicBlock *, std::vector<Loop *> *> Block2Loops;

    // record the list of outer loops for functions
    std::map<Function *, std::vector<Loop *>> Function2OuterLoops;

    // record which evaluated loop the block is belong to, so the pass can directly trace to the
    // loop for the latency
    std::map<BasicBlock *, Loop *> Block2EvaluatedLoop;

    // record the critical path from the loop header to the end of the specific block
    std::map<Loop *, std::map<BasicBlock *, timingBase>> BlockCriticalPath_inLoop;

    // record the critical path to the end of block in the function
    std::map<BasicBlock *, timingBase> tmp_BlockCriticalPath_inFunc;

    // record the critical path to the end of loops in the function
    std::map<Loop *, timingBase> tmp_LoopCriticalPath_inFunc;

    // record the critical path to the end of sub-loops in the loop
    std::map<BasicBlock *, timingBase> tmp_BlockCriticalPath_inLoop;

    // record the critical path from the outter loop header to the end of the specific sub-loop
    std::map<Loop *, timingBase> tmp_SubLoop_CriticalPath;

    // record the critical path from the block entry to the end of the specific instruction
    std::map<BasicBlock *, std::map<Instruction *, timingBase>> InstructionCriticalPath_inBlock;

    // Instruction Schedule
    std::map<Instruction *, std::pair<BasicBlock *, int>> Inst_Schedule;

    // record the latest operand of each instruction
    std::map<Instruction *, Instruction *> Inst2LatestOperand;

    // record when the register for the result of Instruction can be release
    std::map<Instruction *, std::pair<BasicBlock *, int>> RegRelease_Schedule;

    // record whether the result reg of the instruction I has been reused
    std::set<Instruction *> I_RegReused;

    bool topFunctionFound = 0;

    float clock_period = 10.0;

    int top_function_latency;

    std::string clock_period_str = "10.0";

    std::string HLS_lib_path = "";

    //////////////////////////////////////// Some LLVM Analysis could be involved ///////////////////////////////////////////
    ScalarEvolution *SE;

    LoopInfo *LI;

    // LoopAccessLegacyAnalysis *LAA;

    //////////////////////////////////////// A unit class to store the information of timing for instruction ///////////////////////////////////////////
    class inst_timing_info
    {
    public:
        int Lat;
        int delay;
        int timing;
        int II;

        std::string core_name;

        inst_timing_info &operator=(inst_timing_info input)
        {
            Lat = input.Lat;
            timing = input.timing;
            II = input.II;
            core_name = input.core_name;
            return *this;
        }

        inst_timing_info(const inst_timing_info &input)
        {
            Lat = input.Lat;
            timing = input.timing;
            II = input.II;
            core_name = input.core_name;
        }

        inst_timing_info()
        {
            Lat = 0;
            timing = 0;
            II = 0;
            core_name = "";
        }

        void print()
        {
            llvm::errs() << " Lat=" << Lat << " timing=" << timing << " II" << II << "\n";
        }
    };

    class timingBase
    {
    public:
        int latency;
        double timing;
        int II;
        double clock_period;
        bool strict_timing = false;

        timingBase(int l, double t, int i, double p)
        {
            latency = l;
            timing = t;
            II = i;
            clock_period = p;
        }

        timingBase &operator=(timingBase input)
        {
            latency = input.latency;
            timing = input.timing;
            II = input.II;
            clock_period = input.clock_period;
            strict_timing = input.strict_timing;
            return *this;
        }

        timingBase()
        {
            latency = 0;
            II = 0;
            timing = 0;
            clock_period = 0;
        }
    };

    friend timingBase operator+(timingBase lhs, timingBase rhs)
    {
        assert(lhs.clock_period == rhs.clock_period);
        if (rhs.latency == -1) // for operation like br/ret
        {
            lhs.timing = rhs.timing;
            lhs.latency++;
            return lhs;
        }
        if (rhs.strict_timing)
        {
            if (rhs.latency == -2) // for operation like load
            {
                if (lhs.clock_period - lhs.timing > Strict_LoadStore_Thredhold)
                {
                    lhs.timing = rhs.timing;
                    lhs.latency++;
                    return lhs;
                }
                else
                {
                    lhs.timing = rhs.timing;
                    lhs.latency += 2;
                    return lhs;
                }
            }
        }
        else
        {
            if (rhs.latency == -2) // for operation like load
            {
                if (lhs.clock_period - lhs.timing > LoadStore_Thredhold)
                {
                    lhs.timing = rhs.timing;
                    lhs.latency++;
                    return lhs;
                }
                else
                {
                    lhs.timing = rhs.timing;
                    lhs.latency += 2;
                    return lhs;
                }
            }
        }

        lhs.latency = lhs.latency + rhs.latency;
        lhs.timing = lhs.timing + rhs.timing;
        if (rhs.strict_timing)
        {
            if (lhs.timing > lhs.clock_period * Strict_CertaintyRatio)
            {
                lhs.timing = rhs.timing;
                lhs.latency++;
            }
        }
        else
        {
            if (lhs.timing > lhs.clock_period * CertaintyRatio)
            {
                lhs.timing = rhs.timing;
                lhs.latency++;
            }
        }
        return lhs;
    }

    friend timingBase operator-(timingBase lhs, timingBase rhs)
    {
        assert(lhs.clock_period == rhs.clock_period);
        if (rhs.latency < 0) // for operation like br/ret
        {
            lhs.latency--;
            return lhs;
        }
        lhs.latency -= rhs.latency;
        return lhs;
    }

    friend bool operator>(timingBase lhs, timingBase rhs)
    {
        assert(lhs.clock_period == rhs.clock_period);
        return (((lhs.latency > rhs.latency)) || (lhs.latency == rhs.latency && lhs.timing > rhs.timing));
    }

    friend timingBase operator*(timingBase lhs, int rhs)
    {
        if (lhs.timing > 0.0001)
        {
            lhs.latency++;
            lhs.timing = 0;
        }
        lhs.latency *= rhs;
        return lhs;
    }

    friend timingBase operator*(int rhs, timingBase lhs)
    {
        if (lhs.timing > 0.0001)
        {
            lhs.latency++;
            lhs.timing = 0;
        }
        lhs.latency *= rhs;
        return lhs;
    }

    friend raw_ostream &operator<<(raw_ostream &stream, const timingBase &tb)
    {
        stream << " [latency=" << tb.latency << ", timing=" << tb.timing << "] ";
        return stream;
    }

    //////////////////////////////////////// Functions used for evluating the lantency of Function, BasicBlock, Instruction///////////////////////////////////////////
    HI_ASAPScheduling(const char *config_file_name, const char *Evaluating_log_name, const char *top_function) : ModulePass(ID)
    {
        BlockEvaluated.clear();
        LoopEvaluated.clear();
        FunctionEvaluated.clear();
        Loop_id.clear();
        Loop_Counter = 0;
        config_file = new std::ifstream(config_file_name);
        Evaluating_log = new raw_fd_ostream(Evaluating_log_name, ErrInfo, sys::fs::OF_None);
        top_function_name = std::string(top_function);

        // Get the configuration from the file, e.g. clock period
        Parse_Config();

        // Load the HLS database of timing and resource
        Load_Instruction_Info();
    }

    ~HI_ASAPScheduling()
    {
        for (auto ele : Loop2Blocks)
        {
            delete ele.second;
        }
        for (auto ele : Block2Loops)
        {
            delete ele.second;
        }
        Evaluating_log->flush();
        delete Evaluating_log;
    }

    virtual bool doInitialization(Module &M)
    {
        print_status("Initilizing HI_NoDirectiveTimingResourceEvaluation pass.");
        Loop_id.clear();
        LoopLatency.clear();
        BlockLatency.clear();
        FunctionLatency.clear();
        BlockEvaluated.clear();
        Func_BlockEvaluated.clear();
        LoopEvaluated.clear();
        FunctionEvaluated.clear();
        InstructionEvaluated.clear();
        BlockVisited.clear();
        Func_BlockVisited.clear();
        Instruction_FFAssigned.clear();
        Function2OuterLoops.clear();
        Block2EvaluatedLoop.clear();
        BlockCriticalPath_inLoop.clear();
        tmp_BlockCriticalPath_inFunc.clear();
        tmp_LoopCriticalPath_inFunc.clear();
        tmp_BlockCriticalPath_inLoop.clear();
        tmp_SubLoop_CriticalPath.clear();
        InstructionCriticalPath_inBlock.clear();
        return false;
    }

    // set the dependence of Passes
    void getAnalysisUsage(AnalysisUsage &AU) const;

    // parse the file to get configuration
    void Parse_Config();

    // load the timing and resource information for the instructions
    void Load_Instruction_Info();

    virtual bool runOnModule(Module &M);

    // check whether the block is in some loops
    bool isInLoop(BasicBlock *BB);

    // evaluatate the latency of a outer loop, which could be a nested one,  and return the timing
    // information
    timingBase analyzeOuterLoop(Loop *outerL);

    // get the most outer loop which contains the block, treat the loop as a node for the evaluation
    // of latency
    Loop *getOuterLoopOfBlock(BasicBlock *B);

    // find the inner unevaluated loop for processing
    Loop *getInnerUnevaluatedLoop(Loop *outerL);

    // evaluate a loop in which all the children loops have been evauluated and return the timing
    // information
    timingBase analyzeLoop_InnerChecked(Loop *L);

    // demangle the name of functions
    std::string demangleFunctionName(std::string mangled_name);

    // get the latency of functions in the module  and compute the resource cost
    void AnalyzeFunctions(Module &M);

    // get the latency of TopFunction Latency
    void analyzeTopFunction(Module &M);

    // get the function latency  and compute the resource cost
    timingBase analyzeFunction(Function *F);

    // get how many state needed for the function
    int getFunctionStageNum(timingBase tmp_critical_path, Function *F, BasicBlock *curBlock);

    // get how many state needed for the application
    int getTotalStateNum(Module &M);

    // get the number of stage arrive the instruction
    int getStageTo(Instruction *I);

    /*
        get the latency of functions in the path to the instruction
    */
    int getFunctionLatencyInPath(Instruction *I);

    // get the number of stage in the block
    int getStageNumOfBlock(BasicBlock *B);

    // get the function critical path by traversing the blocks based on DFS and compute the resource
    // cost
    void analyzeFunction_traverseFromEntryToExiting(timingBase tmp_critical_path, Function *F, BasicBlock *curBlock);

    // get the loop latency by traversing from the header to the exiting blocks and evluation
    // resource
    void LoopLatencyResourceEvaluation_traversFromHeaderToExitingBlocks(timingBase tmp_critical_path, Loop *L,
                                                                        BasicBlock *curBlock);

    // mark the block in loop with latency by traversing from the header to the exiting blocks
    void MarkBlock_traversFromHeaderToExitingBlocks(timingBase total_latency, Loop *L, BasicBlock *curBlock);

    // evaluate the block latency by traversing the instructions
    timingBase BlockLatencyEvaluation(BasicBlock *B);

    // check whether the instruction is in the block
    bool BlockContain(BasicBlock *B, Instruction *I);

    // get the latency of a specific instruction
    timingBase getInstructionLatency(Instruction *I);

    // check whether all the sub-function are evaluated
    bool CheckDependencyFesilility(Function &F);

    // get the relationship between loops and blocks
    void getLoopBlockMap(Function *F);

    // For each type of instruction, there will be a list to store a series of information
    // corresponding to different parameters
    typedef std::map<int, std::map<int, std::map<std::string, inst_timing_info>>> Info_type_list;

    // A map from opcode to the information list of hte timing
    std::map<std::string, Info_type_list> BiOp_Info_name2list_map;

    // Get the information of a specific instruction, based on its opcode, operand_bitwidth,
    // result_bitwidth and clock_period
    inst_timing_info get_inst_info(std::string opcode, int operand_bitwid, int res_bitwidth,
                                   std::string period);

    // Organize the information into timingBase after getting the information of a specific
    // instruction, based on its opcode, operand_bitwidth, result_bitwidth and clock period.
    timingBase get_inst_TimingInfo_result(std::string opcode, int operand_bitwid, int res_bitwidth, std::string period);

    // Trace back to find the original_operator, bypassing SExt and ZExt operations
    Instruction *byPassUnregisterOp(Instruction *cur_I);

    Value *byPassBitcastOp(Instruction *cur_I);

    Value *byPassBitcastOp(Value *cur_I_val);

    // Check whether we can infer the information by increasing the clock frequency
    bool checkInfoAvailability(std::string opcode, int operand_bitwid, int res_bitwidth, std::string period);

    // check whether we can infer the information by increasing the clock frequency
    bool checkFreqProblem(std::string opcode, int operand_bitwid, int res_bitwidth, std::string period);

    inst_timing_info checkInfo_HigherFreq(std::string opcode, int operand_bitwid,
                                          int res_bitwidth, std::string period);
    // Trace back to get the bitwidth of an operand, bypassing truct/zext/sext
    int getOriginalBitwidth(Value *Val);

    int getActualUsersNumm(Instruction *I, int dep);

    // Check whether the two operations can be chained
    bool canChainOrNot(Instruction *PredI, Instruction *I);

    // Check whether the two operations can be chained into a MAC operation
    bool isMACpossible(Instruction *PredI, Instruction *I);

    // Check whether the three operations can be chained into an AMA operation
    bool isAMApossible(Instruction *PredI, Instruction *I);

    struct timeval tv_begin;
    struct timeval tv_end;
};

#endif