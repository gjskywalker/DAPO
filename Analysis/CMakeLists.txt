cmake_minimum_required(VERSION 3.12)

project(DAPOAnalysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -g")

set(LLVM_DIR "/usr/local/lib/cmake/llvm" CACHE PATH "Path to LLVMConfig.cmake")
set(Polly_DIR "/usr/local/lib/cmake/polly" CACHE PATH "Path to PollyConfig.cmake")

find_package(LLVM REQUIRED CONFIG)
find_package(Polly QUIET CONFIG)

set(_polly_targets_ok TRUE)
if(TARGET Polly)
  get_target_property(_loc Polly IMPORTED_LOCATION)
  if(NOT _loc)
    set(_polly_targets_ok FALSE)
  endif()
else()
  set(_polly_targets_ok FALSE)
endif()

if(Polly_FOUND AND _polly_targets_ok)
  set(Polly_LIBRARIES ${Polly_LIBRARIES})
else()
  message(WARNING "Polly config missing usable imported libraries; falling back to manual search.")
  find_library(POLLY_MAIN_LIB NAMES Polly HINTS /usr/local/lib)
  find_library(POLLY_ISL_LIB NAMES PollyISL HINTS /usr/local/lib)

  if(NOT POLLY_MAIN_LIB OR NOT POLLY_ISL_LIB)
    message(FATAL_ERROR "Could not locate required Polly libraries. Set POLLY_MAIN_LIB, POLLY_ISL_LIB manually.")
  endif()

  set(Polly_LIBRARIES ${POLLY_MAIN_LIB} ${POLLY_ISL_LIB})
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using PollyConfig.cmake in: ${Polly_DIR}")

include_directories(${LLVM_INCLUDE_DIRS} ${Polly_INCLUDE_DIRS})

file(GLOB _dapo_include_children RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/*")
foreach(_child ${_dapo_include_children})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/${_child}")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/${_child}")
  endif()
endforeach()

add_definitions(${LLVM_DEFINITIONS} ${Polly_DEFINITIONS})

llvm_map_components_to_libnames(DAPO_LLVM_COMPONENTS    
  analysis
  asmparser
  asmprinter
  bitreader
  bitwriter
  core
  coroutines
  irreader
  instcombine
  ipo
  linker
  objcarcopts
  profiledata
  scalaropts
  support
  target
  vectorize
)

set(DAPO_EXTRA_LLVM_LIBS
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
  LLVMCoroutines
  LLVMInstrumentation
  LLVMScalarOpts
  LLVMLinker
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
  LLVMBinaryFormat
  LLVMSupport
  LLVMDemangle
)

set(DAPO_CLANG_LIBS
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

set(DAPO_COMMON_LINK_LIBS
  ${DAPO_LLVM_COMPONENTS}
  ${DAPO_EXTRA_LLVM_LIBS}
  ${DAPO_CLANG_LIBS}
  ${Polly_LIBRARIES}
  pthread
  dl
)

add_subdirectory(lib)
add_subdirectory(tools)
