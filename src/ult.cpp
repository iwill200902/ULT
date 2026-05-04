#include "ult.hpp"

#include <array>
#include <queue>

constexpr std::size_t MAX_THREAD = 4096;

std::array<TCB, MAX_THREAD> tcb_table;
std::queue<TCB*> ready_queue;
TCB* current_tcb = nullptr;
TCB* main_tcb = nullptr;


int64_t make_thread(void (*fanc)(), TCB*& new_TCB);


int64_t uthread(void (*fanc)()){ //新たなthreadを作るときの雑務処理
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

int64_t make_thread(void (*fanc)(), TCB*&new_TCB){ //新しいthreadを作る処理
    int check_count = 0;

    while(new_TCB->state != UNUSED){ //空いているTCBを探すためのloop
        new_TCB++;
        check_count++;
        if((new_TCB - tcb_table.data()) == MAX_THREAD){ //探してときにtcb_tableの最後までいくと、初めに戻す
            new_TCB = tcb_table.data();
        }
        if(check_count == MAX_THREAD){ //すべて使われていたら、-1を返す
            return-1;
        }
    }

    if(new_TCB->stack != nullptr){
        delete[] new_TCB->stack;
    }
    //作ったthreadの初期化
    new_TCB->init_stack(fanc);
    new_TCB->init_registers();
    new_TCB->init_state();
    new_TCB->tid = new_TCB - tcb_table.data();

    return new_TCB->tid;
}

void register_main(){ //main関数のthreadをTCBに登録する関数
    current_tcb = tcb_table.data();
    main_tcb = tcb_table.data();

    current_tcb->tid = 0;
    current_tcb->state = RUNNING;
    current_tcb->stack = nullptr;

    return;
}

int64_t yield(){ //TCBをswitchするための関数
    if(ready_queue.empty()){
        return 0;
    }
    
    //次のtcbをpopする
    TCB* next_tcb = ready_queue.front();    
    ready_queue.pop();

    //stateの更新
    if(current_tcb == main_tcb){
        //main_tcbだったら
        current_tcb->state = BLOCK;
    }else{
        current_tcb->state = READY;
    }
    next_tcb->state = RUNNING;

    //current_tcbの更新(main関数ならpushしない
    if(current_tcb == main_tcb){
    }else{
        ready_queue.push(current_tcb);
    }
    TCB* back_tcb = current_tcb;
    current_tcb = next_tcb;

    //context_switch
    context_switch(back_tcb->registers, next_tcb->registers);
    
    return 0;
}

void remove_thread(){
    if(ready_queue.empty()){
        main_tcb->state = READY;
        ready_queue.push(main_tcb);
    }
    
    //次のtcbをpopする
    TCB* next_tcb = ready_queue.front();    
    ready_queue.pop();

    //stateの更新
    current_tcb->state = UNUSED;
    next_tcb->state = RUNNING;

    //current_tcbの更新
    TCB* back_tcb = current_tcb;
    current_tcb = next_tcb;
    
    context_switch(back_tcb->registers, next_tcb->registers);

    return;
}