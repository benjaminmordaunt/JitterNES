//
// Created by Benjamin Mordaunt on 30/05/2022.
//

#ifndef UNTITLED1_TOKEN_H
#define UNTITLED1_TOKEN_H

#include <map>
#include <string>
#include <vector>

enum Token {
    // Line ending
    tok_eof = -1,

    // Primary
    tok_number = -2,
    tok_identifier = -3,
    tok_instruction = -4,
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

extern std::map<std::string, std::vector<AddressingMode>> instrs;
int gettok();
int getNextToken();
void populateInstructions();

extern int CurToken;
extern std::string IdentifierString;
extern std::string IdentifierUpper;
extern double NumVal;

#endif //UNTITLED1_TOKEN_H
