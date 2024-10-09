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
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

void HI_IR2Verilog::Array_Process(Module &M)
{
    for (auto &Global : M.getGlobalList())
    {
        // *Arrayfile << Global.getType()->getTypeID() << "\n";
        // if (Global.getType()->isArrayTy())
        // {
        //     *Arrayfile << "The array name is "
        //                << "\n";
        //     *Arrayfile << Standard_Name(Global.getName().str()) << "\n";
        // }
        // if (Global.getType()->isIntegerTy())
        // {
        //     *Arrayfile << "The integer name is "
        //                << "\n";
        //     *Arrayfile << Standard_Name(Global.getName().str()) << "\n";SS
        // }
        // if (Global.getType()->isPointerTy())
        // {
        //     PointerType *PT = dyn_cast<PointerType>(Global.getType());
        //     *Arrayfile << "The pointer name is "
        //                << "\n";
        //     *Arrayfile << PT->getContext() << "\n";
        //     *Arrayfile << Standard_Name(Global.getName().str()) << "\n";
        // }
        // *Arrayfile << "this is the global variable " << Global << "\n";
        // *Arrayfile << "this is the result of the getAggregateElement function " << Global.getAggregateElement() << "\n";
        // *Arrayfile << "this is the result of the getAlign function " << Global.getAlign() << "\n";
        // *Arrayfile << "this is the result of the getGlobalIdentifier function " << Global.getGlobalIdentifier() << "\n";
        // *Arrayfile << "this is the result of the getIntegetValue function " << Global.getIntegerValue() << "\n";
        // *Arrayfile << "this is the result of the getOperand function " << Global.getOperand() << "\n";
        // *Arrayfile << "-------------------------------------------------"
        //            << "\n";
        *Arrayfile << Global.getValueType()->isIntegerTy() << "\n";
        *Arrayfile << *(Global.getInitializer()) << "\n";
        *Arrayfile << Global.getValueType()->isArrayTy() << "\n";
        // if (auto tmp_arrayptr = dyn_cast<ArrayType>(Global.getValueType()))
        // {
        //     // we can use this function to get the context of the array
        //     *Arrayfile << Global.getInitializer() << "\n";
        //     *Arrayfile << *(Global.getInitializer()) << "\n";
        //     *Arrayfile << Global.getInitializer() << "\n";
        //     // but why it gets 0 ? result I think should equal to getArrayNumElements is 6.
        //     *Arrayfile << Global.getInitializer()->getNumOperands() << "\n";
        //     *Arrayfile << "getArrayElementType: " << *(tmp_arrayptr->getArrayElementType()) << "\n";
        //     *Arrayfile << "getArrayNumElements: " << tmp_arrayptr->getArrayNumElements() << "\n";
        //     *Arrayfile << *tmp_arrayptr << "\n";
        // }
        // Global.isConstantGlobal();
        // Global.constanta
    }
}

void HI_IR2Verilog::GenerateRAM(Module &M)
{
    int DWIDTH = 0;
    int AWIDTH = 0;
    std::string RAMname;
    int MEM_SIZE = 1;
    for (auto &F : M)
        for (auto &B : F)
            for (auto &I : B)
            {
                if (AllocaInst *allocI = dyn_cast<AllocaInst>(&I))
                {
                    *Verilogfile << "`include \" RAM.v \""
                                 << "\n"
                                 << "\n";
                    RAMname = Standard_Name(allocI->getName().str()) + "_";
                    Type *tmp_type = allocI->getAllocatedType();
                    while (auto array_T = dyn_cast<ArrayType>(tmp_type))
                    {
                        MEM_SIZE *= (array_T->getNumElements());
                        tmp_type = array_T->getArrayElementType();
                    }
                    if (tmp_type->isIntegerTy())
                        DWIDTH = tmp_type->getIntegerBitWidth();
                    else if (tmp_type->isFloatTy())
                        DWIDTH = 32;
                    else if (tmp_type->isDoubleTy())
                        DWIDTH = 64;
                    AWIDTH = ceil(log2(MEM_SIZE));
                    std::pair<int, int> WIDTH(DWIDTH, AWIDTH);
                    RAMInfo.insert(std::pair<std::string, std::pair<int, int>>(RAMname, WIDTH));
                }
            }
    assert(DWIDTH != 0 && "we should get BW for the basic element type.\n");
    *RAMfile << "module test_" << RAMname << "ram ("
             << "\n"
             << RAMname << "waddr," << RAMname << "raddr, " << RAMname << "wd, " << RAMname << "rd, ce, clk);"
             << "\n"
             << "\n"
             << "parameter DWIDTH = " << DWIDTH << ";"
             << "\n"
             << "parameter AWIDTH = " << AWIDTH << ";"
             << "\n"
             << "parameter MEM_SIZE = " << MEM_SIZE << ";"
             << "\n"
             << "\n"
             << "input [AWIDTH - 1:0] " << RAMname << "waddr;"
             << "\n"
             << "input [AWIDTH - 1:0] " << RAMname << "raddr;"
             << "\n"
             << "input ce;"
             << "\n"
             << "input wire [DWIDTH - 1:0] " << RAMname << "wd;"
             << "\n"
             << "output wire [DWIDTH - 1:0] " << RAMname << "rd;"
             << "\n"
             << "input clk;"
             << "\n"
             << "\n"
             << "reg [DWIDTH - 1:0] ram [0:MEM_SIZE - 1];"
             << "\n"
             << "reg [DWIDTH - 1:0] q0_t0;"
             << "\n"
             << "reg [DWIDTH - 1:0] q0_t1;"
             << "\n"
             << "reg [DWIDTH - 1:0] q0_t2;"
             << "\n"
             << "\n"
             << "assign " << RAMname << "rd = q0_t1;"
             << "\n"
             << "always @(posedge clk)"
             << "\n"
             << "begin"
             << "\n"
             << "    if (ce)"
             << "\n"
             << "   begin"
             << "\n"
             << "        q0_t1 <= q0_t0;"
             << "\n"
             << "    end"
             << "\n"
             << "end"
             << "\n"
             << "\n"
             << "always @(posedge clk)"
             << "\n"
             << "    begin if (ce)"
             << "\n"
             << "        begin"
             << "\n"
             << "            q0_t0 <=ram[" << RAMname << "raddr];"
             << "\n"
             << "        end"
             << "\n"
             << "   end"
             << "\n"
             << "\n"
             << "always @(posedge clk)"
             << "\n"
             << "begin"
             << "\n"
             << "    if (ce)"
             << "\n"
             << "   begin"
             << "\n"
             << "        q0_t2 <= " << RAMname << "wd;"
             << "\n"
             << "    end"
             << "\n"
             << "end"
             << "\n"
             << "\n"
             << "always @(posedge clk)"
             << "\n"
             << "    begin if (ce)"
             << "\n"
             << "        begin"
             << "\n"
             << "            ram[" << RAMname << "raddr] <= q0_t2;"
             << "\n"
             << "        end"
             << "\n"
             << "   end"
             << "\n"
             << "\n"
             << "endmodule";
}