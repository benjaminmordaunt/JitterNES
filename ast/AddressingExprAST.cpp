//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#include <iostream>
#include "AddressingExprAST.h"
#include "LabelAST.h"

/// Currently, only accept labels, immediate values and absolute addresses. This should be enough
/// to parse a basic disassembled example.
std::unique_ptr<AddressingExprAST> AddressingExprAST::ParseAddressingExpr(std::shared_ptr<CPUState> &cpu, const std::vector<AddressingMode> &modes) {
    if (std::count(modes.begin(), modes.end(), implied) > 0)
        return std::make_unique<AddressingExprAST>(cpu);

    // Eat the instruction word.
    getNextToken();

    switch (CurToken) {
        case tok_immediate:
            if (std::count(modes.begin(), modes.end(), immediate) < 1)
                return nullptr;
            std::cout << "Building immediate addressing expression, with value " << NumVal << std::endl;
            return std::make_unique<AddressingExprAST>(cpu, NumVal, immediate_ast);
        case tok_absolute:
            if (std::count(modes.begin(), modes.end(), absolute) < 1)
                return nullptr;
            std::cout << "Building absolute addressing expression, with value " << NumVal << std::endl;
            return std::make_unique<AddressingExprAST>(cpu, NumVal, absolute_ast);
        case tok_identifier:
            if (std::count(modes.begin(), modes.end(), absolute) < 1)
                return nullptr;
            // If this label does not yet exist, create it, unbound, within LabelMap.
            // When a label is created later in the module, update the binding using LabelAST::UpdateBinding.
            // If a key is present within LabelMap without a binding at end of parsing, raise an error.

            std::cout << "Building label addressing expression to label " << IdentifierString << std::endl;
            return std::make_unique<AddressingExprAST>(cpu, LabelMap[IdentifierString]);
        default:
            break;
    }

    return nullptr;
}

llvm::Value *AddressingExprAST::codegen() const {
    switch(_type) {
        case immediate_ast:
            return llvm::ConstantInt::get(_cpu->builder.getInt8Ty(), _val);
        default:
            std::cerr << "AddressingExpr type not yet implemented." << std::endl;
            return nullptr;
    }
}
