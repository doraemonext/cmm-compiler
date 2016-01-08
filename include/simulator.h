#ifndef CMM_SIMULATOR_H
#define CMM_SIMULATOR_H

class Code {

};

class Stack {

};

class VarTable {

};

class FuncTable {

};

class Simulator {
public:
    Simulator() { }

private:
    Code code_;
    Stack stack_;
    VarTable var_table_;
    FuncTable func_table_;
    int eip_;
};

#endif //CMM_SIMULATOR_H
