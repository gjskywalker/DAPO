#include "HI_ASAPScheduling.h"
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

/*
    Evaluate the latency of the block
    (1) Iterate the instructions in the block
    (2) Check the CP to the instruction's predecessors and find the maximum one to update the CP
    (3) Get the maximum CP among instructions and take it as the CP of block
*/
HI_ASAPScheduling::timingBase HI_ASAPScheduling::BlockLatencyEvaluation(BasicBlock *B)
{
    *Evaluating_log << "---- Evaluating Block Latency for Block: " << B->getName() << ":\n";
    if (BlockLatency.find(B) != BlockLatency.end())
    {
        *Evaluating_log << "---- Done evaluation of Block Latency for Block: " << B->getName()
                        << " and the latency is " << BlockLatency[B] << "\n";
        return BlockLatency[B] * 1;
    }
    // A container records the critical path from the block entry to specific instruction
    std::map<Instruction *, timingBase> cur_InstructionCriticalPath;

    // initialize the timing statistics
    timingBase max_critical_path(0, 0, 1, clock_period);
    timingBase origin_path(0, 0, 1, clock_period);

    // (1) iterate the instructions in the block
    if (B->sizeWithoutDebug() > 1) // Becareful! This is not evaluated whether it's the same meaning to getInstList()
    // if (B->getInstList().size() > 1) // ignore block with only branch instruction
    {
        for (Instruction &rI : *B)
        {
            Instruction *I = &rI;
            timingBase tmp_I_latency = getInstructionLatency(I);
            cur_InstructionCriticalPath[I] = origin_path + tmp_I_latency;
            bool Chained = 0;

            // (2) check the CP to the instruction's predecessors and find the maximum one to update
            // its CP
            //     but Store/Load operation should be scheduled specially due to the limited number
            //     of BRAM ports

            timingBase latest_timing(0, 0, 1, clock_period);
            // for instructions, we find the latest-finished operand of them
            for (User::op_iterator I_tmp = I->op_begin(), I_Pred_end = I->op_end();
                 I_tmp != I_Pred_end; ++I_tmp) // update the critical path to I by checking its
                                               // predecessors' critical path
            {
                if (auto I_Pred = dyn_cast<Instruction>(I_tmp))
                {
                    // ensure that the predecessor is in the block and before I, considering some
                    // predecessors may be located behind the instruction itself (not in
                    // cur_InstructionCriticalPath yet) in some loop structures
                    if (BlockContain(B, I_Pred) && cur_InstructionCriticalPath.find(I_Pred) !=
                                                       cur_InstructionCriticalPath.end())
                    {
                        if (canChainOrNot(I_Pred, I))
                        {
                            // TODO: may need to rethink the machanism carefully
                            // *Evaluating_log << "        --------- Evaluated Instruction critical
                            // path for Instruction: <<" << *I << " which can be chained.\n";
                            if (cur_InstructionCriticalPath[I_Pred] >
                                cur_InstructionCriticalPath[I]) // addition chained with
                                                                // multiplication
                            {
                                cur_InstructionCriticalPath[I] =
                                    cur_InstructionCriticalPath[I_Pred];
                                latest_timing = cur_InstructionCriticalPath[I_Pred];
                                Inst2LatestOperand[I] = I_Pred;
                            }
                            Chained = 1;
                        }
                        else
                        {
                            // *Evaluating_log << "        --------- Evaluated Instruction critical
                            // path for Instruction: <<" << *I << " which cannot be chained.\n";
                            if (cur_InstructionCriticalPath[I_Pred] + tmp_I_latency >
                                cur_InstructionCriticalPath[I]) // update the critical path
                            {
                                cur_InstructionCriticalPath[I] =
                                    cur_InstructionCriticalPath[I_Pred] + tmp_I_latency;
                                latest_timing = cur_InstructionCriticalPath[I_Pred];
                                Inst2LatestOperand[I] = I_Pred;
                            }
                        }
                    }
                }
            }

            // record where the instruction is scheduled
            Inst_Schedule[I] = std::pair<BasicBlock *, int>(
                B, (cur_InstructionCriticalPath[I] - getInstructionLatency(I)).latency);

            // (3) get the maximum CP among instructions and take it as the CP of block
            if (cur_InstructionCriticalPath[I] > max_critical_path)
                max_critical_path = cur_InstructionCriticalPath[I];
            *Evaluating_log << "--------- Evaluated Instruction critical path for Instruction: <<"
                            << *I << ">> and its CP is :" << cur_InstructionCriticalPath[I];
            if (Chained)
                *Evaluating_log << "(Chained))";
            else
                *Evaluating_log << "(Not Chained)";

            *Evaluating_log << "\n";
            Evaluating_log->flush();
        }
    }
    else
    {
        for (Instruction &rI : *B)
        {
            Instruction *I = &rI;
            Inst_Schedule[I] = std::pair<BasicBlock *, int>(B, 0);
        }
    }

    InstructionCriticalPath_inBlock[B] = cur_InstructionCriticalPath;

    BlockLatency[B] = max_critical_path;
    *Evaluating_log << "---- Done evaluation of Block Latency for Block: " << B->getName()
                    << " and the latency is " << BlockLatency[B];
    BlockEvaluated.insert(B);
    return max_critical_path * 1;
}

/*
    Mainly used to ensure that the predecessor is in the block
*/
bool HI_ASAPScheduling::BlockContain(BasicBlock *B, Instruction *I)
{
    return I->getParent() == B;
}

/*
    Get the latency of functions in the path to the instruction
*/
int HI_ASAPScheduling::getFunctionLatencyInPath(Instruction *I)
{
    int res = 0;

    if (CallInst *callI = dyn_cast<CallInst>(I))
    {
        res = FunctionLatency[callI->getCalledFunction()].latency;
    }

    if (Inst2LatestOperand.find(I) == Inst2LatestOperand.end())
        return res;

    Instruction *preI = Inst2LatestOperand[I];

    if (preI->getParent() != I->getParent())
        return res;

    return res + getFunctionLatencyInPath(preI);
}

// Get the number of stage arrive the instruction
int HI_ASAPScheduling::getStageTo(Instruction *I)
{
    assert(Inst_Schedule.find(I) != Inst_Schedule.end());
    int instruction_latency = getInstructionLatency(I).latency;
    if (instruction_latency == -1)
        return Inst_Schedule[I].second + 1 - getFunctionLatencyInPath(I);
    else
        return Inst_Schedule[I].second + instruction_latency - getFunctionLatencyInPath(I);
}

// Get the number of stage in the block
int HI_ASAPScheduling::getStageNumOfBlock(BasicBlock *B)
{
    *Evaluating_log << "Checking BLOCK:\n"
                    << *B << "\n";

    int max_stage_num = -1;
    if (BlockLatency[B].latency == 0 && BlockLatency[B].timing < 0.00001)
        return 0;
    for (auto &I : *B)
    {
        *Evaluating_log << "checking instruction: [" << I << "]\n";
        int stage_to_inst = getStageTo(&I);
        if (stage_to_inst > max_stage_num)
            max_stage_num = stage_to_inst;
    }
    *Evaluating_log << "checking BLOCK: " << B->getName() << " #stage=" << max_stage_num << "\n";
    assert(max_stage_num >= 0);
    return max_stage_num;
}