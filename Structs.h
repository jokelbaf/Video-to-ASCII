#ifndef Structs_h
#define Structs_h

#include <string>

struct FlagActions {
    std::string flag;
    std::string alias;
    bool isAct;
    void (*fAct)();
};

struct FlagOps {
    std::string flag;
    std::string val;
};

#endif
