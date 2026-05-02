#pragma once
#include <cstdint>

struct SavedRegister
{
    uint64_t rbx;
    uint64_t rbp;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rsp;
};

extern "C" void context_switch(SavedRegister& currentThread, SavedRegister& nextThread);