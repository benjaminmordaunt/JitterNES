//
// Created by Benjamin Mordaunt on 30/05/2022.
//

#include <boost/algorithm/string.hpp>
#include <map>
#include <vector>
#include "token.h"

std::string IdentifierString, IdentifierUpper;
double NumVal;
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
            if (instrs.count(IdentifierUpper) > 0)
                return tok_instruction;
        }

        // Otherwise we have just created an arbitrary identifier
        return tok_identifier;
    }

    // Is a number of some kind
    if (isdigit(LastChar)) {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    // Is a comment? Skip the rest of the line
    if (LastChar == '#') {
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        // Produce the next token
        if (LastChar != EOF)
            return gettok();
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