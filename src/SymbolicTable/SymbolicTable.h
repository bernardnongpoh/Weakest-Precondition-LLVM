//
// Created by bernard on 24/6/16.
//

#ifndef APPROXWP_SYMBOLICTABLE_H
#define APPROXWP_SYMBOLICTABLE_H

#include <IR_Record.h>
#include <llvm/Support/raw_ostream.h>
using namespace std;

class SymbolicTable {
private:
    std::list<IR_Record> IR_Records;
public:
    void putIR_Records(IR_Record ir_record);
    std::list<IR_Record> getIR_Records() const;
    std::list<IR_Record> getIR_RecordsByVariableName(string variableName) ;
    IR_Record getMostRecent_IR_RecordByVariableName(string variableName) const;
    std::list<IR_Record> getAllIR_RecordsByBlockID(string blockID) const;
    void printAllRecords() const;
};


#endif //APPROXWP_SYMBOLICTABLE_H
