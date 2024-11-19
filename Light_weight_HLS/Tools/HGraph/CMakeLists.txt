project(FeatureExtractor) 

set(Outputname "FeatureExtractor_V5")

cmake_minimum_required(VERSION 3.10) 

find_package(LLVM REQUIRED CONFIG) 
find_package(Polly REQUIRED CONFIG) 

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}") 
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}") 

message(STATUS "Using PollyConfig.cmake in: ${Polly_DIR}")
 
# Needed to use support library 
message(STATUS "C++ flags==== ${CMAKE_CXX_FLAGS}") 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 -g") 

include_directories(${LLVM_INCLUDE_DIRS}) 
include_directories(${Polly_INCLUDE_DIRS}) 

include_directories(./ 
../../Implementations/HI_ExtraCDFG
../../Implementations/HI_FindFunctions 
../../Implementations/HI_print 
../../Implementations/HI_LoopInFormationCollect 
../../Implementations/HI_DependenceList 
../../Implementations/HI_SysExec 
../../Implementations/HI_StringProcess 
../../Implementations/HI_Polly_Info 
../../Implementations/HI_LoopDependenceAnalysis
../../Implementations/HI_SimpleTimingEvaluation
../../Implementations/HI_DuplicateInstRm
../../Implementations/HI_VarWidthReduce
../../Implementations/HI_SeparateConstOffsetFromGEP
../../Implementations/HI_NoDirectiveTimingResourceEvaluation
)

link_directories("../LLVM_Learner_Libs/build/building")
set(MAKE_CMD "../../../BuildLibs.sh")
MESSAGE("COMMAND: ${MAKE_CMD} ")
execute_process(COMMAND ${MAKE_CMD}
   RESULT_VARIABLE CMD_ERROR
      OUTPUT_FILE CMD_OUTPUT)
add_definitions(${LLVM_DEFINITIONS}) 
add_definitions(${Polly_DEFINITIONS}) 

add_executable(${Outputname} FeatureExtractor.cc) 

llvm_map_components_to_libnames(llvm_libs support core irreader engine nativecodegen) 

target_link_libraries(${Outputname} pthread dl 
HI_ExtraCDFG
HI_print
HI_SeparateConstOffsetFromGEP
HI_DuplicateInstRm
) #link llvm component 

target_link_libraries(${Outputname} ${Polly_LIBRARIES})

message(STATUS "Polly Libraries are: ${Polly_LIBRARIES}") 


# set(llvm_libs "")

#link llvm library 
target_link_libraries(${Outputname} 
LLVMLTO 
LLVMPasses 
LLVMObjCARCOpts 
LLVMMIRParser 
LLVMSymbolize 
LLVMDebugInfoPDB 
LLVMDebugInfoDWARF 
LLVMCoverage 
LLVMMCA 
LLVMTableGen 
LLVMDlltoolDriver 
LLVMXRay 
LLVMOrcJIT 
LLVMXCoreDisassembler 
LLVMXCoreCodeGen 
LLVMXCoreDesc 
LLVMXCoreInfo 
LLVMObjectYAML 
LLVMLibDriver 
LLVMOption 
LLVMWindowsManifest 
LLVMTextAPI 
LLVMFuzzMutate 
LLVMX86Disassembler 
LLVMX86AsmParser 
LLVMX86CodeGen 
LLVMGlobalISel 
LLVMSelectionDAG 
LLVMAsmPrinter 
LLVMX86Desc 
LLVMMCDisassembler 
LLVMX86Info 
LLVMMCJIT 
LLVMInterpreter 
LLVMExecutionEngine 
LLVMRuntimeDyld 
LLVMCodeGen 
LLVMTarget 
LLVMCoroutines 
LLVMipo 
LLVMInstrumentation 
LLVMVectorize 
LLVMScalarOpts 
LLVMLinker 
LLVMIRReader 
LLVMAsmParser 
LLVMInstCombine 
LLVMBitWriter 
LLVMAggressiveInstCombine 
LLVMTransformUtils 
LLVMAnalysis 
LLVMProfileData 
LLVMObject 
LLVMMCParser 
LLVMMC 
LLVMDebugInfoCodeView 
LLVMDebugInfoMSF 
LLVMBitReader 
LLVMCore 
LLVMBinaryFormat 
LLVMSupport 
LLVMDemangle
clangFrontend 
clangParse 
clangSema 
clangAnalysis 
clangAST 
clangLex 
clangBasic 
clangDriver 
clangSerialization 
clangFrontendTool
clangCodeGen 
clangStaticAnalyzerFrontend 
clangStaticAnalyzerCheckers 
clangStaticAnalyzerCore 
clangRewrite 
clangEdit 
clangTooling
clangARCMigrate
clangRewriteFrontend
) 