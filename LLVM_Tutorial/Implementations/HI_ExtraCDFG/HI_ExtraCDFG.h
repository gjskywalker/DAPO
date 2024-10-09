#ifndef _HI_EXTRACDFG
#define _HI_EXTRACDFG

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

class HI_ExtraCDFG : public ModulePass
{
public:
    
    HI_ExtraCDFG(string pgm_name, string path, bool verb) : ModulePass(ID)
    {
        function_count = 0;
        verbose = verb;
        Edge = new raw_fd_ostream(path+"/"+"Edge_"+pgm_name+".csv", ErrInfo, llvm::sys::fs::OF_None);
        Node_Feature = new raw_fd_ostream(path+"/"+"Node_Feature_"+pgm_name+".csv", ErrInfo, llvm::sys::fs::OF_None);
        if (verbose)
        {
            Inst_info = new raw_fd_ostream("Inst_Info_"+pgm_name+".csv", ErrInfo, llvm::sys::fs::OF_None);
        }
        BB_count = 0;
        inst_count = 0;
        BasicBlockindex.clear();
        /*
            In the initial version, make the granularity be instruction, no need to do complex taxonomy of basic blocks. So the 0th ~ 23th use one-hot number to represent the type of instructions, and use 24th ~ 28th to represent the variable bitwidth;
        */

        // 0th~4th use one-hot to represent the Block types
        // NodeFeature["Standard_Block"] = 1;
        // InsertOrder.push_back("Standard_Block");
        // NodeFeature["Load_Block"] = 0;
        // InsertOrder.push_back("Load_Block");
        // NodeFeature["Store_Block"] = 0;
        // InsertOrder.push_back("Store_Block");
        // NodeFeature["Loop_Block"] = 0;
        // InsertOrder.push_back("Loop_Block");
        // NodeFeature["Function_Block"] = 0;
        // InsertOrder.push_back("Function_Block");
        // 5th~7th use real number to represent the loop information
        // NodeFeature["Loop_Stride"] = 0;
        // InsertOrder.push_back("Loop_Stride");
        // NodeFeature["Loop_Depth"] = 0;
        // InsertOrder.push_back("Loop_Depth");
        // NodeFeature["Loop_Trip_Count"] = 0;
        // InsertOrder.push_back("Loop_Trip_Count");
        // 8th~10th use real number to record the call instruction number and function parameter/return variable bitwidth in one Basic Block
        // NodeFeature["Function_Invocations"] = 0;
        // InsertOrder.push_back("Function_Invocations");
        // NodeFeature["Function_Parameters"] = 0;
        // InsertOrder.push_back("Function_Parameters");
        // NodeFeature["Function_Return"] = 0;
        // 11th~34th use real number to represent the number of all instruction within one basicblock
        // InsertOrder.push_back("PTI"); //0
        // InsertOrder.push_back("ITP"); //1
        // InsertOrder.push_back("FTU"); //2
        // InsertOrder.push_back("FTS"); //3
        // InsertOrder.push_back("UTF"); //4
        // InsertOrder.push_back("STF"); //5
        // InsertOrder.push_back("ZEXTI"); //6
        // InsertOrder.push_back("SEXTI"); //7
        // InsertOrder.push_back("TI"); //8
        // InsertOrder.push_back("BI"); //9
 
        // InsertOrder.push_back("Shl"); //10
        // InsertOrder.push_back("Lshr"); //11
        // InsertOrder.push_back("Ashr"); //12
        // InsertOrder.push_back("And"); //13
        // InsertOrder.push_back("Xor"); //14
        // InsertOrder.push_back("Or"); //15

        // InsertOrder.push_back("Add"); //16
        // InsertOrder.push_back("Mul"); //17
        // InsertOrder.push_back("Sub"); //18
        // InsertOrder.push_back("Div"); //19
        // InsertOrder.push_back("Rem"); //20
        

        // InsertOrder.push_back("Store"); //21
        // InsertOrder.push_back("Load");  //22
        // InsertOrder.push_back("Alloca");  //23

        // InsertOrder.push_back("Icmp"); //24
        // InsertOrder.push_back("Fcmp"); //25
        // InsertOrder.push_back("Call"); //26
        // InsertOrder.push_back("PHI"); //27
        // InsertOrder.push_back("Select"); //28
        // InsertOrder.push_back("SI"); //29
        // InsertOrder.push_back("Br"); //30

        // InsertOrder.push_back("Ret"); //31
        // InsertOrder.push_back("URI"); //32
        // InsertOrder.push_back("UNI"); //33
        

        // InsertOrder.push_back("BasicBlock"); //34
        
        // InsertOrder.push_back("Bitwidth0");
        // InsertOrder.push_back("Bitwidth1");
        // InsertOrder.push_back("Bitwidth2");
        // InsertOrder.push_back("Bitwidth3");
        // InsertOrder.push_back("Bitwidth4");
        // InsertOrder.push_back("Bitwidth5");
        // InsertOrder.push_back("Bitwidth6");
        // InsertOrder.push_back("Cast"); //0
        // InsertOrder.push_back("Logic"); //1
        // InsertOrder.push_back("Binary"); //2
        // InsertOrder.push_back("Mem"); //3
        // InsertOrder.push_back("Control"); //4
        // InsertOrder.push_back("Loop"); //5
        // InsertOrder.push_back("Terminal"); // 6
        // InsertOrder.push_back("BasicBlock"); //7
        // // How to illustrate this dimension;
        // InsertOrder.push_back("Misc"); //8

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
    ~HI_ExtraCDFG()
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
        print_status("Initilizing HI_ExtraCDFG pass.");
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
    // int getBBIndex(BasicBlock *BB);
    void DitoBi(Node &node, int num);
    void CountInstr(Instruction &I, Node &node);
    void CountBB(BasicBlock &BB, Node &node);
    // void GetLoopFeature(BasicBlock &B);
    bool Is_InLoop(BasicBlock &B);
    // void getInstdef(BasicBlock &B);
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
