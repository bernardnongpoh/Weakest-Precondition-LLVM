//
// Created by bernard on 29/6/16.
//

#include "WP.h"

void WP::parseInstruction(llvm::Instruction &instruction) {

    if (isa<CallInst>(instruction)) {

            CallInst *callInst = dyn_cast<CallInst>(&instruction);
            Function *function =callInst->getCalledFunction();

            if(function==NULL){
                return;
            }
            std::string functionName = function->getName();
            if (functionName.compare("_wp_end")==0) {
                createPostCondition(callInst, function);
            }
             if (functionName.compare("_wp_begin")==0) {

             llvm::outs()<<post_cond->value();

                 //Just to display here
                 /*while(!wp_instruction.empty()){
                     Instruction *wp_inst =wp_instruction.top();
                     wp_instruction.pop();
                    wp_inst->dump();
                 }
                */

                ///transformIR();

                 /* Testing */
                 Variable::VariablePtr a(new Variable("a"));
                 ConstValue::ConstantPtr constantPtr(new ConstValue("8"));
                 BinaryOp::BinaryOpPtr lte(new BinaryOp(LTE,a,constantPtr));
                 Z3_config cfg=Z3_mk_config();
                 z3Obj obj;
                 expr e = obj.toZ3Expr(post_cond);
                 // e.simplify();
                 obj.showExpr(e);
                 solutionMap=obj.solveZ3Expr(e);
                 /*End Testing */
                //Setting flags here
                 transformIR();
                 post_cond=NULL;
                 performWPFlag = false;
             }


            if (functionName.compare("_Z7_wp_endPKc")==0) {

                //get a Post condition from the return type of WP i.e we need to extract string from here
                createPostCondition(callInst, function);

            }
            if (functionName.compare("_Z9_wp_beginv")==0) {
                post_cond=NULL;
                performWPFlag = false;
            }


            return;
        }


    if(!performWPFlag){
        return;
    }

        instruction.dump();
   wp_instruction.push(&instruction);

    Variable::VariablePtr label(new Variable(""));
    if(instruction.hasName()){
        label->setName(instruction.getName());
    }

       if(isa<ReturnInst>(instruction)){

           ReturnInst *returnInst=cast<ReturnInst>(&instruction);
            //TODO: check for return type is it a  void,int ,float and etc.
           if(returnInst->getType()->getTypeID()==Type::VoidTyID){
               //Look for what precondition to satisfied which is the parameters of the function.
               //hardcoded as for now: consider an arr as the post cond 0<=arr<=10
             //   instruction.dump();

            //check for type
              if(returnInst->getNumOperands() >0){
                  Variable::VariablePtr arr(new Variable(returnInst->getOperand(0)->getName()));
                  ConstValue::ConstantPtr const_0(new ConstValue("3.140"));
                  ConstValue::ConstantPtr const_10(new ConstValue("3.145"));
                  BinaryOp::BinaryOpPtr gte(new BinaryOp(Operator::GTE,arr,const_0));
                  BinaryOp::BinaryOpPtr lte(new BinaryOp(Operator::LTE,arr,const_10));
                  BinaryOp::BinaryOpPtr AND(new BinaryOp(Operator::AND,gte,lte));
                  //llvm::outs()<<AND->value(); OK
                  post_cond=AND;
                 // llvm::outs()<<post_cond->value();
                  return;

              }



               Variable::VariablePtr arr(new Variable("arr"));
               ConstValue::ConstantPtr const_0(new ConstValue("0"));
               ConstValue::ConstantPtr const_10(new ConstValue("10"));
               BinaryOp::BinaryOpPtr gte(new BinaryOp(Operator::GTE,arr,const_0));
               BinaryOp::BinaryOpPtr lte(new BinaryOp(Operator::LTE,arr,const_10));
               BinaryOp::BinaryOpPtr AND(new BinaryOp(Operator::AND,gte,lte));
               //llvm::outs()<<AND->value(); OK
               post_cond=AND;
             //  llvm::outs()<<post_cond->value();
               return;
           }

           if(returnInst->getType()->getTypeID()==Type::DoubleTyID){
               //Look for what precondition to satisfied which is the parameters of the function.
               //hardcoded as for now: consider an arr as the post cond 0<=arr<=10
               //llvm::outs()<<"Double";
               return;
           }
           instruction.dump();
           Variable::VariablePtr return_variable(new Variable(returnInst->getOperand(0)->getName()));
            ConstValue::ConstantPtr post_value(new ConstValue("3.14"));
            BinaryOp::BinaryOpPtr gte(new BinaryOp(Operator::GTE, return_variable, post_value));
            post_cond=gte;
       //    llvm::outs()<<post_cond->value()<<"\n";




    }

    if(isa<PHINode>(instruction)){

        PHINode *phiNode=cast<PHINode>(&instruction);
        unsigned  numberOfIncomingBlock=phiNode->getNumIncomingValues();
        Variable::VariablePtr lhs(new Variable(phiNode->getName()));
        BinaryOp::BinaryOpPtr rhs(new BinaryOp(Operator::OR,NULL,NULL));
        AssignmentOperator::AssignmentOperatorPtr assignment(new AssignmentOperator(lhs,rhs));

        for(int i=0;i<numberOfIncomingBlock;i++){
            Variable::VariablePtr numUse(new Variable(phiNode->getOperand(i)->getName()));
            Variable::VariablePtr block(new Variable(phiNode->getIncomingBlock(i)->getName()));
            BinaryOp::BinaryOpPtr AND(new BinaryOp(Operator::AND,numUse,block));
            if(rhs->left==NULL)
                rhs->setLeft(AND);
            else
                rhs->setRight(AND);
        }

        //Look for the variable in the lhs of post condition if found replace by rhs
       post_cond->replace(lhs,rhs);
        //post_cond->value();

    }

    if(isa<BinaryOperator>(instruction)){
        llvm::outs()<<"Binary Operator";
      //instruction.dump();
        BinaryOperator *binaryOperator =cast<BinaryOperator>(&instruction);


        switch(binaryOperator->getOpcode()){
            case llvm::Instruction::Add:
            {

                Value *operand1=binaryOperator->getOperand(0);
                Value *operand2=binaryOperator->getOperand(1);
                Variable::VariablePtr variablePtr(new Variable(operand1->getName()));
                BinaryOp::BinaryOpPtr PLUS(new BinaryOp(Operator::PLUS,NULL,NULL));
                PLUS->setLeft(variablePtr);
                if(Constant *c=dyn_cast<Constant>(operand1)){
                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        PLUS->setLeft(constantPtr);
                        // check for pre condition here

                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand1->getName()));
                    PLUS->setLeft(variablePtr1);
                }

                if(Constant *c=dyn_cast<Constant>(operand2)){
                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        PLUS->setRight(constantPtr);
                        // check for pre condition here
                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand2->getName()));
                    PLUS->setRight(variablePtr1);
                }

                if(wp_expr!=NULL)
                {
                    wp_expr->replace(label,PLUS);
                }
                //  llvm::outs()<<label->value()<<"="<<"MUL::"<<MUL->value()<<"\n";
                // llvm::outs()<<"BPOST"<<post_cond->value()<<"\n";
                if(post_cond->replace(label,wp_expr)){
                    wp_expr=NULL;
                   // llvm::outs()<<"NULL POINTER TO WP_EXPR";
                }
                // llvm::outs()<<"POST"<<post_cond->value()<<"\n";
                Value *newValue = ConstantInt::get(Type::getInt32Ty(instruction.getContext()),1000);


                break;
            }

                // Float Addition
            case llvm::Instruction::FAdd:
            {
               // llvm::outs()<<"inside fdiv";
                //llvm::outs()<<binaryOperator->getOperand(0)->getName()<<":"<<binaryOperator->getOperand(1)->getName() <<"\n";
                Value *operand1=binaryOperator->getOperand(0);
                Value *operand2=binaryOperator->getOperand(1);
                Variable::VariablePtr variablePtr(new Variable(operand1->getName()));
                BinaryOp::BinaryOpPtr MUL(new BinaryOp(Operator::PLUS,NULL,NULL));
                MUL->setLeft(variablePtr);
                if(Constant *c=dyn_cast<Constant>(operand1)){
                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setLeft(constantPtr);
                        // check for pre condition here

                    }
                    if(c->getType()->isDoubleTy()){
                       // llvm::outs()<<"inside double";
                        c->dump();
                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand1->getName()));
                    MUL->setLeft(variablePtr1);
                }

                if(Constant *c=dyn_cast<Constant>(operand2)){
                    llvm::outs()<<"constant 2";

                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setRight(constantPtr);
                        // check for pre condition here
                    }

                    if(c->getType()->isDoubleTy()){

                        if (ConstantFP *Op = dyn_cast<ConstantFP>(c)) {
                            double val=Op->getValueAPF().convertToDouble();
                            ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                            MUL->setRight(constantPtr);
                        }


                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand2->getName()));
                    MUL->setRight(variablePtr1);
                }

                if(wp_expr!=NULL)
                {
                    wp_expr->replace(label,MUL);

                    if(post_cond->replace(label,wp_expr)){
                        wp_expr=NULL;
                        llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                }
                else{
                    if(post_cond->replace(label,MUL)){
                        wp_expr=NULL;
                        llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                    else{
                        wp_expr=MUL;
                    }
                }
                //  llvm::outs()<<label->value()<<"="<<"MUL::"<<MUL->value()<<"\n";
                // llvm::outs()<<"BPOST"<<post_cond->value()<<"\n";

                // llvm::outs()<<"POST"<<post_cond->value()<<"\n";

                llvm::outs()<<post_cond->value();
                break;
            }

            case llvm::Instruction::Mul:

            {
                Value *operand1=binaryOperator->getOperand(0);
                Value *operand2=binaryOperator->getOperand(1);
                Variable::VariablePtr variablePtr(new Variable(operand1->getName()));
                BinaryOp::BinaryOpPtr MUL(new BinaryOp(Operator::MUL,NULL,NULL));
                MUL->setLeft(variablePtr);
                if(Constant *c=dyn_cast<Constant>(operand1)){
                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setLeft(constantPtr);
                        // check for pre condition here

                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand1->getName()));
                    MUL->setLeft(variablePtr1);
                }

                if(Constant *c=dyn_cast<Constant>(operand2)){
                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setRight(constantPtr);
                        // check for pre condition here
                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand2->getName()));
                    MUL->setRight(variablePtr1);
                }

                if(performWPFlag){
                    llvm::outs()<<"Here\n";
                    Variable::VariablePtr wp_default(new Variable("wp_default"));
                    post_cond->replace(wp_default,MUL);
                    llvm::outs()<<"Here"<<post_cond->value();

                }

                if(wp_expr!=NULL)
                {
                    wp_expr->replace(label,MUL);

                    if(post_cond->replace(label,wp_expr)){
                        wp_expr=NULL;
                        llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                }

              //  llvm::outs()<<label->value()<<"="<<"MUL::"<<MUL->value()<<"\n";
               // llvm::outs()<<"BPOST"<<post_cond->value()<<"\n";
                if(post_cond->replace(label,wp_expr)) {
                    wp_expr = NULL;
                    //   llvm::outs()<<"NULL POINTER TO WP_EXPR";
                }
                else{
                    if(post_cond->replace(label,MUL)){
                        wp_expr=NULL;
                        llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                    else{
                        wp_expr=MUL;
                    }

                }




              //llvm::outs()<<"POST"<<post_cond->value()<<"\n";
                break;
            }

            case llvm::Instruction::FMul:
            {
                llvm::outs()<<"inside fmul";
                //llvm::outs()<<binaryOperator->getOperand(0)->getName()<<":"<<binaryOperator->getOperand(1)->getName() <<"\n";
                Value *operand1=binaryOperator->getOperand(0);
                Value *operand2=binaryOperator->getOperand(1);
                Variable::VariablePtr variablePtr(new Variable(operand1->getName()));
                BinaryOp::BinaryOpPtr MUL(new BinaryOp(Operator::MUL,NULL,NULL));
                MUL->setLeft(variablePtr);
                if(Constant *c=dyn_cast<Constant>(operand1)){
                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setLeft(constantPtr);
                        // check for pre condition here

                    }
                    if(c->getType()->isDoubleTy()){
                       // llvm::outs()<<"inside double";
                        c->dump();
                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand1->getName()));
                    MUL->setLeft(variablePtr1);
                }

                if(Constant *c=dyn_cast<Constant>(operand2)){
                    llvm::outs()<<"constant 2";

                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setRight(constantPtr);
                        // check for pre condition here
                    }

                    if(c->getType()->isDoubleTy()){

                        if (ConstantFP *Op = dyn_cast<ConstantFP>(c)) {
                            double val=Op->getValueAPF().convertToDouble();
                            ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                            MUL->setRight(constantPtr);
                        }


                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand2->getName()));
                    MUL->setRight(variablePtr1);
                }

                if(wp_expr!=NULL)
                {
                    wp_expr->replace(label,MUL);

                    if(post_cond->replace(label,wp_expr)){
                        wp_expr=NULL;
                     //   llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                }
                else{
                    if(post_cond->replace(label,MUL)){
                        wp_expr=NULL;
                     //   llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                    else{
                        wp_expr=MUL;
                    }
                }
                //  llvm::outs()<<label->value()<<"="<<"MUL::"<<MUL->value()<<"\n";
                // llvm::outs()<<"BPOST"<<post_cond->value()<<"\n";

                // llvm::outs()<<"POST"<<post_cond->value()<<"\n";




                llvm::outs()<<post_cond->value();
                break;
            }

            // Float Division
            case llvm::Instruction::FDiv:
            {
                llvm::outs()<<"inside fdiv";
                //llvm::outs()<<binaryOperator->getOperand(0)->getName()<<":"<<binaryOperator->getOperand(1)->getName() <<"\n";
                Value *operand1=binaryOperator->getOperand(0);
                Value *operand2=binaryOperator->getOperand(1);
                Variable::VariablePtr variablePtr(new Variable(operand1->getName()));
                BinaryOp::BinaryOpPtr MUL(new BinaryOp(Operator::DIV,NULL,NULL));
                MUL->setLeft(variablePtr);
                if(Constant *c=dyn_cast<Constant>(operand1)){
                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setLeft(constantPtr);
                        // check for pre condition here

                    }
                    if(c->getType()->isDoubleTy()){
                      //  llvm::outs()<<"inside double";
                      //  c->dump();
                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand1->getName()));
                    MUL->setLeft(variablePtr1);
                }

                if(Constant *c=dyn_cast<Constant>(operand2)){
                    //llvm::outs()<<"constant 2";

                    if(c->getType()->isIntegerTy()){
                        int val=c->getUniqueInteger().getSExtValue();
                        ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                        MUL->setRight(constantPtr);
                        // check for pre condition here
                    }

                    if(c->getType()->isDoubleTy()){

                        if (ConstantFP *Op = dyn_cast<ConstantFP>(c)) {
                            double val=Op->getValueAPF().convertToDouble();
                            ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                            MUL->setRight(constantPtr);
                        }


                    }
                }
                else{
                    Variable::VariablePtr variablePtr1(new Variable(operand2->getName()));
                    MUL->setRight(variablePtr1);
                }

                if(wp_expr!=NULL)
                {
                    wp_expr->replace(label,MUL);

                    if(post_cond->replace(label,wp_expr)){
                        wp_expr=NULL;
                       // llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                }
                else{
                    if(post_cond->replace(label,MUL)){
                        wp_expr=NULL;
                      //  llvm::outs()<<"NULL POINTER TO WP_EXPR";
                    }
                    else{
                        wp_expr=MUL;
                    }
                }
                //  llvm::outs()<<label->value()<<"="<<"MUL::"<<MUL->value()<<"\n";
                // llvm::outs()<<"BPOST"<<post_cond->value()<<"\n";

                // llvm::outs()<<"POST"<<post_cond->value()<<"\n";

                llvm::outs()<<post_cond->value();
                break;
            }
        }
    }

    if(ICmpInst *IcmpInst=dyn_cast<ICmpInst>(&instruction)){
        //instruction.dump();

      // Value *newValue = ConstantInt::get(Type::getInt32Ty(instruction.getContext()),1000);
      // IcmpInst->setOperand(0,newValue);
        /*
         * Check Operand type and perform the operation base on its type
         *
         */
        Variable::VariablePtr operand2(new Variable(IcmpInst->getOperand(1)->getName()));
        Value *op1 = IcmpInst->getOperand(1);
        if(Constant *c=dyn_cast<Constant>(op1)){
                if(c->getType()->isIntegerTy()) {
                   int value = c->getUniqueInteger().getSExtValue();
                    operand2->setName(std::to_string(value));
                }
        }


        Variable::VariablePtr labelCmp(new Variable(IcmpInst->getName()));
        Variable::VariablePtr operand1(new Variable(IcmpInst->getOperand(0)->getName()));
        BinaryOp::BinaryOpPtr binaryOpPtr(new BinaryOp(Operator::DEFAULT,operand1,operand2));
       // llvm::outs()<<IcmpInst->getSignedPredicate();
        switch(IcmpInst->getSignedPredicate()){
            case llvm::CmpInst::ICMP_SGT:
                binaryOpPtr->setOperatorSymbol(Operator::GT);
                break;
            case llvm::CmpInst::ICMP_EQ:
                binaryOpPtr->setOperatorSymbol(Operator::EQ);
                break;
        }
        post_cond->replace(labelCmp,binaryOpPtr);
        //llvm::outs()<<"Final String\n"<<post_cond->value();
    }

if(isa<BranchInst>(&instruction)){
    instruction.dump();
    // Value *newValue = ConstantInt::get(Type::getInt32Ty(instruction.getContext()),10000);
    // loadInst->setOperand(0,newValue);

    BranchInst *branchInst=dyn_cast<BranchInst>(&instruction);
    llvm::outs()<<branchInst->getOperand(0)->getName();
    if(branchInst->getNumSuccessors()==1)
        return;
    Variable::VariablePtr controlVar(new Variable(branchInst->getOperand(0)->getName())); //will return a boolean variable cmp
    Variable::VariablePtr successor1(new Variable(branchInst->getSuccessor(0)->getName()));
    BinaryOp::BinaryOpPtr TPart(new BinaryOp(AND,controlVar,successor1));
    Variable::VariablePtr successor2(new Variable(branchInst->getSuccessor(1)->getName()));
    //Look for two successor and replace by control variable
    UnaryNOTOperator::Ptr NOTControlVar(new UnaryNOTOperator(controlVar));
    BinaryOp::BinaryOpPtr FPart(new BinaryOp(AND,NOTControlVar,successor2));


    BinaryOp::BinaryOpPtr ORPart(new BinaryOp(OR,TPart,FPart));

    //llvm::outs()<<post_cond->value();
    if(performWPFlag){
        Variable::VariablePtr wp_default(new Variable("wp_default"));
       post_cond->replace(wp_default,ORPart);
    }

   // llvm::outs()<<post_cond->value();

}

    /*STORE Instruction */

    if(isa<StoreInst>(&instruction))
    {
        StoreInst *storeInst=dyn_cast<StoreInst>(&instruction);
        Variable::VariablePtr RHS(new Variable(storeInst->getOperand(0)->getName()));
        Variable::VariablePtr LHS(new Variable(storeInst->getOperand(1)->getName()));
        BinaryOp::BinaryOpPtr equal(new BinaryOp(Operator::ASSIGNMT,LHS,RHS));
       // llvm::outs()<<"STORE::"<<equal->value()<<"\n"; //OK
        // Find a variable(LHS) in the precondition expression tree if there exist a node = to LHS replace it with RHS
        if(!post_cond->replace(LHS,RHS) && wp_expr== NULL){
            wp_expr=equal;
        }





    }

    /*LOAD Instruction */

    if(isa<LoadInst>(&instruction))
    {
       // instruction.dump();
        //Replacement with a constant


        LoadInst *loadInst=dyn_cast<LoadInst>(&instruction);
      //  llvm::Type::TypeID intType =llvm::Type::IntegerTyID;
      //  llvm::Type* typePtr= llvm::Type::getPrimitiveType(loadInst->getContext(),intType);
       // Value *newValue = ConstantInt::get(Type::getInt32Ty(instruction.getContext()),10000);
       // loadInst->setOperand(0,newValue);
       // loadInst->dump();

        llvm::outs()<<loadInst->getOperand(0)->getName();
        Variable::VariablePtr RHS(new Variable(loadInst->getOperand(0)->getName()));
        if(wp_expr!=NULL){
            wp_expr->replace(label,RHS);
        }
        else{
            BinaryOp::BinaryOpPtr equal(new BinaryOp(Operator::ASSIGNMT,label,RHS));
            wp_expr=equal;
        }

     if(post_cond->replace(label,wp_expr->right)){
            wp_expr=NULL;

        }
        else{
         Variable::VariablePtr wp_default(new Variable("wp_default"));
         if(post_cond->replace(wp_default,wp_expr->right)){
             wp_expr=NULL;

         }
     }

      //  llvm::outs()<<"Load"<<wp_expr->value()<<"\n";
      //llvm::outs()<<"LOad"<<post_cond->value()<<"\n";


    }

    if( GetElementPtrInst *getElementPtrInst=dyn_cast<GetElementPtrInst>(&instruction))
    {
      //this will give us the array name.
        //check if it is a pointer/can be an array also or structure
        Type *type=getElementPtrInst->getSourceElementType();
        std::string name = getElementPtrInst->getOperand(0)->getName();
        //if it is a structure , then two operands structname(0) and index(1)
        if(type->getTypeID()==Type::StructTyID)
        {
            llvm::outs()<<"Name:"<<getElementPtrInst->getOperand(0)->getName();
            Value *value=getElementPtrInst->getOperand(1);
            if(Constant *c=dyn_cast<Constant>(value)){
                if(c->getType()->getTypeID()==Type::IntegerTyID){
                    int index=c->getUniqueInteger().getSExtValue();
                    llvm::outs()<<"Index:"<<index;
                    name+="->"+std::to_string(index);
                }
            }

            Variable::VariablePtr structVar(new Variable(name));


            if(!post_cond->replace(label,structVar)){
                if(wp_expr!=NULL){
                    wp_expr->replace(label,structVar);
                }

            }


        }



        llvm::outs()<<"Struct Name:="<<name<<"\n";





       Value *arrayName = getElementPtrInst->getOperand(0);
        llvm::outs()<<arrayName->getName();
        //we can create arrayref from here let see if it work
            if(arrayName!=NULL){

            }
      //  Array::ArrayPtr arrayPtr(new Array(arrayName->getName()));

        Value *index = getElementPtrInst->getOperand(1);
        //Operand 2 always be an integer
        if(Constant *c=dyn_cast<Constant>(index)){
            if(c->getType()->isIntegerTy()){
                int index=c->getUniqueInteger().getSExtValue();

                //llvm::outs()<<"index:"<<index<<"\n";
                //Insert into a main array reference pointer we will se later on if we need this one
               // arrayPtr->insertValue(constantPtr,val);
                // check for pre condition here

                //llvm::outs()<<"Val"<<arrayRefPtr->value();
                //Look into the expression tree and do replemen
                //wp_expr->replace(label,arrayRefPtr);
                //llvm::outs()<<"POST COND " << wp_expr->value() <<"\n";
                if(wp_expr!=NULL){
                  //if we want to check for all element in an array use this one i.e instead of making array reference we may assume
                    // that array as a variable
                    std::string name = arrayName->getName().str();
                   // llvm::outs()<<"name"<<name<<"\n";
                    //insert into main array
                    ArrayReference::ArrayRefPtr arrayRefPtr(new ArrayReference(name,index));
                    Variable::VariablePtr arrName(new Variable(name));
                    wp_expr->replace(label,arrayRefPtr);

                    //Create a new reference to an array or use exisiting one

                    //Find the array by name and give a  pointer as arrayRef
                    auto arrayRef=arrays.find(name);
                    if(arrayRef!=arrays.end()) {
                        if (post_cond->replace(arrName, arrayRef->second)) {
                            wp_expr = NULL;
                        }
                    }
                    insertArrayByIndex(name,index,wp_expr->right);
                    wp_expr=NULL;


                    // If is is indidual element then we use array ref
                //    ArrayReference::ArrayRefPtr arrayRefPtr(new ArrayReference(arrayName->getName(),val));
                //    ArrayReference::ArrayRefPtr arrayRefPtr(new ArrayReference(arrayName->getName(),val));


                 //llvm::outs()<<post_cond->value()<<"\n";

                }


            }
        }



           }

    if(isa<CallInst>(instruction))
    {

        CallInst *callInst=dyn_cast<CallInst>(&instruction);
        //So here is our trick : we can do this
        //llvm::outs()<<"Call This :"<<callInst->getCalledFunction()->getName() <<"\n";
        //Now we check here if it is a start and stop of the instruction .. that make sense

    }

    //Type casting instruction
    if(isa<SIToFPInst>(instruction)){

        SIToFPInst *siToFPInst=dyn_cast<SIToFPInst>(&instruction);
       // llvm::outs()<<"#Operand:" <<siToFPInst->getOperand(0)->getName();
        Variable::VariablePtr variablePtr(new Variable(siToFPInst->getOperand(0)->getName()));
        if(!post_cond->replace(label,variablePtr)){
                if(wp_expr!=NULL){
                    wp_expr->replace(label,variablePtr);
                }

        }
        llvm::outs()<<post_cond->value();

    }

    if(isa<ZExtInst>(instruction)){

        ZExtInst *zExtInst=dyn_cast<ZExtInst>(&instruction);
        llvm::outs()<<"#Operand:" <<zExtInst->getOperand(0)->getName();
        Variable::VariablePtr variablePtr(new Variable(zExtInst->getOperand(0)->getName()));
        if(!post_cond->replace(label,variablePtr)){
            if(wp_expr!=NULL){
                wp_expr->replace(label,variablePtr);
            }

        }
        llvm::outs()<<post_cond->value();

    }

    if(isa<FCmpInst>(instruction)){

        FCmpInst *fCmpInst=dyn_cast<FCmpInst>(&instruction);



      // get Operand and check whether a variable or constant in here
        Value *operand1=fCmpInst->getOperand(0);
        Value *operand2=fCmpInst->getOperand(1);
        BinaryOp::BinaryOpPtr cmp(new BinaryOp(Operator::DEFAULT,NULL,NULL));
        //Handle for Operand 1
         if(Constant *c=dyn_cast<Constant>(operand1)){
             if(c->getType()->isIntegerTy()){
                 int val=c->getUniqueInteger().getSExtValue();
                 ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                 cmp->setLeft(constantPtr);
                 // check for pre condition here
             }

             if(c->getType()->isDoubleTy()){

                 if (ConstantFP *Op = dyn_cast<ConstantFP>(c)) {
                     double val=Op->getValueAPF().convertToDouble();
                     ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                     cmp->setLeft(constantPtr);
                 }


             }

         }
        else{
            //this is for variable
             Variable::VariablePtr op1(new Variable(operand1->getName()));
             cmp->setLeft(op1);
         }
        //end of operand 1

        //handle operand 2
        if(Constant *c=dyn_cast<Constant>(operand2)){
            if(c->getType()->isIntegerTy()){
                int val=c->getUniqueInteger().getSExtValue();
                ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                cmp->setRight(constantPtr);
                // check for pre condition here
            }

            if(c->getType()->isDoubleTy()){

                if (ConstantFP *Op = dyn_cast<ConstantFP>(c)) {
                    double val=Op->getValueAPF().convertToDouble();
                    ConstValue::ConstantPtr constantPtr(new ConstValue(std::to_string(val)));
                    cmp->setRight(constantPtr);
                }


            }

        }
        else{
            //this is for variable
            Variable::VariablePtr op2(new Variable(operand2->getName()));
            cmp->setRight(op2);
        }
        //end of operand 2
        switch (fCmpInst->getPredicate()){
            case llvm::FCmpInst::FCMP_OLE:
                    //do something here
                    cmp->setOperatorSymbol(Operator::LTE);
                    break;
        }

       // llvm::outs()<<"label: "<<label->value();
        if(!post_cond->replace(label,cmp)){
            if(wp_expr!=NULL){
                wp_expr->replace(label,cmp);
            }

        }
        llvm::outs()<<post_cond->value();

    }


}

void WP::transformIR()  {
    while(!wp_instruction.empty()){
                     //do the transformation here , to to retrieve again all the instruction maybe in a stack one by one
                     //worklist algo will work for this one i quest


                    Instruction *wp_inst= wp_instruction.top();
                  wp_instruction.pop();

        //Do the transformation here

                if(GetElementPtrInst *getElementPtrInst = dyn_cast<GetElementPtrInst>(wp_inst)){

                    Type *type=getElementPtrInst->getSourceElementType();
                    std::string name = getElementPtrInst->getOperand(0)->getName();
                    if(type->getTypeID()==Type::StructTyID)
                    {

                        Value *value=getElementPtrInst->getOperand(1);
                        if(Constant *c=dyn_cast<Constant>(value)){
                            if(c->getType()->getTypeID()==Type::IntegerTyID){
                                int index=c->getUniqueInteger().getSExtValue();
                                name+="->"+std::to_string(index);
                            }
                        }
                        std::string solution;
                        if((solution=getSolutionByOperandName(name))!="null"){

                            Value *newValue = ConstantInt::get(Type::getInt32Ty(wp_inst->getContext()), 10);
                            //wp_inst->get





                        }
                        Variable::VariablePtr structVar(new Variable(name));
                        llvm::outs()<<"WP "<<structVar->value();

                    }

                }



                     int i;
                     for(i=0;i<wp_inst->getNumOperands();i++){
                         outs()<<wp_inst->getOperand(i)->getName()<<"\n";
                         std::string operandName = wp_inst->getOperand(i)->getName();
                         if(operandName.compare("a")==0){
                             outs()<<"Here\n"<<operandName;
                            // instruction.dump();
                             Value *newValue = ConstantInt::get(Type::getInt32Ty(wp_inst->getContext()), 10);
                             wp_inst->setOperand(i,newValue);
                           wp_inst->dump();
                         }

                         if(operandName.compare("b")==0){
                             outs()<<"Here\n"<<operandName;
                             // instruction.dump();
                             Value *newValue = ConstantInt::get(Type::getInt32Ty(wp_inst->getContext()), 15);
                             wp_inst->setOperand(i,newValue);
                             wp_inst->dump();
                         }
                         if(operandName.compare("c")==0){
                             outs()<<"Here\n"<<operandName;
                             // instruction.dump();
                             Value *newValue = ConstantInt::get(Type::getInt32Ty(wp_inst->getContext()), 20);
                             wp_inst->setOperand(i,newValue);
                             wp_inst->dump();
                         }
                     }






                     wp_inst->dump();
                 }
}

void WP::createPostCondition(const CallInst *callInst, const Function *function) {
    std::string post_cond_string;

    if(ConstantExpr *constEx=dyn_cast<ConstantExpr>(callInst->getArgOperand(0))){
                    if(GlobalVariable *constGlobal=dyn_cast<GlobalVariable>(constEx->getOperand(0)))
                    {
                      //  constEx->getOperand(0)->dump();
                        if(ConstantArray *constArray=dyn_cast<ConstantArray>(constGlobal->getInitializer())){

                           // constArray->dump();
                        }
                        if(ConstantDataArray *constDataArray = dyn_cast<ConstantDataArray>(constGlobal->getInitializer())){

                            post_cond_string=constDataArray->getAsString();
                        }
                    }
                }

  //  outs()<<post_cond_string<<" POST\n";
    //Hardcoded as for now waiting for the parser to be completed

    llvm::outs()<<"Post condition String==>>"<<post_cond_string<<"\n";
    //int line = std::stoi(post_cond_string);

  int line = std::stoi(post_cond_string);

    if(line==119){
        llvm::outs()<<"119\n";
        performWPFlag = true;
        Variable::VariablePtr postVar(new Variable("wp_default"));
        ConstValue::ConstantPtr left(new ConstValue("0"));
        ConstValue::ConstantPtr right(new ConstValue("342"));
        BinaryOp::BinaryOpPtr gte(new BinaryOp(GT, postVar, left));
        BinaryOp::BinaryOpPtr lte(new BinaryOp(LT, postVar, right));
        BinaryOp::BinaryOpPtr AND(new BinaryOp(::AND, gte, lte));
        post_cond =AND;
       llvm::outs()<<post_cond->value();
        return;
    }



    // this string will pass to a function stringToExpr(std::string str) : Expr(){}
    // Once we have that we assign it to post cond;

        /*Mannual work for time being  */

  /*  Variable::VariablePtr postVar(new Variable("wp_default"));
    ConstValue::ConstantPtr left(new ConstValue("1"));
    ConstValue::ConstantPtr right(new ConstValue("4"));

    BinaryOp::BinaryOpPtr gte(new BinaryOp(GTE, postVar, left));
    BinaryOp::BinaryOpPtr lte(new BinaryOp(LTE, postVar, right));
    BinaryOp::BinaryOpPtr AND(new BinaryOp(::AND, gte, lte));
    post_cond =AND;
    llvm::outs()<<post_cond->value();
    performWPFlag = true;*/
}


Expr::ExprPtr WP::getWPExpression() const {
    return wp_expr;
}

Expr::ExprPtr WP::getPostCondition() const {
    return post_cond;
}

bool WP::isArrayExist(std::string name) const {
    if(arrays.find(name)==arrays.end())
        return false;
    return true;
}

bool WP::insertArrayByIndex(std::string name,int index,Expr::ExprPtr exprPtr) {
    llvm::outs()<<"begin:insert"<<exprPtr->value()<<"\n";

    if(isArrayExist(name)){
        //Modify the existing array
        auto it = arrays.find(name);
        //modify the iterator here
       //
        if(it!=arrays.end()){
            //If Array already present we look if the index is also present if yes the do the replacement
            //create a variable with that name

            auto val=it->second->getValue(index);
            if(val!=NULL){
                Variable::VariablePtr target(new Variable(name));
                //replace it then
                val->replace(target,exprPtr);
                //hope it should replace by reference
            }
        }
       it->second->insertValue(exprPtr,index);

        return true;
    }

    else{
        Array::ArrayPtr arrayPtr(new Array(name));
        //insert a new array
        arrayPtr->insertValue(exprPtr,index);
        arrays.insert(std::pair<std::string,Array::ArrayPtr>(name,arrayPtr));


    }



}

std::string WP::getSolutionByOperandName(std::string operandName) {
    for(auto it:solutionMap){
        if(it.first.compare(operandName)==0){
            return it.second;
        }
    }
    return "null";

}









