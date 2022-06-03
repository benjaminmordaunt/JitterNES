//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#ifndef UNTITLED1_ADDRESSABLEAST_H
#define UNTITLED1_ADDRESSABLEAST_H

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/BasicBlock.h"

class AddressableAST {
    unsigned int _addr;
public:
    virtual llvm::Value *codegen() = 0;
    static std::unique_ptr<AddressableAST> ParseAddressable();
    virtual ~AddressableAST() = default;
};

#endif //UNTITLED1_ADDRESSABLEAST_H
