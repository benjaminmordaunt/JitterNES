//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#ifndef UNTITLED1_ADDRESSINGEXPRAST_H
#define UNTITLED1_ADDRESSINGEXPRAST_H

#include <vector>
#include "lexer/token.h"
#include "AddressableAST.h"

/// Indicates what the value in _val means. Is it an absolute value, an immediate?
enum AddressingExprASTType {
    absolute_ast,
    immediate_ast,
    unused_ast
};

/// For now, only accept the token '0' as a test.
class AddressingExprAST {
    long _val;
    AddressingExprASTType _type;
    std::shared_ptr<AddressableAST> _addr_ast;

public:
    static std::unique_ptr<AddressingExprAST> ParseAddressingExpr(const std::vector<AddressingMode> &);

    // TODO: Split these up into separate classes.
    // An empty addressing expression for implied operations.
    explicit AddressingExprAST() : _val(0L), _type(unused_ast), _addr_ast(nullptr) {}

    // An addressing expression that references an immediate, or an absolute.
    explicit AddressingExprAST(long imm, AddressingExprASTType type) : _val(imm), _type(type),
        _addr_ast(nullptr) {}

    // An addressing expression that references a label
    explicit AddressingExprAST(std::shared_ptr<AddressableAST> &addr_ast) : _val(0L),
        _type(unused_ast), _addr_ast(addr_ast) {}
};


#endif //UNTITLED1_ADDRESSINGEXPRAST_H
