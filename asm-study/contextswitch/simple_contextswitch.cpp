#include <iostream>
#include <cstdint>
#include <queue>
#include <stack>

struct saved_register
{
    uint64_t rbx;
    uint64_t rbp;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
};


void context_switch(saved_register& thisThread, saved_register& nextThread);

int main(){
    saved_register thread0{}, thread1{};

    context_switch(thread0, thread1);
    context_switch(thread1, thread0);

    return 0;
}

void context_switch(saved_register& thisThread, saved_register& nextThread){

    asm volatile(
        "mov %%rbx, %[t_rbx]\n\t"
        "mov %%rbp, %[t_rbp]\n\t"
        "mov %%r12, %[t_r12]\n\t"
        "mov %%r13, %[t_r13]\n\t"
        "mov %%r14, %[t_r14]\n\t"
        "mov %%r15, %[t_r15]\n\t"
        "mov %[n_rbx], %%rbx\n\t"
        "mov %[n_rbp], %%rbp\n\t"
        "mov %[n_r12], %%r12\n\t"
        "mov %[n_r13], %%r13\n\t"
        "mov %[n_r14], %%r14\n\t"
        "mov %[n_r15], %%r15\n\t"
        :[t_rbx] "=m" (thisThread.rbx), [t_rbp] "=m" (thisThread.rbp), [t_r12] "=m" (thisThread.r12), [t_r13] "=m" (thisThread.r13), [t_r14] "=m" (thisThread.r14), [t_r15] "=m" (thisThread.r15)
        :[n_rbx] "m" (nextThread.rbx), [n_rbp] "m" (nextThread.rbp), [n_r12] "m" (nextThread.r12), [n_r13] "m" (nextThread.r13), [n_r14] "m" (nextThread.r14), [n_r15] "m" (nextThread.r15)
        :
    );

    //現在のスレッドのレジスタ情報の保存
    //構造体を現在のスレッドスタックにpush
    
    //contextswitch(RSP交換)

    //switch後のスタックから構造体のpop
    //レジスタ情報の変更
}