//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#ifndef UNTITLED1_INSTRUCTIONAST_H
#define UNTITLED1_INSTRUCTIONAST_H

#include "AddressableAST.h"
#include "AddressingExprAST.h"
#include "lexer/token.h"

class InstructionAST : public AddressableAST {
    std::string _inst;
    std::vector<AddressingMode> _addr_modes;
    std::unique_ptr<AddressingExprAST> _addr;

public:
    InstructionAST(std::string inst, std::vector<AddressingMode> addr_modes) : _inst(std::move(inst)),
        _addr_modes(std::move(addr_modes)), _addr(nullptr) {}

    virtual llvm::Value *codegen() override;
    static std::unique_ptr<InstructionAST> ParseInstruction();

    bool setAddressExpr(std::unique_ptr<AddressingExprAST> addr_exp);
    std::vector<AddressingMode> &getAddressingModes();
};


#endif //UNTITLED1_INSTRUCTIONAST_H
