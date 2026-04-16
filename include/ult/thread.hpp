#pragma once
#include "contextswitch.hpp"

#include <stack>
#include <cstdint>

enum ThreadState {
    RUNNING,
    READY,
    BLOCK
};


struct TCB{
    int id;
    SavedRegister registers;
    ThreadState state;
    std::stack<uint8_t> stack;
};