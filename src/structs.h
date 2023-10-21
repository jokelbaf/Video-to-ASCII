#ifndef STRUCTS_H
#define STRUCTS_H

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
