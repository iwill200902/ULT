#include <iostream>
#include <queue>
#include <stack>

struct saved_register
{
    size_t rbx;
    size_t rbp;
    size_t r12;
    size_t r13;
    size_t r14;
    size_t r15;
};


void context_switch();

int main(){
    std::queue<int> que;
}

void context_switch(){

    //現在のスレッドのレジスタ情報の保存
    //構造体を現在のスレッドスタックにpush
    
    //contextswitch(RSP交換)

    //switch後のスタックから構造体のpop
    //レジスタ情報の変更
}