#include "LLVM_exp12_HI_NoDirectiveTimingResourceEvaluation.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"

using namespace llvm;
using namespace polly;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        errs() << "Usage: " << argv[0] << " <Top_Function_Name> <Config_File_Path> <test_File>\n";
        return 1;
    }

    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
    // Compile the source code into IR and Parse the input LLVM IR file into a module
    SMDiagnostic Err;
    LLVMContext Context;
    // std::string cmd_str = "clang -O1 -emit-llvm -S " + std::string(argv[1]) + " -o top.bc 2>&1";
    std::string top_str = std::string(argv[1]);
    std::string configFile_str = std::string(argv[2]);
    std::string test_File = std::string(argv[3]);
    // print_cmd(cmd_str.c_str());
    // bool result = sysexec(cmd_str.c_str());
    // assert(result);

    std::unique_ptr<Module> Mod(parseIRFile(test_File, Err, Context));
    if (!Mod)
    {
        Err.print(argv[0], errs());
        return 1;
    }

    // Create a pass manager and fill it with the passes we want to run.
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

    auto scopdetectionwrapperpass = new ScopDetectionWrapperPass();
    print_info("Enable ScopDetectionWrapperPass Pass");
    PM.add(scopdetectionwrapperpass);

    auto assumptioncachetracker = new AssumptionCacheTracker();
    print_info("Enable AssumptionCacheTracker Pass");
    PM.add(assumptioncachetracker);

    auto scopinfowrapperpass = new ScopInfoWrapperPass();
    print_info("Enable ScopInfoWrapperPass Pass");
    PM.add(scopinfowrapperpass);

    auto scopinforegionpass = new ScopInfoRegionPass();
    print_info("Enable ScopInfoRegionPass Pass");
    PM.add(scopinforegionpass);

    auto dependenceinfowrapperpass = new DependenceInfoWrapperPass();
    print_info("Enable DependenceInfoWrapperPass Pass");
    PM.add(dependenceinfowrapperpass);

    auto scalarevolutionwrapperpass = new ScalarEvolutionWrapperPass();
    PM.add(scalarevolutionwrapperpass);
    print_info("Enable ScalarEvolutionWrapperPass Pass");

    llvm::raw_fd_ostream OS2("top_output1.bc", EC, llvm::sys::fs::OF_None);
    WriteBitcodeToFile(*Mod, OS2);
    OS2.flush();

    auto hi_nodirectivetimingresourceevaluation = new HI_NoDirectiveTimingResourceEvaluation(
        configFile_str.c_str(), "HI_NoDirectiveTimingResourceEvaluation", "BRAM_info",
        top_str.c_str(), 1);
    print_info("Enable HI_NoDirectiveTimingResourceEvaluation Pass");
    PM.add(hi_nodirectivetimingresourceevaluation);

    print_info("Enable HI_FindFunctions Pass");
    print_info("Enable HI_DependenceList Pass");
    auto hi_findfunction = new HI_FindFunctions();
    PM.add(hi_findfunction);
    auto hi_dependencelist = new HI_DependenceList("Instructions", "Instruction_Dep");
    PM.add(hi_dependencelist);

    print_status("Start LLVM processing");
    PM.run(*Mod);
    print_status("Accomplished LLVM processing");

    assert(hi_nodirectivetimingresourceevaluation->topFunctionFound &&
           "The specified top function is not found in the program");

    print_status("Writing LLVM IR to File");

    llvm::raw_fd_ostream OS("top_output.bc", EC, llvm::sys::fs::OF_None);
    WriteBitcodeToFile(*Mod, OS);
    OS.flush();

    return 0;
}