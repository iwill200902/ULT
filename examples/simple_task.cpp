/*
** Output:
** Main: Going to sleep...
** Task A - Start
** Task B - Start
** Task A - End
** Task B - End
** Main: All tasks completed. Shutting down. 
*/


#include <iostream>
#include "ult.hpp"

// タスクA
void taskA() {
    std::cout << "Task A - Start" << std::endl;
    yield(); // タスクBへ処理を譲る
    std::cout << "Task A - End" << std::endl;
    remove_thread(); // スレッドの安全な破棄
}

// タスクB
void taskB() {
    std::cout << "Task B - Start" << std::endl;
    yield(); // タスクAへ処理を譲る
    std::cout << "Task B - End" << std::endl;
    remove_thread();
}

int main() {
    // 1. メインスレッドの登録
    register_main();

    // 2. ユーザースレッドの生成
    uthread(taskA);
    uthread(taskB);

    std::cout << "Main: Going to sleep..." << std::endl;
    
    // 3. スケジューラ起動（メインスレッドはここで休眠）
    yield();

    // 全タスク完了後にここに戻ってくる
    std::cout << "Main: All tasks completed. Shutting down." << std::endl;
    return 0;
}