//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#ifndef UNTITLED1_ADDRESSINGEXPRAST_H
#define UNTITLED1_ADDRESSINGEXPRAST_H

#include <vector>
#include "token.h"

/// For now, only accept the token '0' as a test.
class AddressingExprAST {
public:
    static std::unique_ptr<AddressingExprAST> ParseAddressingExpr(const std::vector<AddressingMode> &);
    AddressingExprAST() = default;
};


#endif //UNTITLED1_ADDRESSINGEXPRAST_H
