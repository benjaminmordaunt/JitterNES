//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#include "DirectiveAST.h"

llvm::Value *DirectiveAST::codegen() {
    return nullptr;
}

std::unique_ptr<DirectiveAST> DirectiveAST::ParseDirective() {
    return std::unique_ptr<DirectiveAST>();
}
