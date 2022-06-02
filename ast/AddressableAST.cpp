//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#include <iostream>

#include "AddressableAST.h"
#include "DirectiveAST.h"
#include "InstructionAST.h"
#include "lexer/token.h"

/// An addressable may be initialized memory using e.g. `.dw`, or
/// an assembler instruction.
std::unique_ptr<AddressableAST> AddressableAST::ParseAddressable() {
    getNextToken();

    // Attempt to process a directive
    if (CurToken == '.') {
        if (auto dir = DirectiveAST::ParseDirective()) {
            return std::move(dir);
        }

        std::cout << "Expected valid directive, found " << IdentifierString << std::endl;
        return nullptr;
    }

    // This is an instruction
    if (CurToken != tok_instruction) {
        std::cout << "Expected directive or instruction, found " << IdentifierString << std::endl;
        return nullptr;
    }

    if (auto inst = InstructionAST::ParseInstruction()) {
        return std::move(inst);
    }

    return nullptr;
}