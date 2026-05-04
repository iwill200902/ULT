#pragma once
#include "ult/thread.hpp"

extern int64_t uthread(void (*fanc)());
extern void register_main();
extern int64_t yield();
extern void remove_thread();