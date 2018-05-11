//
// Created by Bernard on 27/6/16.
//

#include "llvm/Pass.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "SymbolicTable.h"
#include "ParseIR.h"
#include <ParseIR.h>

using namespace llvm;

class IRPass: public FunctionPass {



public:
    static char ID;
    SymbolicTable symbolicTable;
    void getAnalysisUsage(AnalysisUsage &AU) const override;
    IRPass() : FunctionPass(ID) {}
    virtual bool runOnFunction(Function &F);
    SymbolicTable getSymbolTable() const;

};


