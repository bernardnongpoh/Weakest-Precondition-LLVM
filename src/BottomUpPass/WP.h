//
// Created by bernard on 29/6/16.
//

#ifndef APPROXWP_WP_H
#define APPROXWP_WP_H


#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>
#include <string>
#include <map>
#include <stack>
#include <Z3.h>

using namespace llvm;
using namespace z3;

class WP {

private:
    //Post condition always in the form of binary ie max>=30 or 0<max<20
    BinaryOp::BinaryOpPtr post_cond;
    BinaryOp::BinaryOpPtr wp_expr;
    std::map<std::string,Array::ArrayPtr> arrays;
    std::stack<Instruction*> wp_instruction;
    std::map<std::string,std::string> solutionMap;


public:
    bool performWPFlag=false;
    void parseInstruction(llvm::Instruction &instruction);
    bool isArrayExist(std::string name) const;
    bool insertArrayByIndex(std::string name,int index,Expr::ExprPtr exprPtr);
    Expr::ExprPtr getWPExpression() const;
    Expr::ExprPtr getPostCondition() const;
    void createPostCondition(const CallInst *callInst, const Function *function);
    void transformIR();
    std::string getSolutionByOperandName(std::string  operandName);
};


#endif //APPROXWP_WP_H
