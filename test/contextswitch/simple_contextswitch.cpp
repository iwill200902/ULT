#include "ult.hpp"

#include <iostream>
#include <queue>
#include <stack>

void add();

int main(){
    register_main();
    int a = uthread(add);
    std::cout << a << std::endl;
    if(a == -1){
        std::cout << "error occurred" << std::endl;
    }
    yield ();
    std::cout << a << std::endl;

    return 0;
}

void add(){
    int a = 1;
    int b = 2;

    a = a + b;

    std::cout << a << std::endl;

    yield();

    std::cout << a << std::endl;    

    remove_thread();
}
