#ifndef _HI_FEATURE_EXTRACTOR
#define _HI_FEATURE_EXTRACTOR

// Related LLVM Header Files
#include "HI_print.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/CFG.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/LoopNestAnalysis.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/FileSystem.h"

// Related STL 
#include <unordered_map>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>


using namespace llvm;
using namespace std;

class HI_feature_extractor : public ModulePass
{
public:
    
    HI_feature_extractor(string pgm_name, string path, bool verb) : ModulePass(ID)
    {
        function_count = 0;
        verbose = verb;
        Edge = new raw_fd_ostream(path+"/"+"Edge_"+pgm_name+".csv", ErrInfo, llvm::sys::fs::OF_None);
        Node_Feature = new raw_fd_ostream(path+"/"+"Node_Feature_"+pgm_name+".csv", ErrInfo, llvm::sys::fs::OF_None);
        if (verbose)
        {
            Inst_info = new raw_fd_ostream(path+"/"+"Inst_Info_"+pgm_name+".csv", ErrInfo, llvm::sys::fs::OF_None);
        }
        BB_count = 0;
        inst_count = 0;
        BasicBlockindex.clear();

        InsertOrder.push_back("Terminal");
        InsertOrder.push_back("Unary");
        InsertOrder.push_back("Binary");
        InsertOrder.push_back("Logic");
        InsertOrder.push_back("Vector");
        InsertOrder.push_back("Aggregate");
        InsertOrder.push_back("Memory");
        InsertOrder.push_back("Cast");
        InsertOrder.push_back("Select");
        InsertOrder.push_back("Misc");
        InsertOrder.push_back("BasicBlock");
        InsertOrder.push_back("Loop");
    }
    ~HI_feature_extractor()
    {
        InsertOrder.clear();
        Instnamemap.clear();
        Instmap.clear();
        BasicBlockindex.clear();
        Block2Loops.clear();
        InstVec.clear();
        BBnamemap.clear();
        BlockVec.clear();
        Edge->flush();
        delete Edge;
        Node_Feature->flush();
        delete Node_Feature;
        if(verbose){
            Inst_info->flush();
            delete Inst_info;
        }
    }
    void getAnalysisUsage(AnalysisUsage &AU) const;
    bool runOnModule(llvm::Module &M);
    virtual bool doInitialization(llvm::Module &M)
    {
        print_status("Initilizing HI_feature_extractor pass.");
        return false;
    }
    static char ID;

    int idx;
    int verbose;
    int inst_count;
    int BB_count; // Use this to record BB
    int function_count;
    LoopInfo* LI;
    ScalarEvolution* SE;
    std::error_code ErrInfo;
    raw_ostream *Edge;
    raw_ostream *Node_Feature;
    raw_ostream *Inst_info;
    std::set<Value *> funargs;
    std::vector<std::string> InsertOrder;
    map<Instruction *, string> Instnamemap;
    map<BasicBlock *, string> BBnamemap;
    unordered_map<Instruction *, vector<Instruction *>> Instmap;
    unordered_map<int, BasicBlock *> BasicBlockindex;
    unordered_map<BasicBlock *, std::vector<Loop *>> Block2Loops;

    typedef struct
    {
        std::string name;
        std::unordered_map<std::string, bool> NodeFeature{};
    } Node;
    std::vector<Instruction *> InstVec;
    std::vector<BasicBlock *> BlockVec;
    std::vector<Node> NodeVec;

    void DitoBi(Node &node, int num);
    void CountInstr(Instruction &I, Node &node);
    void CountBB(BasicBlock &BB, Node &node);
    bool Is_InLoop(BasicBlock &B);
    int getvarwidth(Instruction *I);

    // get the relationship between loops and blocks
    void getLoopBlockMap(Function *F);
    int getPointerBitwidth(Instruction *I);
    void initNode(Node &node);
    void ClassifyInst(Instruction &I, Node &node);

    struct timeval tv_begin;
    struct timeval tv_end;
};

#endif
