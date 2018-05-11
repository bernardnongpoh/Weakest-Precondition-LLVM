//
// Created by Jamil Ahmed on 1/7/16.
//

#include <boost/shared_ptr.hpp>
#include "llvm/Support/raw_ostream.h"
#include "z3++.h"
#include "z3_api.h"
#include "Expr.h"
using namespace z3;
class z3Obj{
    context c;
public:
    expr toZ3Expr(Expr::ExprPtr);
    std::map<std::string,std::string> solveZ3Expr(expr);
    void showExpr(expr);
};

