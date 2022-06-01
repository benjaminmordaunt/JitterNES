//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#ifndef UNTITLED1_LABELAST_H
#define UNTITLED1_LABELAST_H

#include <string>
#include <utility>
#include <map>

#include "llvm/IR/BasicBlock.h"
#include "AddressableAST.h"

/* Identifies a label marking a position in the assembly */
class LabelAST {
    std::string _name;

public:
    explicit LabelAST(std::string name) : _name(std::move(name)) {}
    llvm::Value *codegen();

    static std::unique_ptr<LabelAST> ParseLabel();
};

extern std::map<std::string, std::unique_ptr<AddressableAST>> LabelMap;

#endif //UNTITLED1_LABELAST_H
