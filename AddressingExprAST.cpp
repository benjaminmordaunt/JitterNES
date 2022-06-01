//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#include <iostream>
#include "AddressingExprAST.h"

std::unique_ptr<AddressingExprAST> AddressingExprAST::ParseAddressingExpr(const std::vector<AddressingMode> &modes) {
    if (NumVal != 0) {
        std::cout << "Invalid input for expected addressing mode." << std::endl;
        return nullptr;
    }

    return std::make_unique<AddressingExprAST>();
}
