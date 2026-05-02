#pragma once
#include "contextswitch.hpp"

#include <cstdint>

constexpr uint64_t STACK_SIZE = 4096;

enum ThreadState {
    UNUSED,
    RUNNING,
    READY,
    BLOCK
};


struct TCB{
    int tid;
    SavedRegister registers = {0};
    ThreadState state;
    uint8_t*stack;

    void init_stack(void (*fanc)()){
        stack = new uint8_t[STACK_SIZE];
        uint64_t * sp = (uint64_t*)(stack + STACK_SIZE);

        sp--;
        *sp = (uint64_t)fanc;

        registers.rsp = (uint64_t)sp;
    }

    void init_state(){
        state = READY;
    }
};