//
// Created by Jamil Ahmed on 1/7/16.
//
#include "Z3.h"


using namespace z3;
using namespace std;
expr z3Obj::toZ3Expr(Expr::ExprPtr exprPtr) {
    switch (exprPtr->getType()){
        case VARIABLE: {
            expr e = c.int_const(exprPtr->value().c_str());
            return e;             //handle float and int here
        }
        case BINARYOPERATOR: {
            BinaryOp::BinaryOpPtr binaryOpPtr = boost::static_pointer_cast<BinaryOp>(exprPtr);
            expr leftExpr = toZ3Expr(binaryOpPtr->left);
            expr rightExpr = toZ3Expr(binaryOpPtr->right);
            switch (binaryOpPtr->Operator_symbol) {
                case MINUS :
                    return (leftExpr - rightExpr);
                case PLUS:
                    return (leftExpr + rightExpr);
                case DIV:
                    return (leftExpr / rightExpr);
                case MUL:
                    return (leftExpr * rightExpr);
                case AND:
                    return (leftExpr && rightExpr);
                case OR :
                    return (leftExpr || rightExpr);
                case GT:
                    return (leftExpr > rightExpr);
                case LT:
                    return (leftExpr < rightExpr);
                case GTE:
                    return (leftExpr >= rightExpr);
                case LTE:
                    return (leftExpr <= rightExpr);
                case ASSIGNMT:
                    return (leftExpr = rightExpr);
            }
        }
        case UNARYOPERATOR:
        {
            UnaryNOTOperator::Ptr ptr=boost::static_pointer_cast<UnaryNOTOperator>(exprPtr);
            expr restExpr=toZ3Expr(ptr->getNode());
            return (! restExpr);
        }
        case CONSTANT:
        {
            ConstValue::ConstantPtr constantPtr=boost::static_pointer_cast<ConstValue>(exprPtr);
            return c.int_val(constantPtr->value().c_str());
        }
        case ASSIGNMENT:
        {

        }
    }


}

std::map<std::string,std::string> z3Obj::solveZ3Expr(expr e) {
    solver solver1(c);
    solver1.add(e);
    std::map<string,string> solutionMap;
    std::cout << solver1.check() << "\n";
    std::cout<<solver1.to_smt2();
    model m = solver1.get_model();
    std::cout << "\n\n\n";
    for (unsigned i = 0; i < m.size(); i++) {
        func_decl v = m[i];
        assert(v.arity() == 0);

        solutionMap.insert(std::pair<string,string>(v.name().str(), to_string(m.get_const_interp(v))));
        std::cout << v.name() << " = " << to_string(m.get_const_interp(v)) << "\n";
    }



    return solutionMap;
}

void z3Obj::showExpr(expr e) {
    solver s(c);
    s.add(e);
    std::cout<<s.to_smt2();
}






