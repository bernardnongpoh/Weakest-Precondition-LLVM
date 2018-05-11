//
// Created by bernard on 1/7/16.
//
#include <string>
#include <boost/shared_ptr.hpp>
#include <list>
#include <map>
#include "llvm/Support/raw_ostream.h"

enum ExprType{UNDEFINED,VARIABLE,CONSTANT,ARRAY,ARRAYREF,BINARYOPERATOR,ASSIGNMENT,UNARYOPERATOR};

    enum Operator{DEFAULT,MINUS,PLUS,DIV,MUL,AND,OR,GT,LT,GTE,LTE,NOT,ASSIGNMT,EQ};


class Expr{
    ExprType  Type=UNDEFINED;
public:

    typedef boost::shared_ptr<Expr> ExprPtr;
    virtual std::string value() const{} // Pre Implement here otherwise We may face error
    virtual void replace(Expr::ExprPtr find,Expr::ExprPtr replaceBy){}
    virtual Expr::ExprPtr findExprByName(std::string name) const{}
    virtual ExprType getType()const {}

};

class Variable:public Expr{
    std::string name;
    ExprType Type;
public:
    Variable(std::string name);
    std::string value() const;
    void setName(std::string name);
    ExprType getType() const;
    Expr::ExprPtr findExprByName(std::string name) const;
    typedef boost::shared_ptr<Variable> VariablePtr;
};

class ConstValue:public Expr{
    std::string name;
    ExprType Type;
public:
    ConstValue(std::string name);
    std::string value() const;
    ExprType getType() const;
    typedef boost::shared_ptr<ConstValue> ConstantPtr;
};

class Array:public Expr{
    ExprType Type;
    std::string name;
    std::map<int,Expr::ExprPtr> val;
public:
    Array(std::string name);
    std::string value() const;
    ExprType getType() const;
    void insertValue(Expr::ExprPtr val,int index);
    //Find an array by name, if yes return the array ptr else null
    Expr::ExprPtr findExprByName(std::string name) const;
    Expr::ExprPtr getValue(int index) const;
    typedef boost::shared_ptr<Array> ArrayPtr;
};

class ArrayReference:public Expr{
    ExprType  Type;
    std::string name;
    int index;

public:
    ArrayReference(std::string name,int index);
    std::string value() const;
    ExprType getType() const;

    typedef boost::shared_ptr<ArrayReference> ArrayRefPtr;

};


class BinaryOp:public Expr{
    ExprType  Type;
public:
    Operator Operator_symbol;
    Expr::ExprPtr left;
    Expr::ExprPtr right;
    BinaryOp(Operator op,Expr::ExprPtr l,Expr::ExprPtr r);
    std::string value() const;
    std::string getOperatorName() const;
    void setLeft(Expr::ExprPtr exprPtr);
    void setRight(Expr::ExprPtr exprPtr);
    void setOperatorSymbol(Operator op);
    bool replace(Variable::VariablePtr find,Expr::ExprPtr replaceBy);
    ExprType getType() const;
    Expr::ExprPtr findExprByName(std::string name) const;
    typedef boost::shared_ptr<BinaryOp> BinaryOpPtr;
private:
    bool replace(Expr::ExprPtr find,Expr::ExprPtr replaceBy,Expr::ExprPtr subTree);
    Expr::ExprPtr findExprByName(Expr::ExprPtr exprPtr) const;
};

class AssignmentOperator:public Expr{
    ExprType Type;
public:
    Variable::VariablePtr LHS;
    Expr::ExprPtr RHS;
    AssignmentOperator(Variable::VariablePtr LHS,Expr::ExprPtr RHS);
    Expr::ExprPtr getRHS() const;
    Variable::VariablePtr getLHS() const;
    std::string value() const;
    ExprType getType() const;
    bool replace(Variable::VariablePtr find,Expr::ExprPtr replaceBy);
    typedef boost::shared_ptr<AssignmentOperator> AssignmentOperatorPtr;
private:
    bool replace(Expr::ExprPtr find,Expr::ExprPtr replaceBy,Expr::ExprPtr subTree);
};

class UOperator:public Expr{

};

class UnaryNOTOperator:public UOperator{
    ExprType Type;
    Expr::ExprPtr node;

public:

    UnaryNOTOperator(Expr::ExprPtr node);
    std::string value() const;
    void setNode(Expr::ExprPtr node);
    Expr::ExprPtr getNode();
    ExprType getType()const;

    typedef boost::shared_ptr<UnaryNOTOperator> Ptr;
private:
    std::string performOperation(Expr::ExprPtr node) const;
    bool replace(Expr::ExprPtr find,Expr::ExprPtr replaceBy,Expr::ExprPtr subTree);
};

