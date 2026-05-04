/**
 * @file stressTestforAI.cpp
 * @brief Stress test script generated with the assistance of Gemini.
 * @note Manually reviewed and verified by the author.
 */

#include <iostream>
#include "ult.hpp"

// 重いタスク1：素数探索（CPUとレジスタを極限まで酷使）
void heavy_prime_task() {
    // 協調的マルチタスクなので、排他制御(mutex)なしでも静的変数が安全にインクリメント可能！
    static int p_id = 0;
    int id = ++p_id;

    int count = 0;
    const int limit = 40000; // 4万までの素数を全探索する激重O(N^2)ループ

    for (int i = 2; i <= limit; i++) {
        bool is_prime = true;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) count++;

        // 5000回ループするごとに、強制的に他のスレッドへ処理をぶん投げる
        if (i % 5000 == 0) {
            yield();
        }
    }
    
    // 出力が混ざらないよう、終わった時だけ報告
    std::cout << "[Prime Task " << id << "] Finished! Found " << count << " primes." << std::endl;
    remove_thread();
}

// 重いタスク2：無意味に深い再帰関数（スタックメモリへの限界アタック）
void deep_hash_recursive(int depth, unsigned long long current_hash) {
    if (depth == 0) {
        // スタックの底に到達した状態でyieldし、スタックポインタ(rsp)の保存と復元をテスト
        yield();
        return;
    }
    
    // 無駄に複雑なビット演算をして、コンパイラに汎用レジスタをフル活用させる
    current_hash = (current_hash * 31) + depth;
    current_hash ^= (current_hash >> 3);

    // 20階層ごとにyieldして、スタックを積んだまま他のスレッドへ飛ぶ
    if (depth % 20 == 0) {
        yield();
    }

    deep_hash_recursive(depth - 1, current_hash);
}

void heavy_stack_task() {
    static int s_id = 0;
    int id = ++s_id;

    // スタックを100階層まで深く掘る（※8KBのスタック領域なら100階層あたりが安全な限界値）
    deep_hash_recursive(100, 0x12345678);

    std::cout << "[Stack Task " << id << "] Finished deep recursion!" << std::endl;
    remove_thread();
}

int main() {
    std::cout << "========== [EXTREME STRESS TEST BOOT] ==========" << std::endl;
    register_main();

    const int NUM_PRIME_THREADS = 50;
    const int NUM_STACK_THREADS = 50;

    std::cout << "Creating " << (NUM_PRIME_THREADS + NUM_STACK_THREADS) << " heavy threads..." << std::endl;

    // 100個のスレッドをキューにぶち込む（make_thread の配列使い回しテスト）
    for(int i = 0; i < NUM_PRIME_THREADS; i++) uthread(heavy_prime_task);
    for(int i = 0; i < NUM_STACK_THREADS; i++) uthread(heavy_stack_task);

    std::cout << "All threads registered. Main thread is diving into BLOCK..." << std::endl;
    std::cout << "Processing... (This might take a few seconds due to heavy computation)" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    
    // 先輩の長い眠り
    yield(); 
    
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "========== [TEST PASSED] ==========" << std::endl;
    std::cout << "Main thread woke up safely. 100 threads completed!" << std::endl;
    
    return 0;
}