//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#ifndef UNTITLED1_INSTRUCTIONAST_H
#define UNTITLED1_INSTRUCTIONAST_H

#include <llvm/IR/IRBuilder.h>
#include "AddressableAST.h"
#include "AddressingExprAST.h"
#include "lexer/token.h"
#include "cpu/CPUState.h"

class InstructionAST : public AddressableAST {
    std::string _inst;
    std::shared_ptr<CPUState> _cpu;
    std::vector<std::pair<AddressingMode, unsigned char>> &_addr_modes;
    std::unique_ptr<AddressingExprAST> _addr;

public:
    InstructionAST(std::shared_ptr<CPUState> &cpu, std::string inst,
                   std::vector<std::pair<AddressingMode, unsigned char>> &addr_modes) : _inst(std::move(inst)),
        _cpu(cpu), _addr_modes(addr_modes), _addr(nullptr) {}

    llvm::Value *codegen() override;
    static std::unique_ptr<InstructionAST> ParseInstruction(std::shared_ptr<CPUState> &);

    bool setAddressExpr(std::unique_ptr<AddressingExprAST> addr_exp);
    std::vector<AddressingMode> getAddressingModes();
};


#endif //UNTITLED1_INSTRUCTIONAST_H
