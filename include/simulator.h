#ifndef CMM_SIMULATOR_H
#define CMM_SIMULATOR_H

#include "scope.h"

class Code {

};

class Stack {

};

class FuncTable {

};

class LabelTable {

};

class Simulator {
public:
    Simulator() { }

private:
    Code code_;
    Stack stack_;
    ScopeTree tree_;
    FuncTable func_table_;
    LabelTable label_table_;
    int eip_;
};

#endif //CMM_SIMULATOR_H
