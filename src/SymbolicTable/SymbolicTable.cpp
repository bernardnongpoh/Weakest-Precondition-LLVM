//
// Created by Bernard Nongpoh on 24/6/16.
//

#include "SymbolicTable.h"


void SymbolicTable::putIR_Records(IR_Record ir_record) {
    this->IR_Records.push_back(ir_record);
}

std::list<IR_Record> SymbolicTable::getIR_Records() const {
    return this->IR_Records;
}

std::list<IR_Record> SymbolicTable::getIR_RecordsByVariableName(string variableName)  {

    std::list<IR_Record> records;
    //TODO:  Optimization can be done here using STL std::find()

    list<IR_Record>::iterator iterator;

    for(iterator=this->IR_Records.begin();iterator!=this->IR_Records.end();++iterator){
        IR_Record &record=*iterator;
        llvm::outs()<<record.ID;
    }




    return std::list<IR_Record>();
}

IR_Record SymbolicTable::getMostRecent_IR_RecordByVariableName(string variableName) const {
    return IR_Record();
}

std::list<IR_Record> SymbolicTable::getAllIR_RecordsByBlockID(string blockID) const {
    return std::list<IR_Record>();
}

void SymbolicTable::printAllRecords() const {

    for(IR_Record record:this->IR_Records){
        llvm::outs()<<"\n-----------------------\n";
        llvm::outs()<<record.blockLabel;
        llvm::outs()<<"\n-----------------------\n";
    }

}












