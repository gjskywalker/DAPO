#include "HI_ExtraCDFG.h"

using namespace llvm;
using namespace std;

bool HI_ExtraCDFG::runOnModule(
    Module &M) // The runOnModule declaration will overide the virtual one in ModulePass, which will
               // be executed for each Module.
{
    if(verbose)
    {
        *Inst_info << "=================================== Module Begin =======================================" << "\n";
        *Inst_info << M << "\n";
        *Inst_info << "=================================== Module End =========================================" << "\n";
    }
    // for (Function &F : M)
    // {
    //     if(F.getName().find("llvm.") == std::string::npos)
    //     {
    //         LI = &getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
    //         SE = &getAnalysis<ScalarEvolutionWrapperPass>(F).getSE();
    //         Function *F_ptr = &F;
    //         //getLoopBlockMap(F_ptr);
    //         for(BasicBlock &BB : F)
    //         {
    //             BasicBlock *BB_tmp = &BB;
    //             BasicBlockindex.insert(make_pair(BB_count, BB_tmp));
    //             BB_count++;
    //         }
    //     }
    // }
    
    // cout << "=====================================" << "\n";
    // for(auto ele : Block2Loops)
    // {
    //     std::cout << ele.first->getName().str() << ": ";
    //     for(auto loop : ele.second)
    //     std::cout << loop->getName().str() << "; ";
    //     std::cout << endl;
    // }

    // Since we use unorder_map to store basic block attributes, the order maybe not as same as in BasicBlockIndex. I use an extra vector to ensure 
    // the information stored in BB_Attribute will be as same as BasicBlockIndex. 
    // for(int i = 0;i < InsertOrder.size() -1; i++)
    // {
    //     *BB_Attribute << InsertOrder[i] << ",";
    // }
    // *BB_Attribute << "ZExt" << "\n";
    // *BB_Edge << "A->," << "B" << "\n";
    int Node_Count = -1;
    for (Function &F : M)
    {
        if(F.getName().find("llvm.") == std::string::npos)
        {
            LI = &getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
            SE = &getAnalysis<ScalarEvolutionWrapperPass>(F).getSE();
            Function *ptr_F = &F;
            getLoopBlockMap(ptr_F);
            for(auto it = F.arg_begin(), ie = F.arg_end(); it != ie; ++it)
                funargs.insert(&(*it));
            for(BasicBlock &BB : F)
            {
                Node BB_node;
                initNode(BB_node);
                Node_Count += 1;
                BB_count += 1;
                string BB_name = to_string(Node_Count);
                // string BB_name = "BB" + to_string(BB_count);
                BasicBlock *BB_ptr = &BB;
                BBnamemap.insert(pair(BB_ptr, BB_name));
                // errs() << BB_name << "\n";
                BB_node.name = BB_name;
                CountBB(BB, BB_node);
                NodeVec.push_back(BB_node);
                BlockVec.push_back(BB_ptr);
                InstVec.clear();
                for(Instruction &I : BB)
                {
                    Node_Count += 1;
                    Node Inst_node;
                    initNode(Inst_node);
                    inst_count += 1;
                    string inst_name = to_string(Node_Count);
                    // string inst_name = "inst" + to_string(inst_count);
                    // errs() << inst_name << "\n";
                    Instruction *I_ptr = &I;
                    Instnamemap.insert(pair(I_ptr,inst_name));
                    Inst_node.name = inst_name;
                    ClassifyInst(I, Inst_node);
                    // CountInstr(I, Inst_node);
                    NodeVec.push_back(Inst_node);
                    InstVec.push_back(I_ptr);
                }
                // BasicBlock *BB_ptr = &BB;
                // for (BasicBlock *succ_BB : successors(BB_ptr))
                // {
                //     // *BB_Edge << BB_ptr->getName().str() << " " << getBBIndex(BB_ptr) << " : " << succ_BB->getName().str() << " " << getBBIndex(succ_BB) << "\n";
                //     *BB_Edge << idx << "," <<getBBIndex(BB_ptr) << "," << getBBIndex(succ_BB) << "\n";
                // }
                // if(Is_InLoop(BB))
                // {
                //     NodeFeature["Loop_Block"] = 1;
                //     NodeFeature["Standard_Block"] = 0;
                // }
                // GetLoopFeature(BB);
                // // *BB_Attribute << BB.getName().str() << " ";
                // *BB_Attribute << idx << ",";
                // for(int i=0;i < InsertOrder.size()-1; i++)
                // {
                //     string s = InsertOrder[i];
                //     *BB_Attribute << NodeFeature[s] << ",";
                // }
                // *BB_Attribute << NodeFeature["ZExt"] << "\n";
            }
        }
    }
    for (auto ele : Instnamemap)
    {
        for(auto it = ele.first->user_begin(), ie = ele.first->user_end(); it != ie; it ++)
        {
            if(Instruction *Itmp = dyn_cast<Instruction>(*it))
            {
                Instmap[ele.first].push_back(Itmp);
            }
       }
    }
    /*
        Print the node feature information;
    */
    *Node_Feature << "id";
    for(auto index : InsertOrder)
    {
        *Node_Feature << "," << index;
    }
    *Node_Feature << "\n";
    for(auto nodes : NodeVec)
    {
        *Node_Feature << nodes.name;
        for(auto index : InsertOrder)
        {
            *Node_Feature << "," << nodes.NodeFeature[index];
        }
        *Node_Feature << "\n";
    }
    /*
        Print instruction connection information;
    */
    *Edge << "Source,Target,,," << "\n";
    for(auto ele : Instnamemap)
    {
        if(!Instmap[ele.first].empty())
        {
            for(auto insts : Instmap[ele.first])
            {
                *Edge << ele.second << "," << Instnamemap[insts] << ",1,0,0" << "\n";
            }
        }
    }
    /*
        Print BasicBlock connection information;
    */
    for(auto ele : BlockVec)
    {
        for(BasicBlock *BB_succ : successors(ele))
        {
            *Edge << BBnamemap[ele] << "," << BBnamemap[BB_succ] << ",0,1,0" << "\n";
        }
    }
    /*
        Print BasicBlock & Instruction connection information; 
    */
    for(Function &F : M)
    {
            if(F.getName().find("llvm.") == std::string::npos)
            {
                for(BasicBlock &BB : F)
                {
                    BasicBlock *BB_Ptr = &BB;
                    for(Instruction &I : BB)
                    {
                        Instruction *I_ptr = &I;
                        *Edge << Instnamemap[I_ptr] << "," << BBnamemap[BB_Ptr] << ",0,0,1" << "\n";
                    }   
                }
            }
    }
    if(verbose)
    {
        for (auto ele : Instnamemap)
        {
            *Inst_info << *(ele.first) << " ---> " << ele.second << "\n"; 
        }
        for (auto BB : BBnamemap)
        {
            *Inst_info << BB.first->getName().str() << " ---> " << BB.second << "\n";
        }
    }
    return false;
}

/*
    Currently, we don't need the specific loop information, but it will be used later.
*/
// void HI_ExtraCDFG::GetLoopFeature(BasicBlock &BB)
// {
//     int LoopDepth = 0;
//     int LoopTripCount = 1;
//     int Loopstride = 0;
//     BasicBlock *BB_ptr = &BB;
//     for (auto tmploop : Block2Loops[BB_ptr])
//     {
//         LoopDepth += 1;
//         if(SE->getSmallConstantTripCount(tmploop))
//         {
//             LoopTripCount *= SE->getSmallConstantTripCount(tmploop);
//         }
//         if(!tmploop->isCanonical(*SE))
//         {
//             Loopstride = 1;
//         }
//     }
//     NodeFeature["Loop_stride"] = Loopstride;
//     NodeFeature["Loop_Depth"] = LoopDepth;
//     NodeFeature["Loop_Trip_Count"] = ceil(log10(LoopTripCount)); // By doing so, we can ensure that the data range won't vary too large.
// }

bool HI_ExtraCDFG::Is_InLoop(BasicBlock &BB)
{
    BasicBlock *BB_ptr = &BB;
    return (Block2Loops.find(BB_ptr) != Block2Loops.end());
}

void HI_ExtraCDFG::initNode(Node &node)
{
    node.name = "";
    for(auto it : InsertOrder)
    {
        node.NodeFeature.insert(std::pair(it, false));
    }
}

void HI_ExtraCDFG::getLoopBlockMap(Function *F)
{
    Block2Loops.clear();
    for (LoopInfo::iterator i = LI->begin(), e = LI->end(); i != e; ++i)
    {
        Loop *L = *i;
        auto LN = LoopNest::getLoopNest(*L, *SE);
        int ND = LN->getNestDepth(); //Use this function to get the nested depth, so that we can find the nested loops
        for(int i=0; i< ND;i++)
        {
            Loop* Nest_L = LN->getLoop(i);
            for (auto BinL : Nest_L->getBlocks())
            {
            
                std::vector<Loop *> tmp_vec_loop;
                if (Block2Loops.find(BinL) == Block2Loops.end())
                {
                    Block2Loops[BinL].push_back(Nest_L);
                }
                else
                {
                    tmp_vec_loop = Block2Loops[BinL];
                    tmp_vec_loop.push_back(Nest_L);
                    Block2Loops[BinL] = tmp_vec_loop;
                }
            }
        }
    }
}

// int HI_ExtraCDFG::getBBIndex(BasicBlock *BB)
// {
//     for (auto ele : BasicBlockindex)
//     {
//         if(ele.second == BB)
//         {
//             return ele.first;
//         }
//     }
// }

void HI_ExtraCDFG::DitoBi(Node &node, int num)
{
    // assert(num <= 64 && "All test cases should within 64-bits!!!\n");
    if(num > 64)
        num = 64;
    int index = 6;
    string name;
    while(num > 0 && index >=0)
    {
        name = "Bitwidth" + to_string(6-index);
        node.NodeFeature[name] = num % 2;
        num /= 2;
        index --;
    }
    //return node;    
}

int HI_ExtraCDFG::getvarwidth(Instruction *I)
{
    int bitwidth;
    if (ICmpInst *ICI = dyn_cast<ICmpInst>(I))
    {
        int BW;
        Value *op1 = ICI->getOperand(0);
        if(op1->getType()->isIntegerTy())
        {
            BW = op1->getType()->getIntegerBitWidth();
        }
        if(op1->getType()->isPointerTy())
        {
            if(Instruction *tmpI = dyn_cast<Instruction>(op1))
            BW = getPointerBitwidth(tmpI);
        }
        return BW;
    }
    else if (BranchInst *Br = dyn_cast<BranchInst>(I))
        return 1;
    else if (StoreInst *SI = dyn_cast<StoreInst>(I))
    {
        if(SI->getOperand(0)->getType()->isIntegerTy())
        {
            bitwidth = SI->getOperand(0)->getType()->getIntegerBitWidth();
            return bitwidth;
        }
        else if(SI->getOperand(0)->getType()->isFloatTy())
        {
            return 32;
        }
        else if(SI->getOperand(0)->getType()->isDoubleTy())
        {
            return 64;
        }
        else if(auto VecType = dyn_cast<VectorType>(SI->getOperand(0)->getType()))
        {  
            auto tmp_type = VecType->getElementType();
            if(tmp_type->isIntegerTy())
                bitwidth = tmp_type->getIntegerBitWidth();
            return bitwidth;
        }
    }
    else if (ReturnInst *RI = dyn_cast<ReturnInst>(I))
    {
        if(RI->getFunction()->getReturnType()->isIntegerTy())
        {
            bitwidth = RI->getFunction()->getReturnType()->getIntegerBitWidth();
            return bitwidth;
        }
        else if(RI->getFunction()->getReturnType()->isFloatTy())
        {
            return 32;
        }
        else if(RI->getFunction()->getReturnType()->isDoubleTy())
        {
            return 64;
        }
        else
        {
            return 0;
        }
    }
    else if (AllocaInst *ALI = dyn_cast<AllocaInst>(I))
    {
        int bitwidth;
        Type* allocaType = ALI->getAllocatedType();
        if(auto array_T = dyn_cast<ArrayType>(allocaType))
        {
            // errs() << *I << "\n";
            auto tmp_type = array_T->getArrayElementType();
            if(tmp_type->isIntegerTy())
                bitwidth = tmp_type->getIntegerBitWidth();
            else if(tmp_type->isArrayTy())
            {
                auto tmp_type1 = tmp_type->getArrayElementType();
                if(tmp_type1->isIntegerTy())
                    bitwidth = tmp_type1->getIntegerBitWidth();
            }
        }
        else if (allocaType->isIntegerTy())
        {
            bitwidth = allocaType->getIntegerBitWidth();
        }
        return bitwidth;
    }
    else if (I->getType()->isIntegerTy())
    {
        bitwidth = I->getType()->getIntegerBitWidth();
        return bitwidth;
    }
    else if (I->getType()->isFloatTy())
    {
        return 32;
    }
    else if (I->getType()->isDoubleTy())
    {
        return 64;
    }
    else if (I->getType()->isVectorTy())
    {
        if(VectorType *vec = dyn_cast<VectorType>(I->getType()))
        {
            if(vec->getElementType()->isIntegerTy())
            {
                bitwidth = vec->getElementType()->getIntegerBitWidth();
                return bitwidth;
            }
            else if(vec->getElementType()->isFloatTy())
            {
                return 32;
            }
            else if(vec->getElementType()->isDoubleTy())
            {
                return 64;
            }
        }
    }
    errs() <<"Unknown data type : " << *I << "\n";
    assert(false);
    return 0;
}

int HI_ExtraCDFG::getPointerBitwidth(Instruction *I)
{
    if(IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(I))
    {
        int bitwidth;
        Value* op0 = ITP->getOperand(0);
        if(Instruction *tmpI = dyn_cast<Instruction>(op0))
        {
            bitwidth = tmpI->getType()->getIntegerBitWidth();
        }
        return bitwidth;
    }

    else if (AllocaInst *ALI = dyn_cast<AllocaInst>(I))
    {
        int bitwidth;
        Type* allocaType = ALI->getAllocatedType();
        if(auto array_T = dyn_cast<ArrayType>(allocaType))
        {
            // errs() << *I << "\n";
            auto tmp_type = array_T->getArrayElementType();
            if(tmp_type->isIntegerTy())
                bitwidth = tmp_type->getIntegerBitWidth();
            else if(tmp_type->isArrayTy())
            {
                auto tmp_type1 = tmp_type->getArrayElementType();
                if(tmp_type1->isIntegerTy())
                    bitwidth = tmp_type1->getIntegerBitWidth();
            }
        }
        else if (allocaType->isIntegerTy())
        {
            bitwidth = allocaType->getIntegerBitWidth();
        }
        return bitwidth;   
    }
    /*
        TODO: There maybe cases operand is a function arg
    */
    else
    {
        int num = I->getNumOperands();
        // errs() << num << "\n";
        // errs() << *I << "\n";
        for(int i=0; i <num;i++)
        {  
            Value *tmpOp = I->getOperand(i);
            if(funargs.find(tmpOp) != funargs.end())
            {
                // errs() << *tmpOp << "\n";
                for(auto it = tmpOp->user_begin(), ie = tmpOp->user_end(); it != ie; it ++)
                {
                    if(PtrToIntInst* PTI = dyn_cast<PtrToIntInst>(*it))
                    {
                        int bitwidth;
                        bitwidth = PTI->getType()->getIntegerBitWidth();
                        // errs() << *PTI << "\n";
                        // errs() << bitwidth << "\n";
                        return bitwidth;
                    }
                }
            }
            else if(Instruction *tmpI = dyn_cast<Instruction>(tmpOp))
            {
                // errs() << *tmpI << "\n";
                int bitwidth;
                if(tmpI != I)
                {
                    bitwidth = getPointerBitwidth(tmpI);
                    return bitwidth;
                }
            }
        }
    }
    errs() << *I << "\n";
    assert(false && "There should be an ITP before it\n");
}

void HI_ExtraCDFG::CountBB(BasicBlock &BB, Node &node)
{
    node.NodeFeature["BasicBlock"] = 1;
    if(Is_InLoop(BB))
        node.NodeFeature["Loop"] = 1;
}

void HI_ExtraCDFG::ClassifyInst(Instruction &I, Node &node)
{
    if (I.getOpcode() == Instruction::Trunc || I.getOpcode() == Instruction::FPTrunc || I.getOpcode() == Instruction::ZExt || I.getOpcode() == Instruction::SExt || I.getOpcode() == Instruction::FPExt || I.getOpcode() == Instruction::FPToSI || I.getOpcode() == Instruction::FPToUI || I.getOpcode() == Instruction::UIToFP || I.getOpcode() == Instruction::SIToFP || I.getOpcode() == Instruction::PtrToInt || I.getOpcode() == Instruction::IntToPtr || I.getOpcode() == Instruction::BitCast || I.getOpcode() == Instruction::AddrSpaceCast)
        node.NodeFeature["Cast"] = 1;
    
    else if (I.getOpcode() == Instruction::Shl || I.getOpcode() == Instruction::LShr || I.getOpcode() == Instruction::AShr || I.getOpcode() == Instruction::And || I.getOpcode() == Instruction::Or || I.getOpcode() == Instruction::Xor)
        node.NodeFeature["Logic"] = 1;
    
    else if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
        node.NodeFeature["Binary"] = 1;
    else if (CallInst *CI = dyn_cast<CallInst>(&I))
    {
        if(CI->getCalledFunction()->getName().find("fmuladd") != std::string::npos)
            node.NodeFeature["Binary"] = 1;
    }
    
    else if (I.getOpcode() == Instruction::Store || I.getOpcode() == Instruction::Load || I.getOpcode() == Instruction::Alloca || I.getOpcode() == Instruction::GetElementPtr || I.getOpcode() == Instruction::Fence || I.getOpcode() == Instruction::AtomicCmpXchg || I.getOpcode() == Instruction::AtomicRMW)
        node.NodeFeature["Mem"] = 1;
    
    else if (I.getOpcode() == Instruction::Freeze || I.getOpcode() == Instruction::VAArg || I.getOpcode() == Instruction::LandingPad || I.getOpcode() == Instruction::CatchPad || I.getOpcode() == Instruction::CleanupPad)
        node.NodeFeature["Misc"] = 1;
    
    else if (I.getOpcode() == Instruction::ICmp || I.getOpcode() == Instruction::FCmp || I.getOpcode() == Instruction::Select || I.getOpcode() == Instruction::PHI)
        node.NodeFeature["Select"] = 1;
        
    else if (I.getOpcode() == Instruction::Ret || I.getOpcode() == Instruction::Unreachable || I.getOpcode() == Instruction::Br || I.getOpcode() == Instruction::CallBr || I.getOpcode() == Instruction::Switch || I.getOpcode() == Instruction::IndirectBr || I.getOpcode() == Instruction::Invoke || I.getOpcode() == Instruction::Resume || I.getOpcode() == Instruction::CatchRet || I.getOpcode() == Instruction::CatchSwitch || I.getOpcode() == Instruction::CleanupRet)
        node.NodeFeature["Terminal"] = 1;
    
    else if (I.getOpcode() == Instruction::FNeg)
        node.NodeFeature["Unary"] = 1;
    
    else if (I.getOpcode() == Instruction::ExtractElement || I.getOpcode() == Instruction::InsertElement || I.getOpcode() == Instruction::ShuffleVector)
        node.NodeFeature["Vector"] = 1;
    
    else if (I.getOpcode() == Instruction::ExtractValue || I.getOpcode() == Instruction::InsertValue)
        node.NodeFeature["Aggregate"] = 1;
    
    else if (CallInst *CI = dyn_cast<CallInst>(&I))
    {
        if(CI->getCalledFunction()->getName().find("llvm.") == std::string::npos)
        {
            function_count ++;
            assert(function_count < 2 && "There should not be call function existing!");
        }
    }
    else
    {
        errs() << I << "\n";
        assert("Undefined Instruction Type");
    }
}

void HI_ExtraCDFG::CountInstr(Instruction &I, Node &node)
{
    ////////////////////////////// Cast Operations /////////////////////////
    if (PtrToIntInst *PTI = dyn_cast<PtrToIntInst>(&I)) // such operation like trunc/ext will not cost extra timing on FPGA
    {
        node.NodeFeature["PTI"] = 1;
        int BW = PTI->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (IntToPtrInst *ITP = dyn_cast<IntToPtrInst>(&I))
    {
        node.NodeFeature["ITP"] = 1;
        int BW = getPointerBitwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    else if (FPToUIInst *FTU = dyn_cast<FPToUIInst>(&I))
    {
        node.NodeFeature["FTU"] = 1;
        int BW = FTU->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (FPToSIInst *FTS = dyn_cast<FPToSIInst>(&I))
    {
        node.NodeFeature["FTS"] = 1;
        int BW = FTS->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (UIToFPInst *UTF = dyn_cast<UIToFPInst>(&I))
    {
        Value *op1 = UTF->getOperand(0);
        node.NodeFeature["UTF"] = 1;
        int BW = op1->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (SIToFPInst *STF = dyn_cast<SIToFPInst>(&I))
    {
        Value *op1 = STF->getOperand(0);
        node.NodeFeature["STF"] = 1;
        int BW = op1->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (ZExtInst *ZEXTI = dyn_cast<ZExtInst>(&I)) // such operation like trunc/ext will not cost extra timing on FPGA
    {
        Value *op1 = ZEXTI->getOperand(0);
        node.NodeFeature["ZEXTI"] = 1;
        int BW = op1->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (SExtInst *SEXTI = dyn_cast<SExtInst>(&I))
    {
        Value *op1 = SEXTI->getOperand(0);
        node.NodeFeature["SEXTI"] = 1;
        int BW = op1->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (TruncInst *TI = dyn_cast<TruncInst>(&I))
    {
        Value *op1 = TI->getOperand(0);
        node.NodeFeature["TI"] = 1;
        int BW = op1->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    else if (BitCastInst *BI = dyn_cast<BitCastInst>(&I))
    {
        Value *op1 = BI->getOperand(0);
        node.NodeFeature["BI"] = 1;
        int BW = op1->getType()->getIntegerBitWidth();
        DitoBi(node, BW);
        //return node;
    }
    ////////////////// Binary Operations //////////////////////
    else if (ShlOperator *SHLI = dyn_cast<ShlOperator>(&I))
    {
        node.NodeFeature["Shl"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    else if (LShrOperator *LSHRI = dyn_cast<LShrOperator>(&I))
    {
        node.NodeFeature["Lshr"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    else if (AShrOperator *ASHRI = dyn_cast<AShrOperator>(&I))
    {
        node.NodeFeature["Ashr"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    else if (BinaryOperator *BinO = dyn_cast<BinaryOperator>(&I))
    {
        if (BinO->getOpcode() == Instruction::Add || BinO->getOpcode() == Instruction::FAdd)
        {
            node.NodeFeature["Add"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
            //return node;
        }
        else if (BinO->getOpcode() == Instruction::Mul || BinO->getOpcode() == Instruction::FMul)
        {
            node.NodeFeature["Mul"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
            //return node;
        }
        else if (BinO->getOpcode() == Instruction::And)
        {
            node.NodeFeature["And"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
            //return node;
        }
        else if (BinO->getOpcode() == Instruction::Xor)
        {
            node.NodeFeature["Xor"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
            //return node;
        }
        else if (BinO->getOpcode() == Instruction::Sub || BinO->getOpcode() == Instruction::FSub)
        {
            node.NodeFeature["Sub"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
            //return node;
        }
        else if (BinO->getOpcode() == Instruction::Or)
        {
            // node.NodeFeature["Or"] = 1;
            node.NodeFeature["Logic"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
            //return node;
        }        
        else if (BinO->getOpcode() == Instruction::UDiv || BinO->getOpcode() == Instruction::SDiv || BinO->getOpcode() == Instruction::FDiv)
        {
            node.NodeFeature["Div"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
        }
        else if (BinO->getOpcode() == Instruction::URem || BinO->getOpcode() == Instruction::SRem || BinO->getOpcode() == Instruction::FRem)
        {
            node.NodeFeature["Rem"] = 1;
            int BW = getvarwidth(&I);
            DitoBi(node, BW);
        }
    }
    else if (ICmpInst *ICI = dyn_cast<ICmpInst>(&I))
    {
        node.NodeFeature["Icmp"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    else if (FCmpInst *FI = dyn_cast<FCmpInst>(&I))
    {
        node.NodeFeature["Fcmp"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    //////////////////////// Memory Operations ////////////////////////////
    else if (StoreInst *SI = dyn_cast<StoreInst>(&I))
    {
        node.NodeFeature["Store"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
        // node.NodeFeature["Standard_Block"] = 0;
        // node.NodeFeature["Store_Block"] = 1;
    }
    else if (LoadInst *LI = dyn_cast<LoadInst>(&I))
    {
        node.NodeFeature["Load"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
        // node.NodeFeature["Standard_Block"] = 0;
        // node.NodeFeature["Load_Block"] = 1;
    }
    else if (AllocaInst *AI = dyn_cast<AllocaInst>(&I))
    {
        node.NodeFeature["Alloca"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    //////////////////////// Control Operations ///////////////////////////
    else if (CallInst *CallI = dyn_cast<CallInst>(&I))
    {
        if(Function *CallF = CallI->getCalledFunction())
        {
            if (CallF->getName().find("llvm.fmuladd") != std::string::npos)
            {
                node.NodeFeature["Call"] = 1;
                int BW = getvarwidth(CallI);
                DitoBi(node, BW);
                //return node;

            }
            else if (CallF->getName().find("llvm.") == std::string::npos)
            {
                assert(0 && "All test cases should be without call function");
                node.NodeFeature["Function_Invocations"] = 1;
                node.NodeFeature["Standard_Block"] = 0;
                node.NodeFeature["Function_Block"] = 1;
                int paramnum = 0;
                int funreturn = 0;
                if(CallF->arg_size() > paramnum)
                {
                    node.NodeFeature["Function_Parameters"] = CallF->arg_size();
                    paramnum = CallF->arg_size();
                }
            }
        }
    }
    else if (PHINode *PHI = dyn_cast<PHINode>(&I))
    {
        node.NodeFeature["PHI"] = 1;
        DitoBi(node, 1);
        //return node;
    }
    else if (BranchInst *BI = dyn_cast<BranchInst>(&I))
    {
        node.NodeFeature["Br"] = 1;
        DitoBi(node, 1);
        //return node;
    }
    else if (ReturnInst *RI = dyn_cast<ReturnInst>(&I))
    {
        node.NodeFeature["Ret"] = 1;
        int BW = getvarwidth(&I);
        DitoBi(node, BW);
        //return node;
    }
    else if (SelectInst *SeI = dyn_cast<SelectInst>(&I))
    {
        node.NodeFeature["Select"] = 1;
        DitoBi(node, 1);
        //return node;
    }
    else if (UnreachableInst *URI = dyn_cast<UnreachableInst>(&I))
    {
        /*
            This is a new instruction introduced in llvm. Here is the deverloper log:
            https://www.nondot.org/sabre/LLVMNotes/UnreachableInstruction.txt
        */
       node.NodeFeature["URI"] = 1;
       DitoBi(node, 0);
       //return node;
        
    }
    else if (UnaryInstruction *UNI = dyn_cast<UnaryInstruction>(&I))
    {   
        /*
            This will include inst like fneg; A better way is to use UnaryOperator and getOpcode
            to get more specific classification but since we don't have the corresponding Hardware
            library, I just choose omit this inst now.
        */
       node.NodeFeature["UNI"] = 1;
       DitoBi(node, 0);
       //return node;
        
    }
    else if (SwitchInst *SI = dyn_cast<SwitchInst>(&I))
    {
        int casenum;
        casenum = SI->getNumCases();
        node.NodeFeature["SI"] = 1;
        DitoBi(node, casenum);
        //return node;
    }
    /*
        TODO: The following are some new insts in llvm 
    */
    else if (InsertElementInst *IEI = dyn_cast<InsertElementInst>(&I))
    {
        
    }
    else if (ShuffleVectorInst *SVI = dyn_cast<ShuffleVectorInst>(&I))
    {

    }
    else if (ExtractElementInst *EEI = dyn_cast<ExtractElementInst>(&I))
    {

    }
    else if (AtomicRMWInst *ARI = dyn_cast<AtomicRMWInst>(&I))
    {
        
    }
    else if (AtomicCmpXchgInst *ACXI = dyn_cast<AtomicCmpXchgInst>(&I))
    {
        
    }
    else if (AtomicMemCpyInst *AMCI = dyn_cast<AtomicMemCpyInst>(&I))
    {
        
    }
    else if (AtomicMemMoveInst *AMMI = dyn_cast<AtomicMemMoveInst>(&I))
    {
        
    }
    else if (AtomicMemSetInst *AMSI = dyn_cast<AtomicMemSetInst>(&I))
    {
        
    }
    else if (AtomicMemTransferInst *AMTI = dyn_cast<AtomicMemTransferInst>(&I))
    {
       
    }
    else
    {
        llvm::errs() << I << "\n";
        assert(false && "The instruction is not defined.");
    }
}


char HI_ExtraCDFG::ID =
    0; // the ID for pass should be initialized but the value does not matter, since LLVM uses the
       // address of this variable as label instead of its value.

void HI_ExtraCDFG::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.setPreservesAll();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequiredTransitive<ScalarEvolutionWrapperPass>();
}
