//
// Created by Benjamin Mordaunt on 02/06/2022.
//

#include <iostream>
#include "CPUState.h"

llvm::Value *CPUState::insertGlobalBit(const char *name) {
    module->getOrInsertGlobal(name, builder.getInt1Ty());
    auto glb = module->getNamedGlobal(name);
    glb->setLinkage(llvm::GlobalValue::CommonLinkage);
    glb->setAlignment(llvm::MaybeAlign(4));
    glb->setInitializer(llvm::ConstantInt::get(glb->getValueType(), 0));
    return glb;
}

llvm::Value *CPUState::insertGlobal8(const char *name) {
    module->getOrInsertGlobal(name, builder.getInt8Ty());
    auto glb = module->getNamedGlobal(name);
    glb->setLinkage(llvm::GlobalValue::CommonLinkage);
    glb->setAlignment(llvm::MaybeAlign(4));
    glb->setInitializer(llvm::ConstantInt::get(glb->getValueType(), 0));
    return glb;
}

llvm::Value *CPUState::insertGlobal16(const char *name) {
    module->getOrInsertGlobal(name, builder.getInt16Ty());
    auto glb = module->getNamedGlobal(name);
    glb->setLinkage(llvm::GlobalValue::CommonLinkage);
    glb->setAlignment(llvm::MaybeAlign(4));
    glb->setInitializer(llvm::ConstantInt::get(glb->getValueType(), 0));
    return glb;
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

    auto main_fn_ty = llvm::FunctionType::get(builder.getVoidTy(), false);
    mainFn = llvm::Function::Create(main_fn_ty, llvm::GlobalValue::ExternalLinkage,
                                          "main", *module);
    auto main_fn_blk = llvm::BasicBlock::Create(ctx, "entry", mainFn);
    builder.SetInsertPoint(main_fn_blk);
}

void CPUState::statusUpdate(llvm::Value *val, unsigned int bits) {
    // May be passed 8-bit or 16-bit values
    if (bits & Negative) {
        static auto bit7 = llvm::ConstantInt::get(val->getType(), 0x80);
        auto sneg_and = builder.CreateAnd(val, bit7);
        auto sneg_cmp = builder.CreateICmpEQ(sneg_and, bit7);
        builder.CreateStore(sneg_cmp, rSNeg);
    }
    if (bits & Zero) {
        static auto zero = llvm::ConstantInt::get(val->getType(), 0);
        auto szer_cmp = builder.CreateICmpEQ(val, zero);
        builder.CreateStore(szer_cmp, rSZer);
    }
    if (bits & Carry) {
        static auto bit8 = llvm::ConstantInt::get(builder.getInt16Ty(), 0x100);
        auto scar_and = builder.CreateAnd(val, bit8);
        auto scar_cmp = builder.CreateICmpEQ(scar_and, bit8);
        builder.CreateStore(scar_cmp, rSCar);
    }
    if (bits & Overflow) {
        // Overflow needs to be handled by instruction codegen, as more information is required.
        std::cerr << "Cannot generate overflow test from single value." << std::endl;
    }
}
