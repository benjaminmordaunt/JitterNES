//
// Created by Benjamin Mordaunt on 30/05/2022.
//

#ifndef UNTITLED1_TOKEN_H
#define UNTITLED1_TOKEN_H

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

enum Token {
    // Line ending
    tok_eof = -1,

    // Primary
    tok_immediate = -2,
    tok_absolute = -3,
    tok_identifier = -4,
    tok_instruction = -5,
};

enum Instruction6502 {
    ADC,
    BEQ,
    BNE,
    BCS,
    BCC,
    CPY,
    LDA,
    LDY,
    STA,
    DEY,
    INX,
    INY,
    TYA,
    ORA
};

enum AddressingMode {
    accumulator = 0,
    absolute = 1,
    absolute_x = 2,
    absolute_y = 3,
    immediate = 4,
    implied = 5,
    indirect = 6,
    x_idx_indirect = 7,
    indirect_y_idx = 8,
    relative = 9,
    zeropage = 10,
    zeropage_x = 11,
    zeropage_y = 12
};

extern std::map<Instruction6502, std::vector<std::pair<AddressingMode, unsigned char>>> instrs;
extern std::unordered_map<std::string, Instruction6502> const instr_lookup;

int gettok();
int getNextToken();
void populateInstructions();

extern int CurToken;
extern std::string IdentifierString;
extern std::string IdentifierUpper;
extern long NumVal;

#endif //UNTITLED1_TOKEN_H
