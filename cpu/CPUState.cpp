//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#include <iostream>
#include "CPUState.h"

llvm::Value *CPUState::insertGlobalBit(const char *name) {
    module->getOrInsertGlobal(name, builder.getInt1Ty());
    return module->getNamedGlobal(name);
}

llvm::Value *CPUState::insertGlobal8(const char *name) {
    module->getOrInsertGlobal(name, builder.getInt8Ty());
    return module->getNamedGlobal(name);
}

llvm::Value *CPUState::insertGlobal16(const char *name) {
    module->getOrInsertGlobal(name, builder.getInt16Ty());
    return module->getNamedGlobal(name);
}

CPUState::CPUState() : builder(ctx) {
    module = std::make_unique<llvm::Module>("JitterNES", ctx);

    rA = insertGlobal8("_nes_a");
    rX = insertGlobal8("_nes_x");
    rY = insertGlobal8("_nes_y");
    rSP = insertGlobal8("_nes_sp");
    rPC = insertGlobal16("_nes_pc");

    rSNeg = insertGlobalBit("_nes_sneg");
    rSOvf = insertGlobalBit("_nes_sovf");
    rSBrh = insertGlobalBit("_nes_sbrh");
    rSBrl = insertGlobalBit("_nes_sbrl");
    rSDec = insertGlobalBit("_nes_sdec");
    rSInt = insertGlobalBit("_nes_sint");
    rSZer = insertGlobalBit("_nes_szer");
    rSCar = insertGlobalBit("_nes_scar");
}

void CPUState::statusUpdate(llvm::Value *val, unsigned int bits) {
    if (bits & Negative) {
        static auto bit7 = llvm::ConstantInt::get(builder.getInt8Ty(), 0x80);
        auto sneg_and = builder.CreateAnd(val, bit7);
        auto sneg_cmp = builder.CreateICmpEQ(sneg_and, bit7);
        builder.CreateStore(sneg_cmp, rSNeg);
    }
    if (bits & Zero) {
        static auto zero = llvm::ConstantInt::get(builder.getInt8Ty(), 0);
        auto szer_cmp = builder.CreateICmpEQ(val, zero);
        builder.CreateStore(szer_cmp, rSZer);
    }
}
