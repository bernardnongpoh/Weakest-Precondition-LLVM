//
// Created by heprit on 26/6/16.
//

#ifndef APPROXWP_PARSEINSTRUCTION_H
#define APPROXWP_PARSEINSTRUCTION_H

#include <IR_Record.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>
class ParseIR {
public:
    IR_Record parseInstruction(llvm::Instruction &instruction);

};


#endif //APPROXWP_PARSEINSTRUCTION_H
