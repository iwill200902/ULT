#include <iostream>
#include <thread>
#include <chrono>
#include <barrier> 

const int MAX_THREADS = 4000;

// メインスレッド + 4000個のスレッド = 計4001個が揃うまで待つバリア
std::barrier sync_barrier(MAX_THREADS + 1);

void heavy_task_klt() {
    // 1. ここで全スレッドが揃うまで、CPUを消費せずに眠って待つ
    sync_barrier.arrive_and_wait();

    // --- ここから純粋な計算とコンテキストスイッチの計測 ---
    volatile int counter = 0;
    for (int i = 0; i < 2000; i++) {
        counter += i;
        
        if (i == 1000) {
            std::this_thread::yield(); 
        }
    }
}

int main() {
    std::cout << "========== [KLT (pthread) Pure Execution Benchmark] ==========\n";
    
    std::thread* threads[MAX_THREADS] = { nullptr };

    std::cout << "Creating " << MAX_THREADS << " threads (Timer is STOPPED)...\n";

    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i] = new std::thread(heavy_task_klt);
    }

    std::cout << "Starting execution...\n";

    // --- 計測開始 ---
    auto start = std::chrono::high_resolution_clock::now();

    // 2. メインスレッドがバリアに到達することで、4000個のスレッドが一斉に解放される
    sync_barrier.arrive_and_wait();

    // 全スレッドの終了を待つ
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i]->joinable()) {
            threads[i]->join();
        }
    }

    // --- 計測終了 ---
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    for (int i = 0; i < MAX_THREADS; i++) {
        delete threads[i];
    }

    std::cout << "All KLT threads completed.\n";
    std::cout << "Time taken: " << duration << " us (microseconds)\n";
    std::cout << "==============================================================\n";

    return 0;
}