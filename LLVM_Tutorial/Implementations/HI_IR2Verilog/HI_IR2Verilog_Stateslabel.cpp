#include "HI_IR2Verilog.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <ios>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

void HI_IR2Verilog::stateslabel(llvm::Module &M)
{
    int BlockLatencyTmp;
    for (Function &F : M)
    {
        if (F.getName().find("llvm.") != std::string::npos ||
            F.getName().find("HIPartitionMux") != std::string::npos)
        {
            continue;
        }
        else
        {
            *Verilogfile << "//machine state decode"
                         << "\n";
            for (BasicBlock &B : F)
            {
                if (SchedulingInfor[removeExtraSpace(B.getName().str())].latency == 0 || SchedulingInfor[removeExtraSpace(B.getName().str())].timing != 0)
                {
                    SchedulingInfor[removeExtraSpace(B.getName().str())].latency += 1;
                }
                BasicBlock_count += SchedulingInfor[removeExtraSpace(B.getName().str())].latency;
            }
            for (BasicBlock &B : F)
            {
                for (int i = 0; i < SchedulingInfor[removeExtraSpace(B.getName().str())].latency; i++)
                {
                    State_num++;
                    *Verilogfile << "   "
                                 << "parameter";
                    *Verilogfile << "   " << (Standard_Name(B.getName().str())) << i << " = " << (int)(log2(BasicBlock_count)) + 1 << "'d" << State_num << ";"
                                 << "\n";
                    // std::pair<BasicBlock *, int> BlockName_Latency(&B, SchedulingInfor[removeExtraSpace(B.getName().str())].latency);
                    // StatesName.insert(std::pair<std::pair<BasicBlock *, int>, std::string>(BlockName_Latency, removeExtraSpace(B.getName().str() + std::to_string(i))));
                }
                if (SchedulingInfor[removeExtraSpace(B.getName().str())].latency == 0 || SchedulingInfor[removeExtraSpace(B.getName().str())].timing != 0)
                {
                    SchedulingInfor[removeExtraSpace(B.getName().str())].latency -= 1;
                }
            }
            *Verilogfile << "//machine variable"
                         << "\n";
            *Verilogfile << "   reg"
                         << "   [" << (int)(log2(BasicBlock_count)) << ":0]"
                         << "   "
                         << "current_state;"
                         << "\n";
            *Verilogfile << "   reg"
                         << "   [" << (int)(log2(BasicBlock_count)) << ":0]"
                         << "   "
                         << "next_state;"
                         << "\n";
            *Verilogfile << "   reg"
                         << "   [" << (int)(log2(BasicBlock_count)) << ":0]"
                         << "   "
                         << "reg_state;"
                         << "\n";
        }
    }
}

std::string HI_IR2Verilog::Standard_Name(std::string Name)
{
    std::string States_Name;
    for (auto tmp : Name)
    {
        if (tmp == '.')
            continue;
        else
        {
            States_Name += std::toupper(tmp);
        }
    }
    return States_Name;
}