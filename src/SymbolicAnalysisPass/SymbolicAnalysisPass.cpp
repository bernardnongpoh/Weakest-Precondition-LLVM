//
// Created by heprit on 27/6/16.
//

#include "SymbolicAnalysisPass.h"
#include <IRPass.h>
void SymbolicAnalysisPass::getAnalysisUsage(AnalysisUsage &AU) const  {
    AU.addRequired<IRPass>();

}

bool SymbolicAnalysisPass::runOnFunction(Function &F) {

    SymbolicTable irpass=getAnalysis<IRPass>().getSymbolTable();
    irpass.printAllRecords();

    return false;

}



char SymbolicAnalysisPass::ID = 0;
//For opt

static RegisterPass<SymbolicAnalysisPass> X("SymbolicAnalysisPass","Weakest Precondition Analysis: 2nd Pass ");




// Automatically enable the pass for Clang .

static void registerSymbolicAnalysisPass(const PassManagerBuilder &,
                                         legacy::PassManagerBase &PM) {
    PM.add(new SymbolicAnalysisPass());
}
static RegisterStandardPasses
        RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                       registerSymbolicAnalysisPass);
