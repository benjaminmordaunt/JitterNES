//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#ifndef UNTITLED1_DIRECTIVEAST_H
#define UNTITLED1_DIRECTIVEAST_H

#include "AddressableAST.h"

class DirectiveAST : public AddressableAST {
public:
    static std::unique_ptr<DirectiveAST> ParseDirective();

    virtual llvm::Value *codegen() override;
    explicit DirectiveAST() = default;

};


#endif //UNTITLED1_DIRECTIVEAST_H
