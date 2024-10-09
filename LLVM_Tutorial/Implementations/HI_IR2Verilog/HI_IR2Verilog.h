#ifndef _HI_IR2VERILOG
#define _HI_IR2VERILOG

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

#include "HI_print.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
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
#include <array>
#include <bits/stl_map.h>
#include <cstdio>
#include <cxxabi.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

using namespace llvm;

// Wire and Reg, State Number & Select;
class HI_IR2Verilog : public ModulePass
{
public:
    int State_num;
    double BasicBlock_count;
    std::set<std::string> variableset;
    BasicBlock *BB_state;
    std::set<Function *> Function_Checked;
    std::string top_function_name;
    std::error_code ErrInfo;
    raw_ostream *Verilogfile;
    raw_ostream *Arrayfile;
    raw_ostream *RAMfile;
    std::map<std::string, std::string> Function_Demangle_Map;
    // we use this set to store the qualified variables
    std::set<llvm::Instruction *> BindingVariables;

    // we use this map to store the variable name and the corresponding instruction, so we
    // can handle the unname variable
    std::map<llvm::Instruction *, std::string> VariablesName;

    // we use this map to store the state name
    std::map<std::pair<llvm::BasicBlock *, int>, std::string> StatesName;

    // we use this map to store the RAM infor, so this map is especially designed for the alloc instruction
    std::map<std::string, std::pair<int, int>> RAMInfo;

    // If the argument is an array type (pointer type), then its information should be stored as well, so comes this map
    std::map<std::string, std::pair<int, int>> ArrayInfo;

    struct timeval tv_begin;
    struct timeval tv_end;

    HI_IR2Verilog(const char *evaluating_log_name, const char *top_function) : ModulePass(ID)
    {
        State_num = 0;
        BasicBlock_count = 0;
        Verilogfile = new raw_fd_ostream(evaluating_log_name, ErrInfo, sys::fs::OF_None);
        Arrayfile = new raw_fd_ostream("Arrayfile", ErrInfo, sys::fs::OF_None);
        RAMfile = new raw_fd_ostream("RAM.v", ErrInfo, sys::fs::OF_None);
        top_function_name = std::string(top_function);
        Function_Demangle_Map.clear();
        VariablesName.clear();
        BindingVariables.clear();
        StatesName.clear();
        RAMInfo.clear();
    }
    ~HI_IR2Verilog()
    {
        Arrayfile->flush();
        delete Arrayfile;
        Verilogfile->flush();
        delete Verilogfile;
        RAMfile->flush();
        delete RAMfile;
    }

    // we use this function to process the Basic Block , Reg Varaible .....'s name, and change it to the standard Verilog name
    std::string Standard_Name(std::string Name);

    // we use this function to translate the BasicBlock to the states in Verilog
    void stateslabel(llvm::Module &M);

    // we use this function to generate the inputs and outputs in Verilog
    void argument(llvm::Module &M);

    // we use this function to figure out how many regs, we need to define.
    void variable(llvm::Module &M);

    // we use this function to judge the width of a variable
    int varwidth(Instruction *I);

    // we use this function to generate the body of statesmachine
    void statesbody(llvm::Module &M);

    // we use this function to process instructions in a BasicBlock
    void process_inst(llvm::Instruction &I);

    // we use this function to collect the array information for verilog code generation
    void Array_Process(llvm::Module &M);

    // we use this function to get the data width of an argument whose type is array
    int GetArgArrayAwidth(Value *arg);

    // we use this function to get the address width of an agurment whose type is array
    int GetArgArrayDwidth(Module &M, std::string ArrayName);

    // we use this function to parse the Scheduling_log file to collect the latency information of each block and instruction
    void ParseSchedulingfromFile();

    // we use this function to parse the RAM_log file to get the access infor
    void ParseRAMfromFile();

    // we use this function to modify the string and remove the space in it
    std::string removeExtraSpace(std::string tmp_s);

    // we use this function to do binding
    void NaiveBinding(Function &F);

    // we use this function to generate RAM
    void GenerateRAM(llvm::Module &M);

    bool BlockContain(BasicBlock *B, Instruction *I);

    bool JudgeVariable(BasicBlock *B, Instruction *I);

    std::string getStringInstruction(Instruction &I);

    void findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr);

    void getAnalysisUsage(AnalysisUsage &AU) const;
    bool runOnModule(llvm::Module &M);

    virtual bool doInitialization(llvm::Module &M)
    {
        print_status("Initializing HI_IR2Verilog pass.");
        Function_Checked.clear();
        return false;
    }
    static char ID;

    std::string DemangleFunctionName(std::string mangled_name)
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

    // A unit class to record the timing and latency for a(n) instruction/block/function/loop
    class timingBase
    {
    public:
        timingBase(int l, double t, int i, double p)
        {
            latency = l;
            timing = t;
            clock_period = p;
            II = i;
        }
        int latency;
        int II;
        double timing;
        double clock_period;

        timingBase &operator=(timingBase input)
        {
            latency = input.latency;
            II = input.II;
            timing = input.timing;
            clock_period = input.clock_period;
            strict_timing = input.strict_timing;
            return *this;
        }

        timingBase(const timingBase &input)
        {
            latency = input.latency;
            II = input.II;
            timing = input.timing;
            clock_period = input.clock_period;
            strict_timing = input.strict_timing;
        }
        timingBase()
        {
            latency = 0;
            II = 0;
            timing = 0;
            clock_period = 0;
        }
        bool strict_timing = false;
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
                    lhs.timing = rhs.timing; // rhs.timing = Strict_LoadStore_Thredhold
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

    // friend raw_ostream &operator<<(raw_ostream &stream, const timingBase &tb)
    // {
    //     stream << " [latency=" << tb.latency << ", timing=" << tb.timing << "] ";
    //     return stream;
    // }

    // we use this map to store the Scheduling information
    std::map<std::string, timingBase> SchedulingInfor;

    // we use this map to store the access relation between instruciton and array
    std::map<std::string, std::string> AccessMap;
};
#endif