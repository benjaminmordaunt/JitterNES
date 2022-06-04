#include <iostream>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include "lexer/token.h"
#include "ast/LabelAST.h"
#include "ast/InstructionAST.h"
#include "cpu/CPUState.h"

int main() {
    populateInstructions();

    auto cpu = std::make_shared<CPUState>();
    std::unique_ptr<InstructionAST> inst;

    // Handle top-level constructs
    while (getNextToken() != tok_eof) {
        switch (CurToken) {
            case tok_identifier:
                getNextToken();

                // This is a label.
                if (CurToken == ':') {
                    if (LabelAST::ParseLabel(cpu) == nullptr) {
                        std::cout << "Could not parse label. Was the addressee invalid?" << std::endl;
                    } else {
                        std::cout << "Read a label." << std::endl;
                    }
                } else {
                    std::cout << "... not sure what this is..." << std::endl;
                }
                break;
            case tok_instruction:
                if ((inst = InstructionAST::ParseInstruction(cpu)) == nullptr) {
                    std::cout << "Could not parse instruction." << std::endl;
                } else {
                    inst->codegen();
                    std::cout << "Read a top-level instruction." << std::endl;
                }
                break;
            default:
                break;
        }


    }

    // Optimize module
    auto fpm = std::make_unique<llvm::legacy::FunctionPassManager>(cpu->module.get());

    fpm->add(llvm::createInstructionCombiningPass());
    fpm->add(llvm::createReassociatePass());
    fpm->add(llvm::createGVNPass());
    fpm->add(llvm::createCFGSimplificationPass());

    fpm->doInitialization();

    cpu->builder.CreateRetVoid();
    llvm::verifyFunction(*cpu->mainFn);
    fpm->run(*cpu->mainFn);

    cpu->module->print(llvm::errs(), nullptr);

    return 0;
}
