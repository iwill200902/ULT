#pragma once
#include "ult/thread.hpp"
#include <array>
#include <queue>

constexpr std::size_t MAX_THREAD = 4096;
extern std::array<TCB, MAX_THREAD> tcb_table;
extern std::queue<TCB*> tcb_queue;

extern int64_t uthread(void (*fanc)());
extern int64_t register_main();