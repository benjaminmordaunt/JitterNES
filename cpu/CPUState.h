//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#ifndef JITTERNES_CPUSTATE_H
#define JITTERNES_CPUSTATE_H

#include "llvm/Support/Alignment.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

enum StatusRegisterBit : unsigned {
    Negative  = (1 << 7),
    Overflow  = (1 << 6),
    BreakHi   = (1 << 5),
    BreakLo   = (1 << 4),
    Decimal   = (1 << 3),
    Interrupt = (1 << 2),
    Zero      = (1 << 1),
    Carry     = (1 << 0)
};

class CPUState {
    llvm::Value *insertGlobalBit(const char *);
    llvm::Value *insertGlobal8(const char *);
    llvm::Value *insertGlobal16(const char *);

public:
    llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;

    llvm::Value* rA;  /* accumulator */
    llvm::Value* rX;  /* x register */
    llvm::Value* rY;  /* y register */
    llvm::Value* rSP; /* stack pointer */
    llvm::Value* rPC; /* program counter */

    llvm::Value* rSNeg;
    llvm::Value* rSOvf;
    llvm::Value* rSBrh;
    llvm::Value* rSBrl;
    llvm::Value* rSDec;
    llvm::Value* rSInt;
    llvm::Value* rSZer;
    llvm::Value* rSCar;

    llvm::Function *mainFn;

    CPUState();
    void statusUpdate(llvm::Value *val, unsigned bits);
};

#endif //JITTERNES_CPUSTATE_H
