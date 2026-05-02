#include "ult/contextswitch.hpp"


#include <iostream>
#include <queue>
#include <stack>




int main(){
    SavedRegister thread0{}, thread1{};

    context_switch(thread0, thread1);
    context_switch(thread1, thread0);

    return 0;
}

