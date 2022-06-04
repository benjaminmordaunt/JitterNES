//
// Created by Benjamin Mordaunt on 01/06/2022.
//

#include "LabelAST.h"
#include "lexer/token.h"
#include "cpu/CPUState.h"

/// A map of labels to their addressable.
/// Note that labels take ownership of their addressable in the AST tree.
/// So, when looking to e.g. assemble instructions, I also need to examine labels' children.
std::map<std::string, std::shared_ptr<AddressableAST>> LabelMap;

std::unique_ptr<LabelAST> LabelAST::ParseLabel(std::shared_ptr<CPUState> &cpu) {
    if (CurToken != ':')
        return nullptr;

    // Verify that this label is unique
    if (LabelMap.find(IdentifierString) != LabelMap.end())
        return nullptr;

    // A label refers to the resolved address of its immediate successor.
    // Therefore, attempt to parse an Addressable.

    auto addr_next = AddressableAST::ParseAddressable(cpu);
    if (addr_next == nullptr) {
        // This label refers to nonsense, drop it.
        return nullptr;
    }

    LabelMap[IdentifierString] = std::move(addr_next);
    return std::make_unique<LabelAST>(cpu, IdentifierString);
}