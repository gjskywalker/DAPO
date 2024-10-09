#include "HI_IR2Verilog.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

void HI_IR2Verilog::argument(Module &M)
{
    for (Function &F : M)
    {
        if (F.getName().find("llvm.") != std::string::npos ||
            F.getName().find("HIPartitionMux") != std::string::npos)
        {
            continue;
        }
        else
        {
            std::string fname(F.getName().str());
            if (fname[0] == '_')
            {
                *Verilogfile << "module  " << DemangleFunctionName(F.getName().str()) << "\n";
                *Verilogfile << "("
                             << "\n";
                Function_Demangle_Map[F.getName().str()] = DemangleFunctionName(fname);
            }
            for (Function::arg_iterator arg = F.arg_begin(), E = F.arg_end(); arg != E; ++arg)
            {
                if (arg->getType()->isIntegerTy())
                {
                    *Verilogfile << "   input [" << arg->getType()->getIntegerBitWidth() - 1 << ":0] " << Standard_Name(arg->getName().str()) << ',' << "\n";
                }
                else if (arg->getType()->isPointerTy())
                {
                    *Verilogfile << "   output reg [" << GetArgArrayDwidth(M, Standard_Name(arg->getName().str())) - 1 << ":0] " << Standard_Name(arg->getName().str()) << "_address0" << ',' << "\n";
                    *Verilogfile << "   output reg [" << GetArgArrayDwidth(M, Standard_Name(arg->getName().str())) - 1 << ":0] " << Standard_Name(arg->getName().str()) << "_address1" << ',' << "\n";
                    *Verilogfile << "   output reg " << Standard_Name(arg->getName().str()) << "_ce0" << ',' << "\n";
                    *Verilogfile << "   output reg " << Standard_Name(arg->getName().str()) << "_ce1" << ',' << "\n";
                    *Verilogfile << "   input [" << GetArgArrayAwidth(arg) - 1 << ":0] " << Standard_Name(arg->getName().str()) << "_d0" << ',' << "\n";
                    *Verilogfile << "   output [" << GetArgArrayAwidth(arg) - 1 << ":0] " << Standard_Name(arg->getName().str()) << "_d1" << ',' << "\n";
                    int DWIDTH = GetArgArrayDwidth(M, Standard_Name(arg->getName().str()));
                    int AWIDTH = GetArgArrayAwidth(arg);
                    std::pair<int, int> WIDTH(DWIDTH, AWIDTH);
                    std::string Arrayname = Standard_Name(arg->getName().str());
                    ArrayInfo.insert(std::pair<std::string, std::pair<int, int>>(Arrayname, WIDTH));
                }
            }
            if (F.getReturnType()->isIntegerTy())
            {
                *Verilogfile << "   input clk,"
                             << "\n";
                *Verilogfile << "   input rstn,"
                             << "\n";
                *Verilogfile << "   output reg [" << F.getReturnType()->getIntegerBitWidth() - 1 << ":0] result"
                             << "\n";
                *Verilogfile << ");"
                             << "\n";
            }
            else
            {
                *Verilogfile << "   input clk,"
                             << "\n";
                *Verilogfile << "   input rstn"
                             << "\n";
                *Verilogfile << ");"
                             << "\n";
            }
        }
    }
    for (auto &it : RAMInfo)
    {
        std::string RAMname = it.first;
        int DWIDTH = it.second.first;
        int AWIDTH = it.second.second;
        *Verilogfile << "   reg [" << AWIDTH << " - 1:0] " << RAMname << "waddr;"
                     << "\n"
                     << "   reg [" << AWIDTH << " - 1:0] " << RAMname << "raddr;"
                     << "\n"
                     << "   reg " << RAMname << "ce;"
                     << "\n"
                     << "   reg [" << DWIDTH << " - 1:0] " << RAMname << "wd;"
                     << "\n"
                     << "   wire [" << DWIDTH << " - 1:0] " << RAMname << "rd;"
                     << "\n"
                     << "\n"

                     << "   test_" << RAMname << "ram " << RAMname << "U("
                     << "\n"
                     << "       .clk(clk),"
                     << "\n"
                     << "       .ce(" << RAMname << "ce),"
                     << "\n"
                     << "       ." << RAMname << "waddr(" << RAMname << "waddr),"
                     << "\n"
                     << "       ." << RAMname << "raddr(" << RAMname << "raddr),"
                     << "\n"
                     << "       ." << RAMname << "wd(" << RAMname << "wd),"
                     << "\n"
                     << "       ." << RAMname << "rd(" << RAMname << "rd)"
                     << "\n"
                     << "   );"
                     << "\n"
                     << "\n";
    }
}

int HI_IR2Verilog::GetArgArrayAwidth(Value *arg)
{
    for (auto it = arg->use_begin(), ie = arg->use_end(); it != ie; ++it)
    {
        if (PtrToIntInst *PTI = dyn_cast<PtrToIntInst>(it->getUser()))
        {
            if (PTI->getType()->isIntegerTy())
                return PTI->getType()->getIntegerBitWidth();
        }
    }
}

int HI_IR2Verilog::GetArgArrayDwidth(Module &M, std::string ArrayName)
{
    for (auto &F : M)
        for (auto &B : F)
            for (auto &I : B)
            {
                if (StoreInst *StoreI = dyn_cast<StoreInst>(&I))
                {
                    if (AccessMap[removeExtraSpace(getStringInstruction(*StoreI))] == ArrayName)
                    {
                        if (StoreI->getValueOperand()->getType()->isIntegerTy())
                        {
                            return StoreI->getValueOperand()->getType()->getIntegerBitWidth();
                        }
                    }
                }
                else if (LoadInst *LoadI = dyn_cast<LoadInst>(&I))
                {
                    if (AccessMap[removeExtraSpace(getStringInstruction(*LoadI))] == ArrayName)
                    {
                        if (LoadI->getType()->isIntegerTy())
                        {
                            return LoadI->getType()->getIntegerBitWidth();
                        }
                    }
                }
            }
}
