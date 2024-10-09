#ifndef _HI_Binding
#define _HI_Binding

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
#include "llvm/IR/Constant.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/IR/Pass.h"
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

class HI_Binding : public ModulePass
{
public:
    HI_Binding(const char *config_file_name, const char *evaluating_log_name, const char *BRAM_log_name, const char *top_function) : ModulePass(ID)
    {
        BlockEvaluated.clear();
        LoopEvaluated.clear();
        FunctionEvaluated.clear();
        Loop_id.clear();
        Loop_Counter = 0;
        config_file = new std::ifstream(config_file_name);
        Evaluating_log = new raw_fd_ostream(evaluating_log_name, ErrInfo, sys::fs::OF_None);
        BRAM_log = new raw_fd_ostream(BRAM_log_name, ErrInfo, sys::fs::OF_None);
        top_function_name = std::string(top_function);
        FF_log = new raw_fd_ostream("FF_LOG", ErrInfo, sys::fs::OF_None);

        // Get the configuration from the file, e.g. clock period
        Parse_Config();

        // Load the HLS database of resource
        Load_Instruction_Info();
    }

    ~HI_Binding()
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
        LoopResource.clear();
        BlockResource.clear();
        FunctionResource.clear();

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

    class resourceBase;

    // Set the dependence of Passes
    void getAnalysisUsage(AnalysisUsage &AU) const;

    // Parse the file to get configuration
    void Parse_Config();

    // Load the timing and resource information for the instructions
    void Load_Instruction_Info();

    virtual bool runOnModule(Moduel &M);

    ////////////////////// Declaration related to resource evaluation of Basic Block/Loop/Function //////////////////

    // Check whether the block is in some loops
    bool isInLoop(BasicBlock *BB);

    // Get the most outer loop which contains the block, treat the loop as a node for the evaluation
    Loop *getOuterLoopOfBlock(BasicBlock *B);

    // Find the inner unevaluated loop for processing
    Loop *getInnerUnevaluatedLoop(Loop *outerL);

    static char ID;

    int Loop_Counter;

    std::map<Loop *, int> Loop_id;

    // The resource of each loop
    std::map<Function *, resourceBase> LoopResource;

    // The resource of each BasicBlock
    std::map<BasicBlock *, resourceBase> BlockResource;

    // The resoure of each function
    std::map<Function *, resourceBase> FunctionResource;

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

    // Record the infomation of the processing
    raw_ostream *Evaluating_log;

    // Record the information of the BRAMs and related accesses to BRAMs
    raw_ostream *BRAM_log;

    // Record the information of the BRAMs and related access to FF
    raw_ostream *FF_log;

    std::error_code ErrrInfo;
    std::if_stream *config_file;

    // The pass requires a specified top_function name
    std::string top_function_name;

    // Record the relations between loops and blocks
    std::map<Loop *, std::vector<BasicBlock *> *> Loop2Blocks;
    std::map<BasicBlock *, std::vector<Loop *> *> Block2Loops;

    // Record the list of outer loops for functions
    std::map<Function *, std::vector<Loop *>> Function2OuterLoops;

    // Record which evaluated loop the block is belong to, so the pass can directly trace
    // to the loop for the latency
    std::map<BasicBlock *, Loop *> Block2EvaluatedLoop;

    // Record the latest operand of each instruction
    std::map<Instruction *, Instruction *> Inst2LatestOperand;

    // record when the register for the result of Instruction can be release
    std::map<Instruction *, std::pair<BasicBlock *, int>> RegRelease_Schedule;

    // record whether the result reg of the instruction I has been reused
    std::set<Instruction *> I_RegReused;

    // Demangle the name of functions
    std::string demangleFunctionName(std::string mangled_name);

    // Compute the resource cost
    void AnalyzeFunctions(Module &M);

    // Get the resource cost of top_function
    void analyzeTopFunction(Module &M);

    // Compute the resource cost
    void analyzeFunction(Function *F);

    // Return the resource cost of the function
    resourceBase getFunctionResource(Function *F);

    // Get the function critical path by traversing the blocks based on DFS and compute the resource
    void analyzeFunction_traverseFromEntryToExiting(Function *F, BasicBlock *curBlock, resourceBase &resourceAccumulator);

    // Get the loop resource by traversing from the header to the exiting blocks and evaluation resource
    void LoopResourceEvaluation_traversFromHeaderToExitingBlocks(Loop *L, BasicBlock *curBlock, resourceBase &resourceAccumulator);

    // Mark the block in loop with latency by traversing from the header to the exiting blocks
    void MarkBlock_traversFromHeaderToExitingBlocks(Loop *L, BasicBlock *curBlock);

    // Evaluate the block resource by traversing the instructions
    void BlockResourceEvaluation(BasicBlock *B);

    // Get the resource cost of a specific instruction
    resourceBase getInstructionResource(Instruction *I);

    // Update the latest user of the specific user, based on which we can determine the lifetime of a register
    void updateResultRelease(Instruction *I, Instruction *I_Pred, int time_point);

    // check whether all the sub-function are evaluated
    bool CheckDependencyFesilility(Function &F);

    // get the relationship between loops and blocks
    void getLoopBlockMap(Function *F);

    // Calculate the LUT for state
    int LUT_for_FSM(int stateNum);

    // some LLVM analysises could be involved
    ScalarEvolution *SE;
    LoopInfo *LI;
    // LoopAccessLegacyAnalysis *LAA;

    bool topFunctionFound = 0;

    float clock_period = 10.0;

    int top_function_latency;

    std::string clock_period_str = "10.0";

    std::string HLS_lib_path = "";

    //////////////////// Declaration related to resource of instructions
    ///////////////////////

    // A unit class to store the information of resource for instruction
    class inst_resource_info
    {
    public:
        // resource
        int FF;
        int DSP;
        int LUT;

        // core
        std::string core_name;

        inst_resource_info &operator=(inst_resource_info input)
        {
            FF = input.FF;
            DSP = input.DSP;
            LUT = input.LUT;
            core_name = input.core_name;
        }

        inst_resource_info(const inst_resource_info &input)
        {
            FF = input.FF;
            DSP = input.DSP;
            LUT = input.LUT;
            core_name = input.core_name;
        }

        inst_resource_info()
        {
            FF = 0;
            DSP = 0;
            LUT = 0;
            core_name = "";
        }

        void print()
        {
            llvm::errs() << " DSP=" << DSP << " FF=" << FF << " LUT=" << LUT << "\n";
        }
    };

    // A unit class to record the FPGA resource for a(n) instruction/block/function/loop
    class resourceBase
    {
    public:
        resourceBase(int D, int F, int L, double C)
        {
            DSP = D;
            FF = F;
            LUT = L;
            clock_period = C;
            BRAM = 0;
        }
        resourceBase(int B, int D, int F, int L, double C)
        {
            DSP = D;
            FF = F;
            LUT = L;
            clock_period = C;
            BRAM = B;
        }
        int DSP, FF, LUT;
        int BRAM = 0;
        double clock_period;

        resourceBase &operator=(resourceBase input)
        {
            DSP = input.DSP;
            FF = input.FF;
            LUT = input.LUT;
            BRAM = input.BRAM;
            clock_period = input.clock_period;
        }

        resourceBase(const resourceBase &input)
        {
            DSP = input.DSP;
            FF = input.FF;
            LUT = input.LUT;
            BRAM = input.BRAM;
            clock_period = input.clock_period;
        }

        resourceBase()
        {
            DSP = -1;
            FF = -1;
            LUT = -1;
            clock_period = -1;
            BRAM = -1;
        }
    };

    friend resourceBase operator+(resourceBase lhs, resourceBase rhs)
    {
        assert(lhs.clock_period == rhs.clock_period);
        lhs.DSP = lhs.DSP + rhs.DSP;
        lhs.FF = lhs.FF + rhs.FF;
        lhs.LUT = lhs.LUT + rhs.LUT;
        lhs.BRAM = lhs.BRAM + rhs.BRAM;
        return lhs;
    }

    friend resourceBase operator*(resourceBase lhs, int rhs)
    {
        lhs.DSP = lhs.DSP * rhs;
        lhs.FF = lhs.FF * rhs;
        lhs.LUT = lhs.LUT * rhs;
        lhs.BRAM = lhs.BRAM * rhs;
        return lhs;
    }

    friend resourceBase operator*(int rhs, resourceBase lhs)
    {
        lhs.DSP = lhs.DSP * rhs;
        lhs.FF = lhs.FF * rhs;
        lhs.LUT = lhs.LUT * rhs;
        lhs.BRAM = lhs.BRAM * rhs;
        return lhs;
    }

    friend raw_ostream &operator<<(raw_ostream &stream, const resourceBase &rb)
    {
        stream << " [DSP=" << rb.DSP << ", FF=" << rb.FF << ", LUT=" << rb.LUT << ", BRAM=" << rb.BRAM << "] ";
        return stream;
    }

    // For each type of instruction, there will be a list to store a series of information
    // corresponding to different parameters
    typedef std::map<int, std::map<int, std::map<std::string, inst_resource_info>>> Info_type_list;

    // A map from opcode to the information list of resource
    std::map<std::string, Info_type_list> BiOp_Info_name2list_map;

    // get the information of a specific instruction, based on its opcode, operand_bitwidth,
    // result_bitwidth and clock period
    inst_resource_info get_inst_info(std::string opcode, int operand_bitwid, int res_bitwidth,
                                     std::string period);

    // Organize the information into resourceBase after getting the information of a specific
    // instruction, based on its opcode, operand_bitwidth, result_bitwidth and clock period.
    resourceBase get_inst_ResourceInfo_result(std::string opcode, int operand_bitwid, int res_bitwidth,
                                              std::string period);

    // int get_N_DSP(std::string opcode, int operand_bitwid , int res_bitwidth, std::string period);
    // int get_N_FF(std::string opcode, int operand_bitwid , int res_bitwidth, std::string period);
    // int get_N_LUT(std::string opcode, int operand_bitwid , int res_bitwidth, std::string period);
    // int get_N_Lat(std::string opcode, int operand_bitwid , int res_bitwidth, std::string period);
    // double get_N_Delay(std::string opcode, int operand_bitwid , int res_bitwidth, std::string
    // period);

    // evaluate the number of FF needed by the instruction
    resourceBase FF_Evaluate(std::map<Instruction *, timingBase> &cur_InstructionCriticalPath, Instruction *cur_I);

    // trace back to find the original operator, bypassing SExt and ZExt operations
    Instruction *byPassUnregisterOp(Instruction *cur_I);

    Value *byPassBitcastOp(Instruction *cur_I);

    Value *byPassBitcastOp(Value *cur_I_val);

    // evaluate the number of LUT needed by the PHI instruction
    resourceBase IndexVar_LUT(std::map<Instruction *, timingBase> &cur_InstructionCriticalPath, Instruction *I);

    // check whether a specific information is in the database
    bool checkInfoAvailability(std::string opcode, int operand_bitwid, int res_bitwidth, std::string period);

    // check whether we can infer the information by increasing the clock frequency
    bool checkFreqProblem(std::string opcode, int operand_bitwid, int res_bitwidth, std::string period);

    // if the information is not found in database, we may infer the information by increasing the
    // clock frequency
    inst_resource_info checkInfo_HigherFreq(std::string opcode, int operand_bitwid, int res_bitwidth,
                                            std::string period);

    // Trace back to get the bitwidth of an operand, bypassing truct/zext/sext
    int getOriginalBitwidth(Value *Val);

    // Trace forward to get the number of users, bypassing truct/zext/sext
    int getActualUsersNum(Instruction *I, int dep);

    // check whether the two operations can be chained
    bool canChainOrNot(Instruction *PredI, Instruction *I);

    // check whether the two operations can be chained into a MAC operation
    bool isMACpossible(Instruction *PredI, Instruction *I);

    // check whether the three operations can be chained into a AMA operation
    bool isAMApossible(Instruction *PredI, Instruction *I);

    //////////////////// Declaration related to Memory Access Tracing ////////////////////

    // record which target arrays the instruction may access
    std::map<Instruction *, std::vector<Value *>> Access2TargetMap;

    // record that in the basic block, which instruction access which array at which cycle
    std::map<Value *, std::map<BasicBlock *, std::vector<std::pair<int, Instruction *>>>> target2LastAccessCycleInBlock;

    // register number for target array
    std::vector<Instruction *> AccessesList;

    // alias of array from functions to subfunctions
    std::map<Value *, Value *> Alias2Target;

    std::map<BasicBlock *, std::vector<Instruction *>> Block2AccessList;

    // Trace Memory Declaration in Module
    void TraceMemoryDeclarationinModule(Module &M);

    // find the array declaration in the function F and trace the accesses to them
    void findMemoryDeclarationin(Function *F, bool isTopFunction);

    // get the number of BRAMs which are needed by the alloca instruction
    resourceBase get_BRAM_Num_For(AllocaInst *alloca_I);

    // get the number of BRAMs which are needed by the array with specific parameters
    resourceBase get_BRAM_Num_For(int bitwidth, int depth);

    // find out which instrctuins are related to the array, going through PtrToInt, Add, IntToPtr,
    // Store, Load instructions
    void TraceAccessForTarget(Value *cur_node, Value *ori_node);

    // check whether the access to target array can be scheduled in a specific cycle
    bool checkBRAMAvailabilty(Instruction *access, Value *target, std::string StoreOrLoad, BasicBlock *cur_block,
                              timingBase cur_Timing);

    void insertBRAMAccessInfo(Value *target, BasicBlock *cur_block, int cur_latency, Instruction *access);

    // evaluate the number of LUT needed by the BRAM Mux
    resourceBase BRAM_MUX_Evaluate();

    // for load instructions, HLS will reuse the register for the data
    bool checkLoadOpRegisterReusable(Instruction *Load_I, int time_point);

    bool hasRAWHazard(Instruction *loadI, int cycle);

    Value *getTargetFromInst(Instruction *accessI);
    /*
        get the access target from instruction
    */
    Value *getAccessTarget(Instruction *Load_or_Store);

    // class HI_AAResult : public AAResultBase<HI_AAResult>
    // {
    //   public:
    //     explicit HI_AAResult() : AAResultBase()
    //     {
    //     }
    //     HI_AAResult(HI_AAResult &&Arg) : AAResultBase(std::move(Arg))
    //     {
    //     }

    //     AliasResult alias(const MemoryLocation &LocA, const MemoryLocation &LocB);
    // };

    /// Timer

    struct timeval tv_begin;
    struct timeval tv_end;
};

#endif
