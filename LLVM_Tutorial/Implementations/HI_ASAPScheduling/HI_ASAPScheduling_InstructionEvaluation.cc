#include "HI_ASAPScheduling.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <map>

using namespace llvm;

/*
    mainly use to get the latency of an instruction
*/
HI_ASAPScheduling::timingBase HI_ASAPScheduling::getInstructionLatency(Instruction *I)
{
    timingBase result(0, 0, 1, clock_period);

    /////////////////////////////// Cast Operations ///////////////////////////////
    if (PtrToIntInst *PTI = dyn_cast<PtrToIntInst>(I))
    {
        return result;
    }
    else if (IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(I))
    {
        return result;
    }
    else if (FPToUIInst *FTU = dyn_cast<FPToUIInst>(I))
    {
        result = get_inst_TimingInfo_result("fptoui", FTU->getType()->getIntegerBitWidth(), FTU->getType()->getIntegerBitWidth(), clock_period_str);
        return result;
    }
    else if (FPToSIInst *FTS = dyn_cast<FPToSIInst>(I))
    {
        result = get_inst_TimingInfo_result("fptosi", FTS->getType()->getIntegerBitWidth(), FTS->getType()->getIntegerBitWidth(), clock_period_str);
        return result;
    }
    else if (UIToFPInst *UTF = dyn_cast<UIToFPInst>(I))
    {
        Value *op1 = UTF->getOperand(0);
        result = get_inst_TimingInfo_result("uitofp", op1->getType()->getIntegerBitWidth(), op1->getType()->getIntegerBitWidth(), clock_period_str);
        return result;
    }
    else if (SIToFPInst *STF = dyn_cast<SIToFPInst>(I))
    {
        Value *op1 = STF->getOperand(0);
        result = get_inst_TimingInfo_result("sitofp", op1->getType()->getIntegerBitWidth(), op1->getType()->getIntegerBitWidth(), clock_period_str);
        return result;
    }
    else if (ZExtInst *ZEXTI = dyn_cast<ZExtInst>(
                 I)) // such operation like trunc/ext will not cost extra timing on FPGA
    {
        return result;
    }
    else if (SExtInst *SEXTI = dyn_cast<SExtInst>(I))
    {
        return result;
    }
    else if (TruncInst *TI = dyn_cast<TruncInst>(I))
    {
        return result;
    }
    else if (BitCastInst *BI = dyn_cast<BitCastInst>(I))
    {
        return result;
    }

    //////////////////////////////////// Binary Operations ////////////////////////////////////////
    else if (ShlOperator *SHLI = dyn_cast<ShlOperator>(I))
    {
        Value *op1 = SHLI->getOperand(1);

        if (Constant *tmpop = dyn_cast<Constant>(op1))
            return result;
        else
        {
            result = get_inst_TimingInfo_result("shl", op1->getType()->getIntegerBitWidth(), SHLI->getType()->getIntegerBitWidth(), clock_period_str);
            return result;
        }
    }
    else if (LShrOperator *LSHRI = dyn_cast<LShrOperator>(I))
    {
        Value *op1 = LSHRI->getOperand(1);

        if (Constant *tmpop = dyn_cast<Constant>(op1))
            return result;
        else
        {
            result = get_inst_TimingInfo_result("lshr", op1->getType()->getIntegerBitWidth(), LSHRI->getType()->getIntegerBitWidth(), clock_period_str);
            return result;
        }
    }
    else if (AShrOperator *ASHRI = dyn_cast<AShrOperator>(I))
    {
        Value *op1 = ASHRI->getOperand(1);

        if (Constant *tmpop = dyn_cast<Constant>(op1))
            return result;
        else
        {
            result = get_inst_TimingInfo_result("ashr", op1->getType()->getIntegerBitWidth(), ASHRI->getType()->getIntegerBitWidth(), clock_period_str);
            return result;
        }
    }
    else if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(I))
    {
        std::string opcodeInput;
        int operandBitWidth;
        int resBitWidth;
        std::string opcode_str(BinO->getOpcodeName());
        std::transform(opcode_str.begin(), opcode_str.end(), opcode_str.begin(), ::tolower);
        Value *op1 = BinO->getOperand(1);
        if (BinO->getType()->isIntegerTy())
        {
            operandBitWidth = op1->getType()->getIntegerBitWidth();
            resBitWidth = BinO->getType()->getIntegerBitWidth();
        }
        else
        {
            operandBitWidth = -1;
            resBitWidth = -1;
            // For floating operator, we need to consider whether it is a operator for float value or double value
            if (BinO->getType()->isDoubleTy() && opcode_str[0] == 'f')
                opcode_str[0] = 'd';
        }

        // Check Add for IntToPtr
        if (BinO->getOpcode() == Instruction::Add)
        {
            for (auto it = BinO->use_begin(), ie = BinO->use_end(); it != ie; ++it)
            {
                User *tmp_user = it->getUser();
                if (Instruction *UserI = dyn_cast<Instruction>(tmp_user))
                {
                    if (UserI->getOpcode() == Instruction::IntToPtr)
                    {
                        return result;
                    }
                }
            }
            result =
                get_inst_TimingInfo_result(opcode_str, operandBitWidth, resBitWidth, clock_period_str);
            return result;
        }
    }
    else if (ICmpInst *ICI = dyn_cast<ICmpInst>(I))
    {
        int operandBitWidth;
        int resBitWidth;
        Value *op1 = ICI->getOperand(1);
        operandBitWidth = op1->getType()->getIntegerBitWidth();
        resBitWidth = operandBitWidth;
        result = get_inst_TimingInfo_result("icmp", operandBitWidth, resBitWidth, clock_period_str);
        return result;
    }
    else if (FCmpInst *FI = dyn_cast<FCmpInst>(I))
    {
        result = get_inst_TimingInfo_result("fcmp", -1, -1, clock_period_str);
        return result;
    }

    ///////////////////////////////////////////// Memory Operations //////////////////////////////////
    else if (StoreInst *SI = dyn_cast<StoreInst>(I))
    {
        result = get_inst_TimingInfo_result("store", -1, -1, clock_period_str);
        result.strict_timing = true;
        return result;
    }
    else if (LoadInst *LI = dyn_cast<LoadInst>(I))
    {
        result = get_inst_TimingInfo_result("load", -1, -1, clock_period_str);
        return result;
    }
    else if (AllocaInst *AI = dyn_cast<AllocaInst>(I))
    {
        return result;
    }

    //////////////////////////////////////////// Control Operation ////////////////////////////////////
    else if (PHINode *PHI = dyn_cast<PHINode>(I))
    {
        int num_Block = PHI->getNumOperands();
        for (int i = 0; i < num_Block; i++)
        {
            BasicBlock *tmpB = PHI->getIncomingBlock(i);
            if (tmpB == PHI->getParent())
                return result;
        }
        result.latency = 0;
        result.timing = 0.1;
        return result;
    }
    else if (CallInst *CI = dyn_cast<CallInst>(I))
    {
        if (CI->getCalledFunction()->getName().find("HIPartitionMux") != std::string::npos)
        {
            auto partitionConst = dyn_cast<ConstantInt>(CI->getArgOperand(1));
            int partitionVal = partitionConst->getValue().getSExtValue();
            *Evaluating_log << " handling mux with " << partitionVal << " inputs.\n";
            if (partitionVal == 2)
                result.timing = 1.8;
            else if (partitionVal == 4)
                result.timing = 1.95;
            else if (partitionVal == 8)
                result.timing = 2;
            else if (partitionVal == 16)
                result.timing = 2.19;
            else if (partitionVal == 32)
                result.timing = 2.73;
            else if (partitionVal == 64)
                result.timing = 3.35;
            else
            {
                result.timing = 4.1;
                print_warning("using undefined partition factor and the mux delay for it is unknown. Set delay = 4.1ns for it.");
            }
            return result;
        }
        *Evaluating_log << " Going into subfunction: " << CI->getCalledFunction()->getName() << "\n";
        result = analyzeFunction(CI->getCalledFunction());
        return result;
    }
    else if (BranchInst *BI = dyn_cast<BranchInst>(I))
    {
        result = get_inst_TimingInfo_result("br", -1, -1, clock_period_str);
        return result;
    }
    else if (ReturnInst *RI = dyn_cast<ReturnInst>(I))
    {
        result = get_inst_TimingInfo_result("ret", -1, -1, clock_period_str);
        return result;
    }
    else if (SelectInst *SeI = dyn_cast<SelectInst>(I))
    {
        result.timing = 1.0;
        return result;
    }
    else if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(I))
    {
        result = get_inst_TimingInfo_result("getelementptr", -1, -1, clock_period_str);
        return result;
    }
    else
    {
        llvm::errs() << *I << "\n";
        assert(false && "The instruction is not defined.");
    }
    result.latency = 1;
    return result;
}

bool HI_ASAPScheduling::canChainOrNot(Instruction *PredI, Instruction *I)
{
    if (isMACpossible(PredI, I))
    {
        return true;
    }
    if (isAMApossible(PredI, I))
    {
        return true;
    }
    return false;
}

bool HI_ASAPScheduling::isAMApossible(Instruction *PredI, Instruction *I)

{
    // for the GEP MAA, consider to transform it into AMA
    if (I->getOpcode() == Instruction::Add)
    {
        Instruction *ori_PredI = dyn_cast<Instruction>(byPassBitcastOp(PredI));
        if (!ori_PredI)
            return false;
        if (ori_PredI->getOpcode() == Instruction::Add)
        {
            if (auto Pred_Pred_I = dyn_cast<Instruction>(ori_PredI->getOperand(0)))
            {
                Instruction *ori_Pred_Pred_I = dyn_cast<Instruction>(byPassBitcastOp(Pred_Pred_I));
                if (!ori_Pred_Pred_I)
                    return false;
                if (ori_Pred_Pred_I->getOpcode() == Instruction::Mul)
                {
                    Value *op0 = (ori_Pred_Pred_I->getOperand(0));
                    Value *op1 = (ori_Pred_Pred_I->getOperand(1));
                    if (auto Pred_Pred_I_const =
                            dyn_cast<ConstantInt>(ori_Pred_Pred_I->getOperand(1)))
                    {
                        if (auto I_const = dyn_cast<ConstantInt>(I->getOperand(1)))
                        {
                            long long add_const = (I_const->getValue().getSExtValue());
                            long long mul_const = (Pred_Pred_I_const->getValue().getSExtValue());
                            if (add_const % mul_const == 0)
                            {
                                return (getOriginalBitwidth(op0) <= 18) &&
                                       (getOriginalBitwidth(op1) <= 18) &&
                                       (ori_Pred_Pred_I->getType()->getIntegerBitWidth() <= 48);
                            }
                        }
                    }
                    else
                    {
                        if (auto Pred_Pred_I_op1 =
                                dyn_cast<Instruction>(ori_Pred_Pred_I->getOperand(1)))
                        {
                            if (auto I_op1 = dyn_cast<Instruction>(I->getOperand(1)))
                            {
                                if (I_op1 == Pred_Pred_I_op1)
                                {
                                    return (getOriginalBitwidth(op0) <= 18) &&
                                           (getOriginalBitwidth(op1) <= 18) &&
                                           (ori_Pred_Pred_I->getType()->getIntegerBitWidth() <= 48);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool HI_ASAPScheduling::isMACpossible(Instruction *PredI, Instruction *I)
{
    if (I->getOpcode() == Instruction::Add)
    {
        if (PredI->getOpcode() == Instruction::Mul)
        {
            Value *op0 = (PredI->getOperand(0));
            Value *op1 = (PredI->getOperand(1));
            if (op0 && op1 && getActualUsersNumm(PredI, 0) < 2)
            {
                return (getOriginalBitwidth(op0) <= 18) && (getOriginalBitwidth(op1) <= 18) && (I->getType()->getIntegerBitWidth() <= 48);
            }
        }
        else if (PredI->getOpcode() == Instruction::Trunc ||
                 PredI->getOpcode() == Instruction::SExt || PredI->getOpcode() == Instruction::ZExt)
        {
            Instruction *Pred_Pred_I = dyn_cast<Instruction>(PredI->getOperand(0));
            if (Pred_Pred_I)
            {
                if (Pred_Pred_I->getOpcode() == Instruction::Mul)
                {
                    Value *op0 = (Pred_Pred_I->getOperand(0));
                    Value *op1 = (Pred_Pred_I->getOperand(1));
                    if (op0 && op1 && getActualUsersNumm(Pred_Pred_I, 0) < 2)
                    {
                        return (getOriginalBitwidth(op0) <= 18) &&
                               (getOriginalBitwidth(op1) <= 18) &&
                               (I->getType()->getIntegerBitWidth() <= 48);
                    }
                }
            }
        }
    }
    return false;
}

// Trace back to get the bitwidth of an operand, bypassing truct/zext/sext
int HI_ASAPScheduling::getOriginalBitwidth(Value *Val)
{
    if (Instruction *I = dyn_cast<Instruction>(Val))
    {
        if (I->getOpcode() == Instruction::Trunc || I->getOpcode() == Instruction::SExt || I->getOpcode() == Instruction::ZExt)
            return getOriginalBitwidth(I->getOperand(0));
        else
            return I->getType()->getIntegerBitWidth();
    }
    else
        return Val->getType()->getIntegerBitWidth();
}

int HI_ASAPScheduling::getActualUsersNumm(Instruction *I, int dep)
{
    std::string cur_opcode = I->getOpcodeName();
    if (dep == 0 || I->getOpcode() == Instruction::Trunc || I->getOpcode() == Instruction::SExt || I->getOpcode() == Instruction::ZExt)
    {
        int num = 0;
        for (auto it = I->use_begin(), ie = I->use_end(); it != ie; ++it)
        {
            User *tmp_user = it->getUser();
            if (Instruction *tmpI = dyn_cast<Instruction>(tmp_user))
                num += getActualUsersNumm(tmpI, dep + 1);
        }
        return num;
    }
    else
    {
        return 1;
    }
}

// Trace back to find the original operator, bypassing SExt and ZExt operations
Instruction *HI_ASAPScheduling::byPassUnregisterOp(Instruction *cur_I)
{
    if (cur_I->getOpcode() == Instruction::ZExt || cur_I->getOpcode() == Instruction::SExt)
    {
        if (auto next_I = dyn_cast<Instruction>(cur_I->getOperand(0)))
        {
            return byPassUnregisterOp(next_I);
        }
        else
        {
            assert(false && "Predecessor of bitcast operator should be found.\n");
        }
    }
    else
    {
        return cur_I;
    }
}

// trace back to find the original operator, bypassing SExt and ZExt operations
Value *HI_ASAPScheduling::byPassBitcastOp(Instruction *cur_I)
{

    // For ZExt/SExt Instruction, we do not need to consider those constant bits
    if (/*cur_I->getOpcode() == Instruction::Trunc || */ cur_I->getOpcode() == Instruction::ZExt ||
        cur_I->getOpcode() == Instruction::SExt)
    {
        if (auto next_I = dyn_cast<Instruction>(cur_I->getOperand(0)))
        {
            return byPassBitcastOp(next_I);
        }
        else
        {
            if (auto next_Arg = dyn_cast<Argument>(cur_I->getOperand(0)))
            {
                return next_Arg;
            }
            llvm::errs() << "error from instruction: " << *cur_I << "\n";
            llvm::errs() << "In function: " << *(cur_I->getParent()->getParent()) << "\n";
            assert(false && "Predecessor of bitcast operator should be found.\n");
        }
    }
    else
    {
        return cur_I;
    }
}

// trace back to find the original operator, bypassing SExt and ZExt operations
Value *HI_ASAPScheduling::byPassBitcastOp(Value *cur_I_value)
{
    auto cur_I = dyn_cast<Instruction>(cur_I_value);
    if (auto arg = dyn_cast<Argument>(cur_I_value))
        return cur_I_value;
    if (!cur_I)
    {
        return cur_I_value;
        llvm::errs() << *cur_I_value << "\n";
    }
    assert(cur_I && "This should be an instruction.\n");
    // For ZExt/SExt Instruction, we do not need to consider those constant bits
    if (/*cur_I->getOpcode() == Instruction::Trunc || */ cur_I->getOpcode() == Instruction::ZExt ||
        cur_I->getOpcode() == Instruction::SExt)
    {
        if (auto next_I = dyn_cast<Instruction>(cur_I->getOperand(0)))
        {
            return byPassBitcastOp(next_I);
        }
        else
        {
            if (auto next_Arg = dyn_cast<Argument>(cur_I->getOperand(0)))
            {
                return next_Arg;
            }
            assert(false && "Predecessor of bitcast operator should be found.\n");
        }
    }
    else
    {
        return cur_I;
    }
}
