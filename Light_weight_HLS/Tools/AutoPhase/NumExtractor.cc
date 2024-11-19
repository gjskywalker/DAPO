#include "HI_DependenceList.h"
#include "HI_DuplicateInstRm.h"
#include "HI_FindFunction.h"
#include "HI_LoopInFormationCollect.h"
#include "HI_SeparateConstOffsetFromGEP.h"
#include "HI_SysExec.h"
#include "HI_print.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/ScopeExit.h"
#include "llvm/ADT/SetOperations.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/InstructionSimplify.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/MustExecute.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionAliasAnalysis.h"

#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Analysis/DomTreeUpdater.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Scalar/SeparateConstOffsetFromGEP.h"
#include "llvm/Transforms/IPO/Annotation2Metadata.h"
#include "llvm/Transforms/IPO/ForceFunctionAttrs.h"
#include "llvm/Transforms/IPO/InferFunctionAttrs.h"
#include <ios>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/SourceMgr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
// #include "polly/LinkAllPasses.h"
#include "HI_LoopDependenceAnalysis.h"
#include "HI_NoDirectiveTimingResourceEvaluation.h"
#include "HI_Polly_Info.h"
#include "HI_SimpleTimingEvaluation.h"
#include "HI_VarWidthReduce.h"
#include "polly/PolyhedralInfo.h"
#include "llvm-c/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
// #include "X86TargetMachine.h"
#include "HI_ExtraNum.h"

using namespace llvm;
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        errs() << "Usage: " << argv[0] << "<Inputfile>\n";
    }

    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
    SMDiagnostic Err;
    LLVMContext Context;
    std::string Inputfile;
    Inputfile = argv[1];
    std::unique_ptr<Module> Mod(parseIRFile(Inputfile, Err, Context));
    if (!Mod)
    {
        Err.print(argv[0], errs());
        return 1;
    }
    legacy::PassManager PM, PM1;
    LLVMTargetRef T;

    char *Error;

    if (LLVMGetTargetFromTriple((Mod->getTargetTriple()).c_str(), &T, &Error))
    {
        print_error(Error);
    }
    else
    {
        std::string targetname = LLVMGetTargetName(T);
        targetname = "The target machine is: " + targetname;
        print_info(targetname.c_str());
    }

    Triple ModuleTriple(Mod->getTargetTriple());
    TargetLibraryInfoImpl TLII(ModuleTriple);
    PM1.add(new TargetLibraryInfoWrapperPass(TLII));

    PM1.add(createTargetTransformInfoWrapperPass(TargetIRAnalysis()));
    print_info("Enable TargetIRAnalysis Pass");

    auto hi_separateconstoffsetfromgep =
        new HI_SeparateConstOffsetFromGEP("HI_SeparateConstOffsetFromGEP", true);
    PM1.add(hi_separateconstoffsetfromgep);
    print_info("Enable HI_SeparateConstOffsetFromGEP Pass");

    // auto hi_duplicateinstrm = new HI_DuplicateInstRm("rmInsts");
    // PM1.add(hi_duplicateinstrm);
    // print_info("Enable HI_DuplicateInstRm Pass");

    PM1.run(*Mod);

    std::error_code EC;
    llvm::raw_fd_ostream OS1("top_output0.bc", EC, llvm::sys::fs::OF_None);
    WriteBitcodeToFile(*Mod, OS1);
    OS1.flush();

    /*
        We use the following passes to create canonical loop form to simplify trip count analysis.
        https://llvm.org/docs/LoopTerminology.html
    */
    print_info("Enable LoopSimplify Pass");
    auto loopsimplifypass = createLoopSimplifyPass();
    PM.add(loopsimplifypass);

    auto loopstrengthreducepass = createLoopStrengthReducePass();
    PM.add(loopstrengthreducepass);
    print_info("Enable LoopStrengthReducePass Pass");

    auto looprotatepass = createLoopRotatePass();
    PM.add(looprotatepass);

    auto unifyloopexitspass = createUnifyLoopExitsPass();
    PM.add(unifyloopexitspass);

    auto loopsimplifycfgpass = createLoopSimplifyCFGPass();
    PM.add(loopsimplifycfgpass);

    print_info("Enable Mem2Reg Pass");
    auto mem2regpass = createPromoteMemoryToRegisterPass();
    PM.add(mem2regpass);

    // auto hi_duplicateinstrm = new HI_DuplicateInstRm("rmInsts");
    // PM.add(hi_duplicateinstrm);
    // print_info("Enable HI_DuplicateInstRm Pass");

    auto loopinfowrapperpass = new LoopInfoWrapperPass();
    PM.add(loopinfowrapperpass);
    print_info("Enable LoopInfoWrapperPass Pass");

    auto regioninfopass = new RegionInfoPass();
    PM.add(regioninfopass);
    print_info("Enable RegionInfoPass Pass");

    auto dominatortreewrapperpass = new DominatorTreeWrapperPass();
    PM.add(dominatortreewrapperpass);
    print_info("Enable DominatorTreeWrapperPass Pass");

    auto optimizationremarkemitterwrapperpass = new OptimizationRemarkEmitterWrapperPass();
    PM.add(optimizationremarkemitterwrapperpass);
    print_info("Enable OptimizationRemarkEmitterWrapperPass Pass");

    auto aaresultswrapperpass = new AAResultsWrapperPass();
    print_info("Enable AAResultsWrapperPass Pass");
    PM.add(aaresultswrapperpass);

    auto assumptioncachetracker = new AssumptionCacheTracker();
    print_info("Enable AssumptionCacheTracker Pass");
    PM.add(assumptioncachetracker);

    auto scalarevolutionwrapperpass = new ScalarEvolutionWrapperPass();
    PM.add(scalarevolutionwrapperpass);
    print_info("Enable ScalarEvolutionWrapperPass Pass");

    llvm::raw_fd_ostream OS2("top_output1.bc", EC, llvm::sys::fs::OF_None);
    WriteBitcodeToFile(*Mod, OS2);
    OS2.flush();

    HI_ExtraNum* extranum = new HI_ExtraNum();
    PM.add(extranum);
    print_info("ExtractNum BEGIN:");
    PM.run(*Mod);
    print_info("ExtractNum END.");
    llvm::raw_fd_ostream OS3("top_output.bc", EC, llvm::sys::fs::OF_None);
    WriteBitcodeToFile(*Mod, OS3);
    OS3.flush();
    return 0;
}