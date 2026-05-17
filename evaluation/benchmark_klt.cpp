#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

const int MAX_THREADS = 4000;

// 全スレッドを一斉にスタートさせるための信号
std::atomic<bool> start_signal{false};

void heavy_task_klt() {
    // メインスレッドから合図が出るまで待機（これで生成コストを無視できる）
    while (!start_signal.load()) {
        std::this_thread::yield(); 
    }

    // --- ここから純粋な計算とコンテキストスイッチの計測 ---
    volatile int counter = 0;
    for (int i = 0; i < 2000; i++) {
        counter += i;
        
        // 中間地点でカーネルのスケジューラに実行権を譲る
        if (i == 1000) {
            std::this_thread::yield(); 
        }
    }
}

int main() {
    std::cout << "========== [KLT (pthread) Pure Execution Benchmark] ==========\n";
    
    // std::vectorを使わず、ULTの固定長配列と完全に等価な条件にする
    std::thread* threads[MAX_THREADS] = { nullptr };

    std::cout << "Creating " << MAX_THREADS << " threads (Timer is STOPPED)...\n";

    // 1. タイマーの「外」で4000個のスレッドを生成し、待機状態にさせる
    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i] = new std::thread(heavy_task_klt);
    }

    // 全てのスレッドがwhileループに到達するのを確実に待つためのわずかな猶予
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Starting execution...\n";

    // --- 計測開始 ---
    auto start = std::chrono::high_resolution_clock::now();

    // 全スレッドに一斉スタートの合図を出す！
    start_signal.store(true);

    // 全スレッドの終了を待つ
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i]->joinable()) {
            threads[i]->join();
        }
    }

    // --- 計測終了 ---
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // 測定後にメモリを解放
    for (int i = 0; i < MAX_THREADS; i++) {
        delete threads[i];
    }

    std::cout << "All KLT threads completed.\n";
    std::cout << "Time taken: " << duration << " us (microseconds)\n";
    std::cout << "==============================================================\n";

    return 0;
}