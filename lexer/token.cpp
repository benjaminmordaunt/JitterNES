//
// Created by Benjamin Mordaunt on 30/05/2022.
//

#include <boost/algorithm/string.hpp>
#include <map>
#include <vector>
#include <iostream>
#include "token.h"

std::string IdentifierString, IdentifierUpper;
long NumVal;
int CurToken;

/// A map containing all uppercase instruction mnemonics supported, as well as their
/// supported addressing modes.
std::map<std::string, std::vector<AddressingMode>> instrs;

/// gettok - return the next token from standard input.
int gettok() {
    static int LastChar = ' ';

    // Skip any whitespace
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) {
        IdentifierString = LastChar;
        while (isalnum(LastChar = getchar()))
            IdentifierString += LastChar;

        // Check whether this is a recognised instruction word
        // Use search and map each character to upper-case
        for (auto &instr : instrs) {
            IdentifierUpper = boost::to_upper_copy<std::string>(IdentifierString);
            if (instrs.count(IdentifierUpper) > 0) {
                std::cout << "Emitting tok_instruction: " << IdentifierUpper << std::endl;
                return tok_instruction;
            }
        }

        // Otherwise we have just created an arbitrary identifier
        std::cout << "Emitting tok_identifier: " << IdentifierString << std::endl;
        return tok_identifier;
    }

    // Handle immediate value
    if (LastChar == '#') {
        // Switch to hexadecimal mode?
        if ((LastChar = getchar()) == '$') {
            if (isxdigit(LastChar = getchar())) {
                std::string XNumStr;
                do {
                    XNumStr += LastChar;
                    LastChar = getchar();
                } while (isxdigit(LastChar));

                NumVal = strtol(XNumStr.c_str(), nullptr, 16);
                std::cout << "Emitting tok_immediate: " << NumVal << std::endl;
                return tok_immediate;
            } else {
                std::cout << "Expected hex digit, found " << LastChar << std::endl;
            }
        }

        // Handle as a decimal
        if (isdigit(LastChar)) {
            std::string NumStr;
            do {
                NumStr += LastChar;
                LastChar = getchar();
            } while (isdigit(LastChar));

            NumVal = strtol(NumStr.c_str(), nullptr, 10);
            std::cout << "Emitting tok_immediate: " << NumVal << std::endl;
            return tok_immediate;
        } else {
            std::cout << "Expected decimal digit, found " << LastChar << std::endl;
        }
    }

    // Handle absolute address
    if (LastChar == '$') {
        if (isxdigit(LastChar)) {
            std::string XNumStr;
            do {
                XNumStr += LastChar;
                LastChar = getchar();
            } while (isxdigit(LastChar));

            NumVal = strtol(XNumStr.c_str(), nullptr, 16);
            std::cout << "Emitting tok_absolute: " << NumVal << std::endl;
            return tok_absolute;
        }
    }

    // Have we reached the end of file?
    if (LastChar == EOF)
        return tok_eof;

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int getNextToken() {
    return (CurToken = gettok());
}

void populateInstructions() {
    instrs["ADC"] = {immediate, zeropage, zeropage_x, absolute, absolute_x, absolute_y, x_idx_indirect, indirect_y_idx};
    instrs["LDA"] = {immediate, zeropage, zeropage_x, absolute, absolute_x, absolute_y, x_idx_indirect, indirect_y_idx};
    instrs["STA"] = {zeropage, zeropage_x, absolute, absolute_x, absolute_y, x_idx_indirect, indirect_y_idx};
}