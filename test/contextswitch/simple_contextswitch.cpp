#ifndef CONTEXTSWITCH_HPP
#define CONTEXTSWITCH_HPP
#include "contextswitch/contextswitch.hpp"
#endif

#include <iostream>
#include <queue>
#include <stack>




int main(){
    saved_register thread0{}, thread1{};

    context_switch(thread0, thread1);
    context_switch(thread1, thread0);

    return 0;
}

