cmake_minimum_required(VERSION 3.22)
project(JitterNES)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 14)

include_directories(${LLVM_INCLUDE_DIRS} .)
add_definitions(${LLVM_DEFINITIONS})

add_executable(untitled1 main.cpp lexer/token.cpp ast/LabelAST.cpp ast/AddressableAST.cpp ast/InstructionAST.cpp
        ast/AddressingExprAST.cpp ast/DirectiveAST.cpp cpu/CPUState.cpp cpu/CPUState.h)

llvm_map_components_to_libnames(llvm_libs core orcjit native)

target_link_libraries(untitled1 ${llvm_libs})