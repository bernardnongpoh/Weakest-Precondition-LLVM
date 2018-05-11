//
// Created by bernard on 24/6/16.
//

#ifndef APPROXWP_IR_RECORD_H
#define APPROXWP_IR_RECORD_H

#include <string>
#include <list>
#include <Expr.h>


class IR_Record {
    public:
        //Unique Id for each Record
        int ID;
        // IR variable name in SSA example %a %1 and %2
        std::string variableName;
        //The block in which each instruction belong to .
        std::string blockLabel;
        //Opcode in each instruction
        unsigned int  opcode;
        //The Instruction in RHS of = or expression from non-assignment instruction
        std::string expr;
        //true if we want to ignore for the next analysis else false
        bool  ignore;
        //Symbolic representation of the instruction
        std::string symbolicExpr;


};


#endif //APPROXWP_IR_RECORD_H
