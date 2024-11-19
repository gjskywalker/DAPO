//===-- HI_ExtraNum.cpp - Collects the count of all instructions ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This pass collects the count of all instructions and reports them
//
//===----------------------------------------------------------------------===//

#include "HI_ExtraNum.h"
using namespace llvm;

void HI_ExtraNum::print_result()
{
	errs() << "Number of instructions (of all types): " << this->TotalInsts << "\n";
	errs() << "Number of basic blocks: " << this->TotalBlocks << "\n";
	errs() << "Number of BB's with less than 15 instructions: " << this->BlockLow << "\n";
	errs() << "Number of BB's with instructions between [15, 500]: " << this->BlockMid << "\n";
	errs() << "Number of BB's with more than 500 instructions: " << this->BlockHigh << "\n";
	errs() << "Number of non-external functions: " << this->TotalFuncs << "\n";
	errs() << "Number of memory instructions: " << this->TotalMemInst << "\n";
	errs() << "Number of Phi-nodes at beginning of BB: " << this->BeginPhi << "\n";
	errs() << "Total arguments to Phi nodes: " << this->ArgsPhi << "\n";
	errs() << "Number of BB's with no Phi nodes: " << this->BBNoPhi << "\n";
	errs() << "Number of BB's with Phi node in range (0, 3]: " << this->BB03Phi << "\n";
	errs() << "Number of BB's with more than 3 Phi nodes: " << this->BBHiPhi << "\n";
	errs() << "Number of BB where total args for phi nodes > 5: " << this->BBNumArgsHi << "\n";
	errs() << "Number of BB where total args for phi nodes is [1, 5]: " << this->BBNumArgsLo << "\n";
	errs() << "Unary: " << this->testUnary << "\n";
	errs() << "Number of BB where total args for phi nodes is [1, 5]: " << this->binaryConstArg << "\n";
	errs() << "Number of calls with number of arguments > 4: " << this->callLargeNumArgs << "\n";
	errs() << "Number of calls that return an int: " << this->returnInt << "\n"; 
	errs() << "Number of BB's with 1 successor: " << this->oneSuccessor << "\n";
	errs() << "Number of BB's with 2 successors: " << this->twoSuccessors << "\n";
	errs() << "Number of BB's with >2 successors: " << this->moreSuccessors << "\n";
	errs() << "Number of BB's with 1 predecessor: " << this->onePred << "\n";
	errs() << "Number of BB's with 2 predecessors: " << this->twoPred << "\n";
	errs() << "Number of BB's with >2 predecessors: " << this->morePreds << "\n";
	errs() << "Number of BB's with 1 predecessor and 1 successor: " << this->onePredOneSuc << "\n";
	errs() << "Number of BB's with 1 predecessor and 2 successors: " << this->onePredTwoSuc << "\n";
	errs() << "Number of BB's with 2 predecessors and 1 successor: " << this->twoPredOneSuc << "\n";
	errs() << "Number of BB's with 2 predecessors and successors: " << this->twoEach << "\n";
	errs() << "Number of BB's with >2 predecessors and successors: " << this->moreEach << "\n";
	errs() << "Number of edges: " << this->NumEdges << "\n";
	errs() << "Number of critical edges: " << this->CriticalCount <<"\n";
	errs() << "Number of branches: " << this->BranchCount << "\n";
	errs() << "Number of occurrences of constant 1: " << this->numConstOnes << "\n";
	errs() << "Number of occurrences of constant 0: " << this->numConstZeros << "\n";
	errs() << "Number of occurrences of 32-bit integer constants: " << this->const32Bit << "\n";
	errs() << "Number of occurrences of 64-bit integer constants: " << this->const64Bit << "\n";
	errs() << "Number of unconditional branches: " << this->UncondBranches << "\n";
	for (auto &it : this->Instcount)
		errs() << "Number of " << it.first << " insts: " << it.second << "\n";
}

void HI_ExtraNum::visitFunction(Function &F) { this->TotalFuncs ++; }

void HI_ExtraNum::visitBasicBlock(BasicBlock &BB)
{   this->TotalBlocks ++;

	Instruction* term = BB.getTerminator();
	unsigned numSuccessors = term->getNumSuccessors();
	for (int i = 0; i < numSuccessors; i++) {
		this->NumEdges++;
		if (isCriticalEdge(term, i)) {
			this->CriticalCount++;
		}
	}
	unsigned numPreds = 0;
	for (pred_iterator pi = pred_begin(&BB), E = pred_end(&BB); pi != E; ++pi) {
		numPreds++;
	}
	if (numSuccessors == 1) {
		this->oneSuccessor++;
	} else if (numSuccessors == 2) {
		this->twoSuccessors++;

	} else if (numSuccessors > 2) {
		this->moreSuccessors++;
	}
	if (numPreds == 1) {
		this->onePred++;
	} else if (numPreds == 2) {
		this->twoPred++;
	} else if (numPreds > 2) {
		this->morePreds++;
	}

	if (numPreds == 1 && numSuccessors == 1) {
		this->onePredOneSuc++;
	} else if (numPreds == 2 && numSuccessors == 1) {
		this->twoPredOneSuc++;
	} else if (numPreds == 1 && numSuccessors == 2) {
		this->onePredTwoSuc++;
	} else if (numPreds == 2 && numSuccessors == 2) {
		this->twoEach++;
	} else if (numPreds > 2 && numSuccessors > 2) {
		this->moreEach++;
	}

	unsigned tempCount = 0;
	bool isFirst = true;
	unsigned phiCount = 0;
	unsigned BBArgs = 0;
	for (Instruction &I : BB) {
		if (auto *bi = dyn_cast<BranchInst>(&I)) {
			this->BranchCount++;
			if (bi->isUnconditional()) {
				this->UncondBranches++;
			}
		}
		for (int i = 0 ; i < I.getNumOperands(); i++) {
			Value* v = I.getOperand(i);
			//Type* t = v->getType();
			if (auto *c = dyn_cast<Constant>(v)) {
				if (auto *ci = dyn_cast<ConstantInt>(c)) {
					APInt val = ci->getValue();
					unsigned bitWidth = val.getBitWidth();
					if (bitWidth == 32) {
						this->const32Bit++;
					} else if (bitWidth == 64) {
						this->const64Bit++;
					}
					if (val == 1) {
						this->numConstOnes++;
					} else if (val == 0) {
						this->numConstZeros++;
					}
				}
			}	
		} 
		if (CallInst* CallI = dyn_cast<CallInst>(&I)) {
			if (CallI->getNumOperands() > 4) {
				this->callLargeNumArgs++;
			} 
			if (CallI->getType()->isIntegerTy()) {
				this->returnInt++;
			}
		}
		if (isa<UnaryInstruction>(I)){
			this->testUnary++;
		}
		if (isa<BinaryOperator>(I)) {
			if (isa<Constant>(I.getOperand(0)) || isa<Constant>(I.getOperand(1))) { 
				this->binaryConstArg++;
			}	
		}
		if (isFirst && isa<PHINode>(I)) {
			this->BeginPhi++;
		}
		if (isa<PHINode>(I)) {
			phiCount++;
			unsigned inc = cast<PHINode>(I).getNumIncomingValues();
			this->ArgsPhi += inc;
			BBArgs += inc;
		}			
		isFirst = false;
		tempCount++;
	}
	if (phiCount == 0) {
		this->BBNoPhi++;
	} else if (phiCount <= 3) {
		this->BB03Phi++;
	} else {
		this->BBHiPhi++;
	}
	if (BBArgs > 5) {
		this->BBNumArgsHi++;
	} else if (BBArgs>=1) {
		this->BBNumArgsLo++;
	}
	if (tempCount <15) {
		this->BlockLow++;
	} else if (tempCount <= 500) {
		this->BlockMid++;
	} else {
		this->BlockHigh++;
	}
} 

void HI_ExtraNum::visitInstruction(Instruction &I) {
	this->Instcount[I.getOpcodeName(I.getOpcode())] ++;
 }

bool HI_ExtraNum::runOnModule(Module &M) {
	for(Function &F : M)
		if(F.getName().find("llvm") == std::string::npos)
		{
			this->visitFunction(F);
			for(BasicBlock &BB : F)
			{
				this->visitBasicBlock(BB);
				for(Instruction &I : BB)
					visitInstruction(I);
			}
		}
//   unsigned StartMemInsts =
//     NumGetElementPtrInst + NumLoadInst + NumStoreInst + NumCallInst +
//     NumInvokeInst + NumAllocaInst;
//   unsigned EndMemInsts =
//     NumGetElementPtrInst + NumLoadInst + NumStoreInst + NumCallInst +
//     NumInvokeInst + NumAllocaInst;
//   this->TotalMemInst += EndMemInsts-StartMemInsts;
	for(auto it = Instcount.begin(), iv = Instcount.end(); it != iv; it++)
	{
		this->TotalInsts += it->second;
		if(std::string(it->first) == "getelementptr" || std::string(it->first) == "load" || std::string(it->first) == "store")
			this->TotalMemInst += it->second;
	}
  this->print_result();
  return false;
}

char HI_ExtraNum::ID = 0;

void HI_ExtraNum::getAnalysisUsage(AnalysisUsage &AU) const {
	AU.setPreservesAll();
}