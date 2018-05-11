// Created by Bernard on 15/6/16.
//

#include <IRPass.h>

    //Dependency Between passes can be handle here
        void IRPass::getAnalysisUsage(AnalysisUsage &AU) const  {
            AU.setPreservesAll();
        }


         bool IRPass::runOnFunction(Function &F) {
            outs()<<"in  :"<<F.getName()<<"()\n";

            for(Function::iterator BB=F.begin(),BBE=F.end(); BB!=BBE;++BB){


               for(BasicBlock::iterator BBI = BB->begin();BBI!=BB->end();++BBI){

                        Instruction &instruction = *BBI;
                      //Construct a Symbol Table from Here
                       ParseIR parseIR;
                        IR_Record record;
                         record=parseIR.parseInstruction(instruction);
                        record.blockLabel=BB->getName();

                        symbolicTable.putIR_Records(record);
               }
            }
            return false;
        }

         SymbolicTable IRPass::getSymbolTable() const {
             return this->symbolicTable;
         }


char IRPass::ID = 0;
//For opt

static RegisterPass<IRPass> X("IRPass","Weakest Precondition Analysis");
// Automatically enable the pass for Clang .

static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
    PM.add(new IRPass());
}
static RegisterStandardPasses
        RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                       registerPass);

