#include "HI_IR2Verilog.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Operator.h"
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

void HI_IR2Verilog::statesbody(Module &M)
{
    for (Function &F : M)
    {
        if (F.getName().find("llvm.") != std::string::npos || F.getName().find("HIPartitionMux") != std::string::npos)
        {
            continue;
        }
        else
        {
            *Verilogfile << "//(1) state transfer"
                         << "\n";
            *Verilogfile << "   always@(posedge clk or negedge rstn) begin"
                         << "\n";
            *Verilogfile << "       if(!rstn) begin"
                         << "\n";
            *Verilogfile << "           current_state  <=  1'b1;"
                         << "\n";
            *Verilogfile << "       end"
                         << "\n";
            *Verilogfile << "       else begin"
                         << "\n";
            *Verilogfile << "           current_state  <=  next_state;"
                         << "\n";
            *Verilogfile << "       end"
                         << "\n";
            *Verilogfile << "   end"
                         << "\n";
            *Verilogfile << "//(2) state switch, using block assignment for combination-logic "
                         << "\n";
            *Verilogfile << "//all case items need to be displayed completely"
                         << "\n";
            *Verilogfile << "   always@(*) begin"
                         << "\n";
            *Verilogfile << "       case(current_state)"
                         << "\n";
            for (BasicBlock &B : F)
            {
                BasicBlock *Bptr = &B;
                if ((SchedulingInfor[Bptr->getName().str()].latency == 1 && SchedulingInfor[Bptr->getName().str()].timing == 0) || (SchedulingInfor[Bptr->getName().str()].latency == 0 && SchedulingInfor[Bptr->getName().str()].timing >= 0))
                {
                    // std::pair<BasicBlock *, int> BlockName_Lantency(Bptr, SchedulingInfor[removeExtraSpace(Bptr->getName().str())].latency);
                    *Verilogfile << "           " << Standard_Name(Bptr->getName().str()) << "0"
                                 << ":"
                                 << "\n";
                    *Verilogfile << "               "
                                 << "begin"
                                 << "\n";
                    for (Instruction &I : B)
                    {
                        if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
                        {
                            continue;
                        }
                        else if (IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(&I))
                        {
                            continue;
                        }
                        else if (StoreInst *SI = dyn_cast<StoreInst>(&I))
                        {
                            continue;
                        }
                        else if (LoadInst *LI = dyn_cast<LoadInst>(&I))
                        {
                            continue;
                        }
                        else
                        {
                            process_inst(I);
                        }
                    }
                    *Verilogfile << "                   "
                                 << "reg_state = " << Standard_Name(Bptr->getName().str()) << "0"
                                 << ";"
                                 << "\n";
                    *Verilogfile << "               "
                                 << "end"
                                 << "\n";
                }
                else
                {
                    for (int i = 0; i < SchedulingInfor[removeExtraSpace(B.getName().str())].latency + 1; i++)
                    {
                        // std::pair<BasicBlock *, int> BlockName_Lantency(Bptr, SchedulingInfor[removeExtraSpace(Bptr->getName().str())].latency);
                        *Verilogfile << "           " << Standard_Name(Bptr->getName().str()) << i << ":"
                                     << "\n";
                        *Verilogfile << "               "
                                     << "begin"
                                     << "\n";
                        for (Instruction &I : B)
                        {
                            // TODO : Here I just set the clock period manually, since the clock period I use is always 10 ns.
                            // But in the future I need to set the clock period automatically by reading from the configure file
                            if (SchedulingInfor[removeExtraSpace(getStringInstruction(I))].latency + SchedulingInfor[removeExtraSpace(getStringInstruction(I))].timing / 10 >= i && SchedulingInfor[removeExtraSpace(getStringInstruction(I))].latency + SchedulingInfor[removeExtraSpace(getStringInstruction(I))].timing / 10 <= i + 1)
                            {
                                if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
                                {
                                    continue;
                                }
                                else if (IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(&I))
                                {
                                    continue;
                                }
                                else if (StoreInst *SI = dyn_cast<StoreInst>(&I))
                                {
                                    continue;
                                }
                                else if (LoadInst *LI = dyn_cast<LoadInst>(&I))
                                {
                                    continue;
                                }
                                else
                                {
                                    if (i != SchedulingInfor[removeExtraSpace(B.getName().str())].latency)
                                    {
                                        if (BranchInst *BI = dyn_cast<BranchInst>(&I))
                                        {
                                            continue;
                                        }
                                        else
                                            process_inst(I);
                                    }
                                    else
                                    {
                                        process_inst(I);
                                    }
                                }
                            }
                            if (i == SchedulingInfor[removeExtraSpace(B.getName().str())].latency)
                            {
                                if (BranchInst *BI = dyn_cast<BranchInst>(&I))
                                {
                                    process_inst(I);
                                }
                            }
                        }
                        if (i != SchedulingInfor[removeExtraSpace(B.getName().str())].latency)
                        {
                            *Verilogfile << "                       "
                                         << "next_state = " << Standard_Name(Bptr->getName().str()) << i + 1
                                         << ";"
                                         << "\n";
                        }
                        *Verilogfile << "                   "
                                     << "reg_state = " << Standard_Name(Bptr->getName().str()) << i << ";"
                                     << "\n";
                        *Verilogfile << "               "
                                     << "end"
                                     << "\n";
                    }
                }
            }
            *Verilogfile << "       endcase"
                         << "\n";
            *Verilogfile << "   end"
                         << "\n";

            *Verilogfile << "//(3) BinaryOperator"
                         << "\n";
            *Verilogfile << "   always@(posedge clk or negedge rstn) begin"
                         << "\n";
            *Verilogfile << "       if(!rstn) begin"
                         << "\n";
            for (BasicBlock &B : F)
            {
                BasicBlock *Bptr = &B;
                if ((SchedulingInfor[Bptr->getName().str()].latency == 1 && SchedulingInfor[Bptr->getName().str()].timing == 0) || (SchedulingInfor[Bptr->getName().str()].latency == 0 && SchedulingInfor[Bptr->getName().str()].timing >= 0))
                {
                    // std::pair<BasicBlock *, int> BlockName_Lantency(Bptr, SchedulingInfor[removeExtraSpace(Bptr->getName().str())].latency);
                    *Verilogfile << "       if(current_state == " << Standard_Name(B.getName().str()) << "0"
                                 << ") begin "
                                 << "\n";
                    for (Instruction &I : B)
                    {
                        if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
                        {

                            process_inst(I);
                        }
                        else if (IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(&I))
                        {
                            process_inst(I);
                        }
                        else if (StoreInst *SI = dyn_cast<StoreInst>(&I))
                        {
                            process_inst(I);
                        }
                        else if (LoadInst *LI = dyn_cast<LoadInst>(&I))
                        {
                            process_inst(I);
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < SchedulingInfor[removeExtraSpace(B.getName().str())].latency + 1; i++)
                    {
                        // std::pair<BasicBlock *, int> BlockName_Lantency(Bptr, SchedulingInfor[removeExtraSpace(Bptr->getName().str())].latency);
                        *Verilogfile << "       if(current_state == " << Standard_Name(B.getName().str()) << i
                                     << ") begin "
                                     << "\n";
                        for (Instruction &I : B)
                        {
                            // TODO : Here I just set the clock period manually, since the clock period I use is always 10 ns.
                            // But in the future I need to set the clock period automatically by reading from the configure file
                            if (SchedulingInfor[removeExtraSpace(getStringInstruction(I))].latency + SchedulingInfor[removeExtraSpace(getStringInstruction(I))].timing / 10 >= i && SchedulingInfor[removeExtraSpace(getStringInstruction(I))].latency + SchedulingInfor[removeExtraSpace(getStringInstruction(I))].timing / 10 <= i + 1)
                            {
                                if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
                                {

                                    process_inst(I);
                                }
                                else if (IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(&I))
                                {
                                    process_inst(I);
                                }
                                else if (StoreInst *SI = dyn_cast<StoreInst>(&I))
                                {
                                    process_inst(I);
                                }
                                else if (LoadInst *LI = dyn_cast<LoadInst>(&I))
                                {
                                    process_inst(I);
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }
                        if (i != SchedulingInfor[removeExtraSpace(B.getName().str())].latency)
                        {
                            *Verilogfile << "       end"
                                         << "\n";
                        }
                    }
                }
                *Verilogfile << "       end"
                             << "\n";
            }
            *Verilogfile << "       end"
                         << "\n";
            *Verilogfile << "   end"
                         << "\n";
            *Verilogfile << "endmodule"
                         << "\n";
        }
    }
}

void HI_IR2Verilog::process_inst(Instruction &I)
{
    //////////////// Cast Operations /////////////////////////////
    if (PtrToIntInst *PTI = dyn_cast<PtrToIntInst>(&I))
    {
        *Verilogfile << "                       " << VariablesName[&I] << " = 0;"
                     << "\n";
    }
    else if (IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(&I))
    {
        // This is a lazy way to deal with the InToPtr Inst, by using this, there will be extra FF waste
        // A more properating way is to traverse the operands of load and store inst, and find the operand of its inttoptr
        // but this lazy way is more generable since I don't need to consider other situations
        *Verilogfile << "                       " << VariablesName[&I] << " <= ";
        auto l0_Pred = dyn_cast<Instruction>(I.getOperand(0));
        *Verilogfile << VariablesName[l0_Pred] << ";"
                     << "\n";
    }
    else if (FPToUIInst *FTU = dyn_cast<FPToUIInst>(&I))
    {
        // TODO
    }
    else if (UIToFPInst *UTF = dyn_cast<UIToFPInst>(&I))
    {
        // TODO
    }
    else if (SIToFPInst *STF = dyn_cast<SIToFPInst>(&I))
    {
        // TODO
    }
    else if (ZExtInst *ZEXTI = dyn_cast<ZExtInst>(&I))
    {
        // TODO
    }
    else if (SExtInst *SEXTI = dyn_cast<SExtInst>(&I))
    {
        // TODO
    }
    else if (TruncInst *TI = dyn_cast<TruncInst>(&I))
    {
        // TODO
    }
    else if (BitCastInst *BI = dyn_cast<BitCastInst>(&I))
    {
        // TODO
    }
    else if (CallInst *CallI = dyn_cast<CallInst>(&I))
    {
        // TODO
    }
    ////////////////// Binary Operations //////////////////////
    else if (ShlOperator *SHLI = dyn_cast<ShlOperator>(&I))
    {
        // TODO
    }
    else if (LShrOperator *LSHRI = dyn_cast<LShrOperator>(&I))
    {
        // TODO
    }
    else if (AShrOperator *ASHRI = dyn_cast<AShrOperator>(&I))
    {
        // TODO
    }
    else if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
    {
        // for binaty operator, it combines all the algorithim operator, such as add, mul, etc.
        //*Verilogfile << BinO->getOpcodeName() << "\n";
        if (BinO->getOpcode() == Instruction::Add)
        {
            *Verilogfile << "                       " << VariablesName[BinO] << " <= ";
            //*Verilogfile << "                       " << Standard_Name((BinO->getName().str())) << " <= ";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(0)))
                *Verilogfile << constval->getSExtValue();
            else
            {
                auto l0_Pred = dyn_cast<Instruction>(I.getOperand(0));
                *Verilogfile << VariablesName[l0_Pred];
            }
            *Verilogfile << " + ";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(1)))
                *Verilogfile << constval->getSExtValue() << ";"
                             << "\n";
            else
            {
                auto l1_Pred = dyn_cast<Instruction>(I.getOperand(1));
                *Verilogfile << VariablesName[l1_Pred] << ";"
                             << "\n";
            }
        }
        if (BinO->getOpcode() == Instruction::Mul)
        {
            *Verilogfile << "                       " << VariablesName[BinO] << " <= ";
            //*Verilogfile << "                       " << Standard_Name((BinO->getName().str())) << " <= ";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(0)))
                *Verilogfile << constval->getSExtValue();
            else
            {
                auto l0_Pred = dyn_cast<Instruction>(I.getOperand(0));
                *Verilogfile << VariablesName[l0_Pred];
            }
            *Verilogfile << " * ";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(1)))
                *Verilogfile << constval->getSExtValue() << ";"
                             << "\n";
            else
            {
                auto l1_Pred = dyn_cast<Instruction>(I.getOperand(1));
                *Verilogfile << VariablesName[l1_Pred] << ";"
                             << "\n";
            }
        }
    }
    else if (ICmpInst *ICI = dyn_cast<ICmpInst>(&I))
    {
        //*Verilogfile << *ICI->getOpcodeName() << "\n";
        if (ICI->isLT(ICI->getSignedPredicate()))
        {
            *Verilogfile << "                   "
                         << "if(";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(0)))
                *Verilogfile << constval->getSExtValue();
            else
            {
                auto l0_Pred = dyn_cast<Instruction>(I.getOperand(0));
                *Verilogfile << VariablesName[l0_Pred];
            }
            //*Verilogfile << Standard_Name(I.getOperand(0)->getName().str());
            *Verilogfile << " < ";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(1)))
                *Verilogfile << constval->getSExtValue() << ")"
                             << "\n";
            else
            {
                auto l1_Pred = dyn_cast<Instruction>(I.getOperand(1));
                *Verilogfile << VariablesName[l1_Pred] << ")"
                             << "\n";
            }
            *Verilogfile << "                       " << VariablesName[ICI] << " = 1;"
                         << "\n";
            *Verilogfile << "                   "
                         << "else "
                         << "\n";
            *Verilogfile << "                       " << VariablesName[ICI] << " = 0;"
                         << "\n";
        }
        if (ICI->isEquality(ICI->getSignedPredicate()))
        {
            *Verilogfile << "                   "
                         << "if(";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(0)))
                *Verilogfile << constval->getSExtValue();
            else
            {
                auto l0_Pred = dyn_cast<Instruction>(I.getOperand(0));
                *Verilogfile << VariablesName[l0_Pred];
            }
            *Verilogfile << " == ";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(I.getOperand(1)))
                *Verilogfile << constval->getSExtValue() << ")"
                             << "\n";
            else
            {
                auto l1_Pred = dyn_cast<Instruction>(I.getOperand(1));
                *Verilogfile << VariablesName[l1_Pred] << ")"
                             << "\n";
            }
            *Verilogfile << "                       " << VariablesName[ICI] << " = 1;"
                         << "\n";
            *Verilogfile << "                   "
                         << "else "
                         << "\n";
            *Verilogfile << "                       " << VariablesName[ICI] << " = 0;"
                         << "\n";
        }
    }
    else if (FCmpInst *FI = dyn_cast<FCmpInst>(&I))
    {
        // TODO
    }
    //////////////////////// Memory Operations ////////////////////////////
    else if (StoreInst *SI = dyn_cast<StoreInst>(&I))
    {
        std::string Instname = AccessMap[removeExtraSpace(getStringInstruction(I))];
        //*Verilogfile << "//Instname : " << Instname << "\n";
        for (auto &it : ArrayInfo)
        {
            if (it.first == Instname)
            {
                auto Op0 = dyn_cast<Instruction>(SI->getOperand(0));
                auto Op1 = dyn_cast<Instruction>(SI->getOperand(1));
                *Verilogfile << "                       " << Instname << "_ce1"
                             << " <= "
                             << "1"
                             << ";"
                             << "\n";
                *Verilogfile << "                       " << Instname << "_address1"
                             << " <= " << VariablesName[Op1] << ";"
                             << "\n";
                *Verilogfile << "                       " << Instname << "_d1"
                             << " <= " << VariablesName[Op0] << ";"
                             << "\n";
            }
        }
        for (auto &tmp : RAMInfo)
        {
            //*Verilogfile << "//RAMname : " << tmp.first << "\n";
            if (tmp.first == (Instname + "_"))
            {
                auto Op0 = dyn_cast<Instruction>(SI->getOperand(0));
                auto Op1 = dyn_cast<Instruction>(SI->getOperand(1));
                *Verilogfile << "                       " << Instname << "_ce"
                             << " <= "
                             << "1"
                             << ";"
                             << "\n";
                *Verilogfile << "                       " << Instname << "_waddr"
                             << " <= " << VariablesName[Op1] << ";"
                             << "\n";
                *Verilogfile << "                       " << Instname << "_wd"
                             << " <= " << VariablesName[Op0] << ";"
                             << "\n";
            }
        }
    }
    else if (LoadInst *LI = dyn_cast<LoadInst>(&I))
    {
        std::string Instname = AccessMap[removeExtraSpace(getStringInstruction(I))];
        for (auto &it : ArrayInfo)
        {
            if (it.first == Instname)
            {
                auto Op0 = dyn_cast<Instruction>(LI->getOperand(0));
                *Verilogfile << "                       " << Instname << "_ce0"
                             << " <= "
                             << "1"
                             << ";"
                             << "\n";
                *Verilogfile << "                       " << Instname << "_address0"
                             << " <= " << VariablesName[Op0] << ";"
                             << "\n";
                *Verilogfile << "                       " << VariablesName[&I]
                             << " <= " << Instname << "_d0"
                             << ";"
                             << "\n";
            }
        }
        for (auto &tmp : RAMInfo)
        {
            if (tmp.first == (Instname + "_"))
            {
                auto Op0 = dyn_cast<Instruction>(LI->getOperand(0));
                *Verilogfile << "                       " << Instname << "_ce"
                             << " <= "
                             << "1"
                             << ";"
                             << "\n";
                *Verilogfile << "                       " << Instname << "_raddr"
                             << " <= " << VariablesName[Op0] << ";"
                             << "\n";
                *Verilogfile << "                       " << VariablesName[&I]
                             << " <= " << Instname << "_rd"
                             << ";"
                             << "\n";
            }
        }
    }
    else if (AllocaInst *AI = dyn_cast<AllocaInst>(&I))
    {
        // This operation is handled in Array_Process function
    }
    //////////////////////// Control Operations ///////////////////////////
    else if (PHINode *PHI = dyn_cast<PHINode>(&I))
    {
        //*Verilogfile << PHI->getOpcodeName() << "\n";
        for (int i = 0; i < PHI->getNumIncomingValues(); ++i)
        {
            *Verilogfile << "                   "
                         << "if("
                         << "reg_state == " << Standard_Name(PHI->getIncomingBlock(i)->getName().str()) << "0"
                         << ") "
                         << "\n";
            *Verilogfile << "                       " << Standard_Name(PHI->getValueName()->getValue()->getName().str()) << " = ";
            if (ConstantInt *constval = dyn_cast<ConstantInt>(PHI->getOperand(i)))
                *Verilogfile << constval->getSExtValue() << ";"
                             << "\n";
            else
            {
                auto l_Pred = dyn_cast<Instruction>(PHI->getOperand(i));
                *Verilogfile << VariablesName[l_Pred] << ";"
                             << "\n";
            }
        }
        // if (PHI->getNumOperands() == 1)
        // {
        //     *Verilogfile << "                       " << Standard_Name(PHI->getValueName()->getValue()->getName().str()) << " = " << Standard_Name(PHI->getOperand(0)->getName().str()) << ";"
        //                  << "\n ";
        // }
    }
    else if (BranchInst *BI = dyn_cast<BranchInst>(&I))
    {
        //*Verilogfile << BI->getOpcodeName() << "\n";
        if (BI->getNumOperands() == 3)
        {
            *Verilogfile << "                   "
                         << "if(" << Standard_Name(BI->getOperand(0)->getName().str()) << ") "
                         << "\n";
            *Verilogfile << "                       "
                         << "next_state = " << Standard_Name(BI->getOperand(2)->getName().str()) << "0"
                         << ";"
                         << "\n";
            *Verilogfile << "                   "
                         << "else"
                         << "\n";
            *Verilogfile << "                       "
                         << "next_state = " << Standard_Name(BI->getOperand(1)->getName().str()) << "0"
                         << ";"
                         << "\n";
        }
        if (BI->getNumOperands() == 1)
        {
            *Verilogfile << "                       "
                         << "next_state = " << Standard_Name(BI->getOperand(0)->getName().str()) << "0"
                         << ";"
                         << "\n";
        }
    }
    else if (ReturnInst *RI = dyn_cast<ReturnInst>(&I))
    {
        if (RI->getNumOperands() > 0)
        {
            *Verilogfile << "                       "
                         << "result = " << Standard_Name(RI->getOperand(0)->getName().str()) << ";"
                         << "\n";
        }
    }
    else if (SelectInst *SeI = dyn_cast<SelectInst>(&I))
    {
        *Verilogfile << SeI->getOpcodeName() << "\n";
    }
    else if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(&I))
    {
        // TODO
    }
    else
    {
        *Verilogfile << "// Found undefined instruction!!!" << I
                     << "\n";
    }
}
