//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#include <iostream>

#include "InstructionAST.h"
#include "token.h"

std::unique_ptr<InstructionAST> InstructionAST::ParseInstruction() {
    // Look-up the instruction in the instruction map
    if (instrs.count(IdentifierUpper) < 1) {
        std::cout << "Could not find instruction " << IdentifierUpper << std::endl;
        return nullptr;
    }

    auto inst = std::make_unique<InstructionAST>(IdentifierUpper, instrs[IdentifierUpper]);

    // Eat the instruction word.
    getNextToken();

    // The correct addressing syntax must follow. If it doesn't, drop this instruction.
    // setAddressExpr returns false if it receives a nullptr as argument
    if (!inst->setAddressExpr(AddressingExprAST::ParseAddressingExpr(inst->getAddressingModes())))
        return nullptr;

    return inst;
}

bool InstructionAST::setAddressExpr(std::unique_ptr<AddressingExprAST> addr_exp) {
    if (addr_exp == nullptr)
        return false;

    _addr = std::move(addr_exp);
    return true;
}

std::vector<AddressingMode> &InstructionAST::getAddressingModes() {
    return _addr_modes;
}

llvm::Value *InstructionAST::codegen() {
    return nullptr;
}
