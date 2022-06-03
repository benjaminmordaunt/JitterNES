//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#ifndef UNTITLED1_LABELAST_H
#define UNTITLED1_LABELAST_H

#include <string>
#include <utility>
#include <map>

#include "llvm/IR/BasicBlock.h"
#include "ast/AddressableAST.h"
#include "cpu/CPUState.h"

/* Identifies a label marking a position in the assembly */
class LabelAST {
    std::string _name;
    std::shared_ptr<CPUState> _cpu;

public:
    explicit LabelAST(std::shared_ptr<CPUState> &cpu, std::string name) : _cpu(cpu), _name(std::move(name)) {}
    llvm::Value *codegen();

    static std::unique_ptr<LabelAST> ParseLabel(std::shared_ptr<CPUState> &cpu);
};

extern std::map<std::string, std::shared_ptr<AddressableAST>> LabelMap;

#endif //UNTITLED1_LABELAST_H
