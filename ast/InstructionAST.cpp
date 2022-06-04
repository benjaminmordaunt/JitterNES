//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#include <iostream>

#include "InstructionAST.h"
#include "lexer/token.h"
#include "llvm/IR/IRBuilder.h"

std::unique_ptr<InstructionAST> InstructionAST::ParseInstruction(std::shared_ptr<CPUState> &cpu) {
    // Look-up the instruction in the instruction map
    if (instr_lookup.count(IdentifierUpper) < 1) {
        std::cout << "Could not find instruction " << IdentifierUpper << std::endl;
        return nullptr;
    }

    auto inst_resolved = instr_lookup.at(IdentifierUpper);
    auto inst = std::make_unique<InstructionAST>(cpu, inst_resolved, instrs[inst_resolved]);

    // The correct addressing syntax must follow. If it doesn't, drop this instruction.
    // setAddressExpr returns false if it receives a nullptr as argument
    if (!inst->setAddressExpr(AddressingExprAST::ParseAddressingExpr(cpu, inst->getAddressingModes())))
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
    switch(_inst) {
        case ADC: {
            static auto zero = llvm::ConstantInt::get(_cpu->builder.getInt8Ty(), 0);

            auto a_reg = _cpu->builder.CreateLoad(_cpu->builder.getInt8Ty(), _cpu->rA);
            auto rscar_reg = _cpu->builder.CreateLoad(_cpu->builder.getInt1Ty(), _cpu->rSCar);
            auto mval = _addr->codegen();

            // Extend all registers to 16 bits so we can observe overflows and carries
            auto a_reg_ext = _cpu->builder.CreateZExt(a_reg, _cpu->builder.getInt16Ty());
            auto rscar_reg_ext = _cpu->builder.CreateZExt(rscar_reg, _cpu->builder.getInt16Ty());
            auto mval_ext = _cpu->builder.CreateZExt(mval, _cpu->builder.getInt16Ty());

            // Perform addition, truncate and store in accumulator
            auto result1 = _cpu->builder.CreateAdd(a_reg_ext, mval_ext);

            // Detect overflow
            // Overflow occurred when signs are 0 + 0 -> 1 or 1 + 1 -> 0.
            // i.e. signs of operands are the same, AND sign of result different from sign of one of the operands
            auto a_reg_sgn = _cpu->builder.CreateICmpSLT(a_reg, zero);
            auto mval_sgn = _cpu->builder.CreateICmpSLT(mval, zero);
            auto a_reg_mval_sgn_same = _cpu->builder.CreateICmpEQ(a_reg_sgn, mval_sgn);

            auto result2 = _cpu->builder.CreateAdd(result1, rscar_reg_ext);
            auto result2_trunc = _cpu->builder.CreateTrunc(result2, _cpu->builder.getInt8Ty());

            auto result2_sgn = _cpu->builder.CreateICmpSLT(result2_trunc, zero);
            auto a_reg_result2_sgn_diff = _cpu->builder.CreateICmpNE(a_reg_sgn, result2_sgn);
            auto overflow = _cpu->builder.CreateAnd(a_reg_mval_sgn_same, a_reg_result2_sgn_diff);

            _cpu->builder.CreateStore(result2_trunc, _cpu->rA);

            _cpu->statusUpdate(result2, Negative | Zero | Carry);
            _cpu->builder.CreateStore(overflow, _cpu->rSOvf);

            break;
        }
        case INX: {
            static auto inc_one = llvm::ConstantInt::get(_cpu->builder.getInt8Ty(), 1);

            auto x_reg = _cpu->builder.CreateLoad(_cpu->builder.getInt8Ty(), _cpu->rX);
            auto new_x_reg = _cpu->builder.CreateAdd(x_reg, inc_one);
            _cpu->builder.CreateStore(new_x_reg, _cpu->rX);

            _cpu->statusUpdate(new_x_reg, Negative | Zero);
            break;
        }
        default:
            break;
    }
}
