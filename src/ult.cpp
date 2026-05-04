#include "ult.hpp"

#include <array>
#include <queue>

constexpr std::size_t MAX_THREAD = 4096;

std::array<TCB, MAX_THREAD> tcb_table;
std::queue<TCB*> ready_queue;
TCB* current_tcb = nullptr;


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

void register_main(){
    current_tcb = tcb_table.data();

    current_tcb->tid = 0;
    current_tcb->state = RUNNING;
    current_tcb->stack = nullptr;

    return;
}

int64_t yield(){
    if(ready_queue.empty()){
        return -1;
    }
    
    //次のtcbをpopする
    TCB* next_tcb = ready_queue.front();    
    ready_queue.pop();

    //stateの更新
    current_tcb->state = READY;
    next_tcb->state = RUNNING;

    //current_tcbの更新
    ready_queue.push(current_tcb);
    TCB* back_tcb = current_tcb;
    current_tcb = next_tcb;

    //context_switch
    context_switch(back_tcb->registers, next_tcb->registers);
    
    return 0;
}