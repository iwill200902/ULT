#pragma once
#include <cstdint>

struct saved_register
{
    uint64_t rbx;
    uint64_t rbp;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
};

extern "C" void context_switch(saved_register& thisThread, saved_register& nextThread);