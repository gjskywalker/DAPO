#ifndef EXTRANUM
#define EXTRANUM

#include <map>
#include <set>
#include <string>
#include "llvm/Analysis/Passes.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/IR/CFG.h"
#include "HI_print.h"

namespace llvm {
    class HI_ExtraNum : public ModulePass{
        public:
        HI_ExtraNum() : ModulePass(ID) {}
        void visitFunction(Function &F);
        void visitBasicBlock(BasicBlock &BB);
        void visitInstruction(Instruction &I);

        static char ID; // Pass identification, replacement for typeid

        bool runOnModule(Module &M);

        void getAnalysisUsage(AnalysisUsage &AU) const;

        virtual bool doInitialization(Module &M)
        {
            print_status("Initilizing HI_EtraNum pass.");
            return false;
        }
        
        std::map<const char*, int> Instcount;

        int TotalInsts  = 0;
        int TotalBlocks = 0;
        int BlockLow = 0;
        int BlockMid = 0;
        int BlockHigh = 0;
        int TotalFuncs = 0;
        int TotalMemInst = 0;
        int BeginPhi = 0;
        int ArgsPhi = 0;
        int BBNoPhi = 0;
        int BB03Phi = 0;
        int BBNumArgsHi = 0;
        int BBNumArgsLo = 0;
        int testUnary = 0;
        int binaryConstArg = 0;
        int callLargeNumArgs = 0;
        int returnInt = 0;
        int oneSuccessor = 0;
        int twoSuccessors = 0;
        int moreSuccessors = 0;
        int onePred = 0;
        int twoPred = 0;
        int morePreds = 0;
        int onePredOneSuc = 0;
        int onePredTwoSuc = 0;
        int tweEach = 0;
        int moreEach = 0;
        int NumEdges = 0;
        int CriticalCount = 0;
        int BranchCount = 0;
        int numConstOnes = 0;
        int numConstZeros = 0;
        int const32Bit = 0;
        int const64Bit = 0;
        int BBHiPhi = 0;
        int twoPredOneSuc = 0;
        int twoEach = 0;
        int UncondBranches = 0;

        void print_result();
        struct timeval tv_begin;
        struct timeval tv_end;
    };
}

#endif