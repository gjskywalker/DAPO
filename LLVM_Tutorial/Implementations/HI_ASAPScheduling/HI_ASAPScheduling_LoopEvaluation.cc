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

bool HI_ASAPScheduling::isInLoop(BasicBlock *BB)
{
    return (Block2Loops.find(BB) != Block2Loops.end());
}

/*
    Find the outer loop and evaluate it as a integration
*/
Loop *HI_ASAPScheduling::getOuterLoopOfBlock(BasicBlock *B)
{
    for (auto tmp_Loop : *Block2Loops[B])
    {
        if (tmp_Loop->getLoopDepth() == 1)
        {
            return tmp_Loop;
        }
    }
    assert(false && "a loop should be found but actually not");
}

void HI_ASAPScheduling::getLoopBlockMap(Function *F)
{
    *Evaluating_log << "    getLoopBlockMap for Function : " << F->getName() << "\n";
    for (auto ele : Loop2Blocks)
    {
        delete ele.second;
    }
    for (auto ele : Block2Loops)
    {
        delete ele.second;
    }
    Loop2Blocks.clear();
    Block2Loops.clear();
    for (LoopInfo::iterator i = LI->begin(), e = LI->end(); i != e; ++i)
    {
        Loop *L = *i;
        *Evaluating_log << "----------- Loop: " << L->getName() << " address: " << L->getHeader() << " contains:\n";
        for (auto BinL : L->getBlocks())
        {
            *Evaluating_log << "------------ Block: " << BinL->getName() << " address: " << BinL << "\n";
            std::vector<BasicBlock *> *tmp_vec_block;
            std::vector<Loop *> *tmp_vec_loop;

            if (Block2Loops.find(BinL) == Block2Loops.end())
            {
                tmp_vec_loop = new std::vector<Loop *>;
                Block2Loops[BinL] = tmp_vec_loop;
            }
            else
            {
                tmp_vec_loop = Block2Loops[BinL];
            }
            if (Loop2Blocks.find(L) == Loop2Blocks.end())
            {
                tmp_vec_block = new std::vector<BasicBlock *>;
                Loop2Blocks[L] = tmp_vec_block;
            }
            else
            {
                tmp_vec_block = Loop2Blocks[L];
            }
            tmp_vec_block->push_back(BinL);
            tmp_vec_loop->push_back(L);
        }
    }
}

/*
    find the inner unevaluated loop,
    (1) check all the sub-loops
    (2) check the loop itself
*/
Loop *HI_ASAPScheduling::getInnerUnevaluatedLoop(Loop *outerL)
{
    int dep = 0;
    Loop *tmp_inner_Loop = NULL;
    for (auto tmp_Loop : *outerL) // find the most inner unevaluated loop
    {
        *Evaluating_log << "--------- Checking sub-loop: " << tmp_Loop->getName()
                        << " address:" << tmp_Loop->getHeader() << " -> dep = " << tmp_Loop->getLoopDepth() << " ";
        if (LoopEvaluated.find(tmp_Loop->getHeader()) != LoopEvaluated.end())
            *Evaluating_log << " which is evaluated.\n";
        else
            *Evaluating_log << " which is NOT evaluated.\n";
        // Larger depth means more inner

        // Go to the sub-sub-……-Loop to have a check
        Loop *tmp_inner_Sub_Loop = getInnerUnevaluatedLoop(tmp_Loop);
        if (tmp_inner_Sub_Loop)
        {
            if (tmp_inner_Sub_Loop->getLoopDepth() > dep && LoopEvaluated.find(tmp_inner_Sub_Loop->getHeader()) == LoopEvaluated.end())
            {
                dep = tmp_inner_Sub_Loop->getLoopDepth();
                tmp_inner_Loop = tmp_inner_Sub_Loop;
                *Evaluating_log << "--------- update target sub-loop to Loop: " << tmp_inner_Loop->getName() << "\n";
            }
        }
        else
        {
            if (tmp_Loop->getLoopDepth() > dep && LoopEvaluated.find(tmp_Loop->getHeader()) == LoopEvaluated.end())
            {
                dep = tmp_Loop->getLoopDepth();
                tmp_inner_Loop = tmp_Loop;
                *Evaluating_log << "----------- update target sub-loop to loop: " << tmp_inner_Loop->getName() << "\n";
            }
        }
    }
    auto tmp_Loop = outerL;
    if (tmp_inner_Loop == NULL)
    {
        *Evaluating_log << "------ checking loop itself: " << tmp_Loop->getName() << " address:" << tmp_Loop->getHeader() << " -> dep = " << tmp_Loop->getLoopDepth() << " ";
        if (LoopEvaluated.find(tmp_Loop->getHeader()) != LoopEvaluated.end())
            *Evaluating_log << " which is evaluated.\n";
        else
            *Evaluating_log << " which is NOT evaluated.\n";

        // Larger depth means more inner
        if (tmp_Loop->getLoopDepth() > dep && LoopEvaluated.find(tmp_Loop->getHeader()) == LoopEvaluated.end())
        {
            dep = tmp_Loop->getLoopDepth();
            tmp_inner_Loop = tmp_Loop;
            *Evaluating_log << "---------- update target loop to Loop: " << tmp_inner_Loop->getName() << "\n";
        }
    }
    return tmp_inner_Loop;
}

/*
    To get the latency of the entire outer loops,
    (1) iteratively handle the most inner loop,
    (2) traverse the blocks in loops by DFS to find the longest path
    (3) get the total latency by TripCount * IterationLatency
    (4) mark the blocks in loop with the loop latency, so later processing can regard this liio as an integration
*/
HI_ASAPScheduling::timingBase
HI_ASAPScheduling::analyzeOuterLoop(Loop *outerL)
{
    *Evaluating_log << "\n Evaluating Outer Loop Latency for Loop " << outerL->getName() << ":\n";
    *Evaluating_log << "outerL->getHeader() == " << outerL->getHeader() << "\n";
    if (LoopLatency.find(outerL->getHeader()) != LoopLatency.end())
    {
        *Evaluating_log << "Done evaluation outer Loop Latency for Loop " << outerL->getName() << " and its latency is "
                        << LoopLatency[outerL->getHeader()] << " cycles.\n\n\n";
        return LoopLatency[outerL->getHeader()];
    }
    Loop *cur_Loop;
    timingBase outerL_latency(-1, -1, 1, clock_period);
    timingBase tmp_total_latency(0, 0, 1, clock_period);
    timingBase original_latency(0, 0, 1, clock_period);
    // (1) iteratively handle the most inner loop
    cur_Loop = getInnerUnevaluatedLoop(outerL);
    while (cur_Loop != NULL)
    {
        *Evaluating_log << "-- Handling the inner Loop " << cur_Loop->getName() << ":\n";
        BasicBlock *tmp_LoopHeader = cur_Loop->getHeader(); // Get the header of the loop
        *Evaluating_log << "---- its header: " << tmp_LoopHeader->getName() << ":\n";
        SmallVector<BasicBlock *, 8> tmp_ExitingBlocks;
        cur_Loop->getExitingBlocks(tmp_ExitingBlocks);
        if (tmp_ExitingBlocks.size() != 1)
        {
            assert(tmp_ExitingBlocks.size() > 0);
            print_warning("the loop could be better to have only one exiting block for the "
                          "accuracy of latency evaluation.");
        }
        for (auto B_it : tmp_ExitingBlocks)
        {
            *Evaluating_log << "---- its exiting Block(s): " << B_it->getName() << " -- ";
        }
        *Evaluating_log << "\n";

        // (2) traverse the block in loop by DFS to find the longest path
        timingBase max_critical_path_in_curLoop(0, 0, 1, clock_period);
        tmp_BlockCriticalPath_inLoop.clear(); // record the block level critical path in the loop
        tmp_SubLoop_CriticalPath.clear();     // record the critical path to the end of sub-loops in the loop

        BlockVisited.clear();
        LoopLatencyResourceEvaluation_traversFromHeaderToExitingBlocks(original_latency, cur_Loop, tmp_LoopHeader);

        BlockCriticalPath_inLoop[cur_Loop] = tmp_BlockCriticalPath_inLoop;

        for (auto tmp_it : tmp_BlockCriticalPath_inLoop)
            if (tmp_it.second > max_critical_path_in_curLoop)
                max_critical_path_in_curLoop = tmp_it.second;
        for (auto tmp_it : tmp_SubLoop_CriticalPath)
            if (tmp_it.second > max_critical_path_in_curLoop)
                max_critical_path_in_curLoop = tmp_it.second;

        // (3) get the total latency by TripCount * IterationLatency
        tmp_total_latency = SE->getSmallConstantMaxTripCount(cur_Loop) * max_critical_path_in_curLoop;

        // COMMENT because preheader is not in the loop unity and if the preheader is calculated,
        // it is actually duplicated calculation. But just need to add one cycle, as it seems that
        // in VivadoHLS, Loops are regarded as function and the "Call" will take one cycle
        // if(cur_Loop->getLoopPreheader())
        //         tmp_total_latency = tmp_total_latency + BlockLatencyEvaluation(cur_Loop->getLoopPreheader());
        tmp_total_latency = tmp_total_latency + timingBase(1, 0, 1, clock_period);

        // (4) Mark the blocks in loop with the loop latency, so later processing can regard this loop as an integration
        BlockVisited.clear();
        MarkBlock_traversFromHeaderToExitingBlocks(tmp_total_latency, cur_Loop, tmp_LoopHeader);
        LoopLatency[cur_Loop->getHeader()] = tmp_total_latency;

        *Evaluating_log << "inserted Loop Evaluated Address: " << cur_Loop->getHeader() << "\n";
        LoopEvaluated.insert(cur_Loop->getHeader());
        *Evaluating_log << "Trip Count for Loop " << cur_Loop->getHeader() << " is " << SE->getSmallConstantMaxTripCount(cur_Loop) << "\n";
        *Evaluating_log << "Done evaluation Loop Latency for Loop " << cur_Loop->getName() << " and its latency is " << tmp_total_latency
                        << " cycles. \n\n";
        Evaluating_log->flush();
        cur_Loop = getInnerUnevaluatedLoop(outerL);
    }
    outerL_latency = tmp_total_latency; // finally, we will get the latency of outer loop in the last iteration
    *Evaluating_log << "Done evaluation outer Loop Latency for Loop " << outerL->getName() << " and its latency is "
                    << outerL_latency
                    << ".\n\n\n";
    Evaluating_log->flush();
    assert(outerL_latency.latency > -0.5 && "The latency for a loop should be not be negative");
    return outerL_latency * 1;
}

/*
    traverse the block in loop by DFS to find the longest path:
    (1) Mark the block visited, as a step of typical DFS;
    (2) Check whether the search reaches a block in the sub-loops;
    (3a) -- If it is a block in sub-loops, regard the loop as intergration and update the critical path if necessary (max(ori_CP, lastStateCP + LoopLatency));
        -- Find the successors of the loop by checking its exiting blocks' successors and continue the DFS
    (3b) -- If it is a block out of sub-loops, evaluate the block latency and update the critical path if necessary (max(ori_CP, lastStateCP + BlockLatency));
        -- Find the successors of the block and continue the DFS
    (4) Release the block from visited flag, as a step of typical DFS
*/
void HI_ASAPScheduling::LoopLatencyResourceEvaluation_traversFromHeaderToExitingBlocks(timingBase tmp_critical_path, Loop *L, BasicBlock *curBlock)
{
    // (1) Mark the block visited, as a step of typical DFS
    BlockVisited.insert(curBlock);

    *Evaluating_log << "--- traverser arrive Block: " << curBlock->getName() << " ";

    // (2) Check whether the search reaches a block in the sub-loops
    if (Block2EvaluatedLoop.find(curBlock) != Block2EvaluatedLoop.end())
    {
        // (3a) -- If it is a block in sub-loops, regard the loop as integration and update the critical path if necessary (max(ori_CP, lastStateCP + LoopLatency)).
        Loop *tmp_SubLoop = Block2EvaluatedLoop[curBlock];
        *Evaluating_log << " which is evaluated in Loop " << tmp_SubLoop->getName() << " ";
        *Evaluating_log << " LoopLatency = " << LoopLatency[tmp_SubLoop->getHeader()] << " ";
        timingBase try_critical_path = tmp_critical_path + LoopLatency[tmp_SubLoop->getHeader()]; // first, get the ctitical path to the end of sub-loop

        *Evaluating_log << " NewCP = " << try_critical_path << " ";
        bool checkflag = false;

        if (tmp_SubLoop_CriticalPath.find(tmp_SubLoop) == tmp_SubLoop_CriticalPath.end())
        {
            checkflag = true;
        }
        else if (try_critical_path > tmp_SubLoop_CriticalPath[tmp_SubLoop])
            checkflag = true;

        if (checkflag)
        {
            if (tmp_SubLoop_CriticalPath.find(tmp_SubLoop) != tmp_SubLoop_CriticalPath.end())
            {
                *Evaluating_log << "   OriCP = " << tmp_SubLoop_CriticalPath[tmp_SubLoop] << "\n";
            }
            else
            {
                *Evaluating_log << " No OriCP"
                                << "\n";
            }
            tmp_SubLoop_CriticalPath[tmp_SubLoop] = try_critical_path;

            // (3a) -- find the succesors of the loop by checking its exiting blocks' successors and continue the DFS
            SmallVector<BasicBlock *, 8> tmp_SubLoop_ExitingBlocks;
            tmp_SubLoop->getExitingBlocks(tmp_SubLoop_ExitingBlocks);
            for (auto ExitB : tmp_SubLoop_ExitingBlocks)
            {
                for (auto B : successors(ExitB))
                {
                    if (L->contains(B) && BlockVisited.find(B) == BlockVisited.end() && !tmp_SubLoop->contains(B))
                    {
                        *Evaluating_log << "---- continue to traverser to Block: " << B->getName() << " ";
                        LoopLatencyResourceEvaluation_traversFromHeaderToExitingBlocks(try_critical_path, L, B);
                    }
                }
            }
        }
    }
    else
    {
        // (3b) -- If it is a block out of sub-loops, evaluate the block latency and update the critical path if necessary (max(ori_CP, latestStateCP + BlockLatency)).
        *Evaluating_log << " which is not evaluated in Loop "
                        << " ";
        timingBase latency_CurBlock = BlockLatencyEvaluation(curBlock); // first, get the latency of the current block.
        timingBase try_critical_path = tmp_critical_path + latency_CurBlock;
        *Evaluating_log << "---- latency Block = " << latency_CurBlock << " ";
        *Evaluating_log << " NewCP = " << try_critical_path << " ";
        bool checkFlag = false;

        if (tmp_BlockCriticalPath_inLoop.find(curBlock) == tmp_BlockCriticalPath_inLoop.end())
        {
            checkFlag = true;
        }
        else if (try_critical_path > tmp_BlockCriticalPath_inLoop[curBlock])
            checkFlag = true;

        if (checkFlag) // update the block-level critical path
        {
            if (tmp_BlockCriticalPath_inLoop.find(curBlock) != tmp_BlockCriticalPath_inLoop.end())
            {
                *Evaluating_log << " OriCP =  " << tmp_BlockCriticalPath_inLoop[curBlock] << "\n";
            }
            else
            {
                *Evaluating_log << " No OriCP"
                                << "\n";
            }

            tmp_BlockCriticalPath_inLoop[curBlock] = try_critical_path;

            // (3b)  -- find the successors of the block and continue the DFS
            for (auto B : successors(curBlock))
            {
                if (L->contains(B) && BlockVisited.find(B) == BlockVisited.end())
                {
                    *Evaluating_log << "---- continue to traverser from Block: " << curBlock->getName()
                                    << " to Block: " << B->getName() << " ";
                    LoopLatencyResourceEvaluation_traversFromHeaderToExitingBlocks(try_critical_path, L, B);
                }
            }
        }
    }
    // (4) Release the block from visited flag, as a step of typical DFS
    BlockVisited.erase(curBlock);
}

/*
    Simply mark all the blocks in the loop with the totoal_latency by DFS-traverse
*/
void HI_ASAPScheduling::MarkBlock_traversFromHeaderToExitingBlocks(
    HI_ASAPScheduling::timingBase total_latency, Loop *L, BasicBlock *curBlock)
{
    BlockVisited.insert(curBlock);
    Block2EvaluatedLoop[curBlock] = L;

    for (auto B : successors(curBlock))
    {
        if (L->contains(B) && BlockVisited.find(B) == BlockVisited.end())
        {
            MarkBlock_traversFromHeaderToExitingBlocks(total_latency, L, B);
        }
    }
}
