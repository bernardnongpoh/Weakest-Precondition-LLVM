//
// Created by Bernard on 26/6/16.
//

#include "ParseIR.h"


IR_Record ParseIR::parseInstruction(llvm::Instruction &instruction) {

    IR_Record record;
    unsigned int OPCODE =instruction.getOpcode();
    instruction.dump();
    record.variableName=instruction.getName();
    record.opcode=OPCODE;

   switch (OPCODE){
       case llvm::Instruction::Alloca:

           break;
       case llvm::Instruction::Store:
           instruction.dump();
       record.variableName=instruction.getOperand(0)->getName();

       llvm::outs()<<"#Operands"<<instruction.getNumOperands();
       llvm::outs()<<record.variableName<<"\n";
           break;

   }
    return record;
}

