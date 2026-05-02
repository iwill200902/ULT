#pragma once
#include "ult/thread.hpp"
#include <array>

constexpr std::size_t MAX_THREAD = 4096;
extern std::array<TCB, MAX_THREAD> tcb_table;

extern int64_t uthread(void (*fanc)());