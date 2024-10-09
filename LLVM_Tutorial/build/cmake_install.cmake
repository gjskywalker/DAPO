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
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  set(CMAKE_OBJDUMP "/usr/local/bin/llvm-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_FindFunctions/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_ASAPScheduling/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_print/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_DependenceList/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_SysExec/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_LoopInFormationCollect/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_Polly_Info/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_LoopDependenceAnalysis/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_SimpleTimingEvaluation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_GEP_OffsetCombine/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_DuplicateInstRm/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_VarWidthReduce/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_SeparateConstOffsetFromGEP/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_APIntSrcAnalysis/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_NoDirectiveTimingResourceEvaluation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_IntstructionMoveBackward/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_HLSDuplicateInstRm/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_AggressiveLSR_MUL/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_ArrayAccessPattern/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_WithDirectiveTimingResourceEvaluation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_StringProcess/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_FunctionInstantiation/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_ReplaceSelectAccess/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_IR2SourceCode/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_LoopUnroll/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_RemoveRedundantAccess/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_LoopLabeler/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_FunctionInterfaceInfo/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_Mul2Shl/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_MulOrderOpt/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_FaddOrderOpt/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_MuxInsertionArrayPartition/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_LoadALAP/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_PragmaTargetExtraction/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_ArraySensitiveToLoopLevel/cmake_install.cmake")
  include("/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/HI_TopLoop2Func/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/eeuser/Desktop/Light_HLS/LLVM_Tutorial/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
