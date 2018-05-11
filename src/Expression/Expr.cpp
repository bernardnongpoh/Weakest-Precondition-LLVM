//
// Created by Bernard Nongpoh on 27/6/16.
//


#include <Expr.h>

    Variable::Variable(std::string name){
        this->name=name;
        this->Type=VARIABLE;
    }

    std::string Variable::value() const{
        return name;
    }
    void Variable::setName(std::string name){
        this->name=name;
    }

    ExprType Variable::getType() const{
        return Type;
    }

Expr::ExprPtr Variable::findExprByName(std::string name) const {
   if(this->name.compare(name)){
       Variable::VariablePtr var(new Variable(name));
       return var;
   }
    return NULL;
}


ConstValue::ConstValue(std::string name){
        this->name=name;
        this->Type=CONSTANT;
    }

    std::string ConstValue::value() const{
        return name;
    }
    ExprType ConstValue::getType() const{
        return Type;
    }

Array::Array(std::string name) {
    this->Type=ARRAY;
    this->name;
}

void Array::insertValue(Expr::ExprPtr value,int index){
    llvm::outs()<<index <<"::"<<value->value()<<"\n";
    this->val[index]=value;
}
std::string Array::value() const{
    std::string arrValue;
    for(auto t : val){

        arrValue=arrValue + t.second->value();
    }
       return arrValue;

}
ExprType Array::getType() const{
    return Type;
}


Expr::ExprPtr Array::getValue(int index) const{
    std::map<int, Expr::ExprPtr>::const_iterator it;
        it=this->val.find(index);
    if(it!=val.end())
        return  it->second;
    else
        return NULL;
}


Expr::ExprPtr Array::findExprByName(std::string name) const {
    if(this->name.compare(name)){
        Array::ArrayPtr var(new Array(name));
        return var;
    }
    return NULL;
}



BinaryOp::BinaryOp(Operator op,Expr::ExprPtr l,Expr::ExprPtr r){
        this->Operator_symbol=op;
        this->left=l;
        this->right=r;
        this->Type=BINARYOPERATOR;
    }
    std::string  BinaryOp::value() const{

        return "("+left->value() + " " + this->getOperatorName()+ " " + right->value() +")";
    }

    std::string  BinaryOp::getOperatorName() const{
        switch(Operator_symbol){
            case Operator::PLUS:
                return " + ";
            case Operator::MINUS:
                return " - ";
            case Operator::DIV:
                return " / ";
            case Operator::AND:
                return " AND ";
            case Operator::MUL:
                return " * ";
            case Operator::OR:
                return " OR ";
            case Operator::GT:
                return " > ";
            case Operator::LT:
                return " < ";
            case Operator::GTE:
                return " >= ";
            case Operator::LTE:
                return " <= ";
            case Operator ::ASSIGNMT:
                return " = ";
            case Operator ::EQ:
                return " == ";
            default:
                return "BinaryOperator";
        }
    }


    void  BinaryOp::setLeft(Expr::ExprPtr exprPtr){
        this->left=exprPtr;
    }

    void  BinaryOp::setRight(Expr::ExprPtr exprPtr){
        this->right=exprPtr;
    }
    void  BinaryOp::setOperatorSymbol(Operator op){
        this->Operator_symbol=op;
    }

    bool  BinaryOp::replace(Variable::VariablePtr find,Expr::ExprPtr replaceBy){
        bool replaceLeft=false;
        bool replaceRight=false;
        replaceLeft=this->replace(find,replaceBy,this->left);
        if(replaceLeft){
                this->left=replaceBy;
             }
        replaceRight=this->replace(find,replaceBy,this->right);
    if(replaceRight){
        this->right=replaceBy;
    }
        if(replaceLeft)
            return replaceLeft;
       else
            return replaceRight;
       }

    ExprType  BinaryOp::getType() const{
        return Type;
    }
    bool  BinaryOp::replace(Expr::ExprPtr find,Expr::ExprPtr replaceBy,Expr::ExprPtr subTree) {
        bool replace=false;
       // llvm::outs()<<"\nD:"<<subTree->value()<<":Type:"<<subTree->getType() <<"\n";
        if(subTree->getType()==VARIABLE){


            Variable::VariablePtr variablePtr=boost::static_pointer_cast<Variable>(subTree);
            if(find->value().compare(variablePtr->value())==0){
                replace=true;
                return true;
            }
            return false;
        }
        else if(subTree->getType()==BINARYOPERATOR) {

            BinaryOp::BinaryOpPtr binaryOpPtr=boost::static_pointer_cast<BinaryOp>(subTree);
            if(this->replace(find,replaceBy,binaryOpPtr->left)){
                binaryOpPtr->setLeft(replaceBy);
            }
            if(this->replace(find,replaceBy,binaryOpPtr->right)){
                binaryOpPtr->setRight(replaceBy);
            }
        }

        else if(subTree->getType()==UNARYOPERATOR){
           // llvm::outs()<<"\nU:Binary\n";
            UnaryNOTOperator::Ptr unaryOperatorPtr=boost::static_pointer_cast<UnaryNOTOperator>(subTree);
            if(this->replace(find,replaceBy,unaryOperatorPtr->getNode())){
                unaryOperatorPtr->setNode(replaceBy);
            }
        }

        return replace;
    }

Expr::ExprPtr BinaryOp::findExprByName(std::string name) const {
    //find left and right recursively. and return expr.

    Expr::ExprPtr exprPtr=findExprByName(this->left);
     if(exprPtr->getType()==VARIABLE){
         return this->left;
     }

    findExprByName(this->right);

    return Expr::findExprByName(name);


}

Expr::ExprPtr BinaryOp::findExprByName(Expr::ExprPtr exprPtr) const {


}


AssignmentOperator::AssignmentOperator(Variable::VariablePtr LHS,Expr::ExprPtr RHS){
            this->LHS=LHS;
            this->RHS=RHS;
            this->Type=ASSIGNMENT;
        }

    Expr::ExprPtr AssignmentOperator::getRHS() const{
            return this->RHS;
        }

    Variable::VariablePtr AssignmentOperator::getLHS() const{
        return this->LHS;
        }

    std::string AssignmentOperator::value() const{
            return this->LHS->value() + " = " + this->RHS->value();
        }

    ExprType AssignmentOperator::getType() const{
        return Type;
    }

bool  AssignmentOperator::replace(Variable::VariablePtr find,Expr::ExprPtr replaceBy){
    bool replaceRHS=false;
    replaceRHS=this->replace(find,replaceBy,this->RHS);
    if(replaceRHS){
        this->RHS=replaceBy;
    }
   return replaceRHS;

}

bool  AssignmentOperator::replace(Expr::ExprPtr find,Expr::ExprPtr replaceBy,Expr::ExprPtr subTree) {
    bool replace=false;
       if(subTree->getType()==VARIABLE){
        Variable::VariablePtr variablePtr=boost::static_pointer_cast<Variable>(subTree);
        if(find->value().compare(variablePtr->value())==0){
            replace=true;
            return true;
        }
        return false;
    }
    else if(subTree->getType()==BINARYOPERATOR) {

        BinaryOp::BinaryOpPtr binaryOpPtr=boost::static_pointer_cast<BinaryOp>(subTree);
        if(this->replace(find,replaceBy,binaryOpPtr->left)){
            binaryOpPtr->setLeft(replaceBy);
        }
        if(this->replace(find,replaceBy,binaryOpPtr->right)){
            binaryOpPtr->setRight(replaceBy);
        }
    }

    else if(subTree->getType()==UNARYOPERATOR){
       // llvm::outs()<<"\nU:Binary\n";
        UnaryNOTOperator::Ptr unaryOperatorPtr=boost::static_pointer_cast<UnaryNOTOperator>(subTree);
        if(this->replace(find,replaceBy,unaryOperatorPtr->getNode())){
            unaryOperatorPtr->setNode(replaceBy);
        }
    }
    return replace;
}


UnaryNOTOperator::UnaryNOTOperator(Expr::ExprPtr node){
        this->node=node;
        this->Type=UNARYOPERATOR;
       // performOperation(this->node);
    }



    std::string UnaryNOTOperator:: value() const{

        return performOperation(this->node);
    }
    void UnaryNOTOperator::setNode(Expr::ExprPtr node){
        this->node=node;
        //performOperation(this->node);
    }
    std::string UnaryNOTOperator::performOperation(Expr::ExprPtr node) const{
        if(node->getType()==BINARYOPERATOR){
            BinaryOp::BinaryOpPtr binaryOpPtr=boost::static_pointer_cast<BinaryOp>(node);
            if(binaryOpPtr->Operator_symbol==Operator::GT){
                //binaryOpPtr->setOperatorSymbol(Operator::LTE);
                   return "("+binaryOpPtr->left->value() + " <= " + binaryOpPtr->right->value() + ")";
            }
            if(binaryOpPtr->Operator_symbol==Operator::EQ){
                //binaryOpPtr->setOperatorSymbol(Operator::LTE);
                return "!("+binaryOpPtr->left->value() + " == " + binaryOpPtr->right->value() + ")";
            }
        }
        else if(node->getType()==VARIABLE){
            return "!("+node->value() + ")";
        }

        return "";



    }
    bool UnaryNOTOperator::replace(Expr::ExprPtr find,Expr::ExprPtr replaceBy,Expr::ExprPtr subTree) {
        if(subTree->getType()==VARIABLE){
            Variable::VariablePtr variablePtr=boost::static_pointer_cast<Variable>(subTree);
            if(find->value().compare(variablePtr->value())==0){
               return true;
            }
            return false;
        }
        else if(subTree->getType()==BINARYOPERATOR) {

            BinaryOp::BinaryOpPtr binaryOpPtr=boost::static_pointer_cast<BinaryOp>(subTree);
            if(this->replace(find,replaceBy,binaryOpPtr->left)){
                binaryOpPtr->setLeft(replaceBy);
            }
            if(this->replace(find,replaceBy,binaryOpPtr->right)){
                binaryOpPtr->setRight(replaceBy);
            }
        }

        else if(subTree->getType()==UNARYOPERATOR){
            UnaryNOTOperator::Ptr unaryOperatorPtr=boost::static_pointer_cast<UnaryNOTOperator>(subTree);
            if(this->replace(find,replaceBy,unaryOperatorPtr->getNode())){
                unaryOperatorPtr->setNode(replaceBy);
            }
        }
        return false;
    }

Expr::ExprPtr UnaryNOTOperator::getNode() {
    return this->node;
}

ExprType UnaryNOTOperator::getType() const {
    return this->Type;
}


ArrayReference::ArrayReference(std::string name,int index) {
    this->Type=ARRAYREF;
    this->name=name;
    this->index=index;


}

std::string ArrayReference::value() const {
    return name + std::string("[") + std::to_string(index) + std::string("]");
}

ExprType ArrayReference::getType() const {
    return this->Type;
}





