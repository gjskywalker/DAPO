#include "FeatureExtractor.h"

using namespace llvm;
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        errs() << "Usage: " << argv[0] << "<path> <pgm_name> <Inputfile>\n";
    }

    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
    SMDiagnostic Err;
    LLVMContext Context;
    std::string path = std::string(argv[1]);
    std::string Inputfile = std::string(argv[3]);
    std::string pgm_name = std::string(argv[2]);
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

    HI_ExtraCDFG *hi_extracdfg = new HI_ExtraCDFG(pgm_name, path, true);
    PM.add(hi_extracdfg);
    print_info("FeatureExtract BEGIN:");
    PM.run(*Mod);
    print_info("FeatureExtract END.");
    llvm::raw_fd_ostream OS3("top_output.bc", EC, llvm::sys::fs::OF_None);
    WriteBitcodeToFile(*Mod, OS3);
    OS3.flush();
    return 0;
}