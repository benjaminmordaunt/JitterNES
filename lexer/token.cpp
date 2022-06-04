//
// Created by Benjamin Mordaunt on 30/05/2022.
//

#include <boost/algorithm/string.hpp>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "token.h"

std::string IdentifierString, IdentifierUpper;
long NumVal;
int CurToken;

/// A map containing all uppercase instruction mnemonics supported, as well as their
/// supported addressing modes.
std::map<Instruction6502, std::vector<std::pair<AddressingMode, unsigned char>>> instrs;
std::unordered_map<std::string, Instruction6502> const instr_lookup = {
        {"ADC", ADC},
        {"BEQ", BEQ},
        {"BNE", BNE},
        {"BCS", BCS},
        {"BCC", BCC},
        {"CPY", CPY},
        {"LDA", LDA},
        {"LDY", LDY},
        {"STA", STA},
        {"DEY", DEY},
        {"INX", INX},
        {"INY", INY},
        {"TYA", TYA},
        {"ORA", ORA}
};

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
            if (instr_lookup.count(IdentifierUpper) > 0) {
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
    instrs[ADC] = {{immediate, 0x69},
                     {zeropage, 0x65},
                     {zeropage_x, 0x75},
                     {absolute, 0x6D},
                     {absolute_x, 0x7D},
                     {absolute_y, 0x79},
                     {x_idx_indirect, 0x61},
                     {indirect_y_idx, 0x71}};

    instrs[BEQ] = {{relative, 0xF0}};

    instrs[BNE] = {{relative, 0xD0}};

    instrs[BCS] = {{relative, 0xB0}};

    instrs[BCC] = {{relative, 0x90}};

    instrs[CPY] = {{immediate, 0xC0},
                     {zeropage, 0xC4},
                     {absolute, 0xCC}};

    instrs[LDA] = {{immediate, 0xA9},
                     {zeropage, 0xA5},
                     {zeropage_x, 0xB5},
                     {absolute, 0xAD},
                     {absolute_x, 0xBD},
                     {absolute_y, 0xB9},
                     {x_idx_indirect, 0xA1},
                     {indirect_y_idx, 0xB1}};

    instrs[LDY] = {{immediate, 0xA0},
                     {zeropage, 0xA4},
                     {zeropage_x, 0xB4},
                     {absolute, 0xAC},
                     {absolute_x, 0xBC}};

    instrs[STA] = {{zeropage, 0x85},
                     {zeropage_x, 0x95},
                     {absolute, 0x8D},
                     {absolute_x, 0x9D},
                     {absolute_y, 0x99},
                     {x_idx_indirect, 0x81},
                     {indirect_y_idx, 0x91}};

    instrs[DEY] = {{implied, 0x88}};

    instrs[INX] = {{implied, 0xE8}};

    instrs[INY] = {{implied, 0xC8}};

    instrs[TYA] = {{implied, 0x98}};

    instrs[ORA] = {{immediate, 0x09},
                     {zeropage, 0x05},
                     {zeropage_x, 0x15},
                     {absolute, 0x0D},
                     {absolute_x, 0x1D},
                     {absolute_y, 0x19},
                     {x_idx_indirect, 0x01},
                     {indirect_y_idx, 0x11}};
}