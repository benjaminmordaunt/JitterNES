//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#include <iostream>

#include "InstructionAST.h"
#include "lexer/token.h"
#include "llvm/IR/IRBuilder.h"

std::unique_ptr<InstructionAST> InstructionAST::ParseInstruction(std::shared_ptr<CPUState> &cpu) {
    // Look-up the instruction in the instruction map
    if (instrs.count(IdentifierUpper) < 1) {
        std::cout << "Could not find instruction " << IdentifierUpper << std::endl;
        return nullptr;
    }

    auto inst = std::make_unique<InstructionAST>(cpu, IdentifierUpper, instrs[IdentifierUpper]);

    // The correct addressing syntax must follow. If it doesn't, drop this instruction.
    // setAddressExpr returns false if it receives a nullptr as argument
    if (!inst->setAddressExpr(AddressingExprAST::ParseAddressingExpr(inst->getAddressingModes())))
        return nullptr;

    return std::move(inst);
}

bool InstructionAST::setAddressExpr(std::unique_ptr<AddressingExprAST> addr_exp) {
    if (addr_exp == nullptr)
        return false;

    _addr = std::move(addr_exp);
    return true;
}

/// This function is used for parsing purposes only, which doesn't care about
/// the instruction opcode. Therefore, shrink the vector.
std::vector<AddressingMode> InstructionAST::getAddressingModes() {
    std::vector<AddressingMode> addr_modes;
    std::transform(_addr_modes.begin(), _addr_modes.end(),
                   std::back_inserter(addr_modes),
                   [](const std::pair<AddressingMode, unsigned int> &p) { return p.first; });

    return std::move(addr_modes);
}

llvm::Value *InstructionAST::codegen() {
    // Instruction lookup clearly needs improving. String comparison is no good here.
    if (_inst == "INX") {
        static auto inc_one = llvm::ConstantInt::get(_cpu->builder.getInt8Ty(), 1);

        auto x_reg = _cpu->builder.CreateLoad(_cpu->builder.getInt8Ty(), _cpu->rX);
        auto new_x_reg = _cpu->builder.CreateAdd(x_reg, inc_one);
        _cpu->builder.CreateStore(new_x_reg, _cpu->rX);

        _cpu->statusUpdate(x_reg, Zero | Negative);
    }
}
