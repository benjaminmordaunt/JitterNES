#include <iostream>

#include "token.h"
#include "LabelAST.h"

int main() {
    populateInstructions();

    // Handle top-level constructs
    while (getNextToken() != tok_eof) {
        switch (CurToken) {
            case tok_identifier:
                getNextToken();

                // This is a label.
                if (CurToken == ':') {
                    if (LabelAST::ParseLabel() == nullptr) {
                        std::cout << "Could not parse label. Was the addressee invalid?" << std::endl;
                    } else {
                        std::cout << "Read a label." << std::endl;
                    }
                } else {
                    std::cout << "... not sure what this is..." << std::endl;
                }
                break;
            case tok_instruction:
                std::cout << "Found an instruction: " << IdentifierString << "\n";
                break;
            default:
                break;
        }
    }

    return 0;
}
