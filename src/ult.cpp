#include "ult.hpp"

std::array<TCB, MAX_THREAD> tcb_table;
std::queue<TCB*> ready_queue;

int64_t make_thread(void (*fanc)(), TCB*& new_TCB);


int64_t uthread(void (*fanc)()){
    static auto new_TCB = tcb_table.data();

    if(make_thread(fanc, new_TCB) ==-1){
        return -1;
    }
    ready_queue.push(new_TCB);

    int new_tid = new_TCB->tid;

    new_TCB++;
    if((new_TCB - tcb_table.data()) == MAX_THREAD){
        new_TCB = tcb_table.data();
    }
    return new_tid;
}

int64_t make_thread(void (*fanc)(), TCB*&new_TCB){
    int check_count = 0;

    while(new_TCB->state != UNUSED){
        new_TCB++;
        check_count++;
        if((new_TCB - tcb_table.data()) == MAX_THREAD){
            new_TCB = tcb_table.data();
        }
        if(check_count == MAX_THREAD){
            return-1;
        }
    }
    new_TCB->init_stack(fanc);
    new_TCB->init_state();
    new_TCB->tid = new_TCB - tcb_table.data();

    return new_TCB->tid;
}
