# Install script for directory: /home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_FindFunctions/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_ASAPScheduling/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_print/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_DependenceList/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_SysExec/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_LoopInFormationCollect/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_Polly_Info/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_LoopDependenceAnalysis/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_SimpleTimingEvaluation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_GEP_OffsetCombine/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_DuplicateInstRm/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_VarWidthReduce/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_SeparateConstOffsetFromGEP/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_APIntSrcAnalysis/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_NoDirectiveTimingResourceEvaluation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_IntstructionMoveBackward/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_HLSDuplicateInstRm/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_AggressiveLSR_MUL/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_ArrayAccessPattern/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_WithDirectiveTimingResourceEvaluation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_StringProcess/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_FunctionInstantiation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_ReplaceSelectAccess/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_IR2SourceCode/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_LoopUnroll/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_RemoveRedundantAccess/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_LoopLabeler/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_FunctionInterfaceInfo/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_Mul2Shl/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_MulOrderOpt/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_FaddOrderOpt/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_MuxInsertionArrayPartition/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_LoadALAP/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_PragmaTargetExtraction/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_ArraySensitiveToLoopLevel/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/HI_TopLoop2Func/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/Implementations/HI_NoDirectiveTimingResourceEvaluation/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
