#include <iostream>
#include <chrono>
#include "ult.hpp"

const int MAX_THREADS = 4000;

void heavy_task_ult() {
    // --- ここから純粋な計算とコンテキストスイッチの計測 ---
    volatile int counter = 0;
    for (int i = 0; i < 2000; i++) {
        counter += i;
        
        // 中間地点で自作のコンテキストスイッチを発動
        if (i == 1000) {
            yield(); 
        }
    }
    
    // スレッドの終了処理
    remove_thread();
}

int main() {
    std::cout << "========== [ULT Pure Execution Benchmark] ==========\n";
    register_main();

    std::cout << "Creating " << MAX_THREADS << " threads (Timer is STOPPED)...\n";

    // 1. タイマーの「外」で、ユーザー空間の配列に4000個のタスクを登録する
    for (int i = 0; i < MAX_THREADS; i++) {
        if (uthread(heavy_task_ult) == -1) {
            std::cerr << "Error: Failed to create thread " << i << "\n";
            return 1;
        }
    }

    std::cout << "Starting execution...\n";

    // --- 計測開始 ---
    auto start = std::chrono::high_resolution_clock::now();

    // 2. スケジューラ起動（ここから4000個のスレッドが一気に処理される）
    yield();

    // --- 計測終了 ---
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "All ULT threads completed.\n";
    std::cout << "Time taken: " << duration << " us (microseconds)\n";
    std::cout << "====================================================\n";

    return 0;
}