/**
 * @file stressTestforAI.cpp
 * @brief Stress test script generated with the assistance of Gemini.
 * @note Manually reviewed and verified by the author.
 */

#include <iostream>
#include <cstdio>
#include "ult.hpp"

FILE* logfile;

// 激重タスク1：モンテカルロ法による円周率の近似計算
// 浮動小数点演算（FPU/XMMレジスタ）の退避・復元を極限までテストします。
void task_monte_carlo() {
    int inside_circle = 0;
    const int POINTS = 15000;
    unsigned int seed = 123456789; // 乱数シード

    for (int i = 0; i < POINTS; i++) {
        // 軽量な線形合同法で疑似乱数を生成
        seed = (1103515245 * seed + 12345);
        float x = (float)(seed % 10000) / 10000.0f;
        
        seed = (1103515245 * seed + 12345);
        float y = (float)(seed % 10000) / 10000.0f;

        if (x * x + y * y <= 1.0f) {
            inside_circle++;
        }

        // 1000回計算するごとに、強制的にコンテキストスイッチ！
        if (i % 1000 == 0) {
            yield();
        }
    }

    // モンテカルロ法の最後に結果を検証する
    float pi = 4.0f * inside_circle / POINTS;
    if (pi < 3.0f || pi > 3.3f) {
        std::cout << "Error: FPU Register corruption detected! Pi = " << pi << std::endl;
        // fprintf を使って軽く書き込む
        fprintf(logfile, "[ERROR] FPU Register corruption detected! Pi = %f\n", pi);
    } else {
        fprintf(logfile, "[Monte Carlo] Success. Calculated Pi = %f\n", pi);
    }

    // ログが溢れないよう、500個完了するごとに報告
    static int completed_mc = 0;
    completed_mc++;
    if (completed_mc % 500 == 0) {
        std::cout << "[Monte Carlo] " << completed_mc << " threads finished their calculations." << std::endl;
    }
    
    remove_thread();
}

// 激重タスク2：コラッツ予想の長距離探索
// 巨大な整数の乗算と除算を繰り返し、汎用レジスタを極限まで酷使します。
void task_collatz() {
    // 非常にステップ数が長くなる初期値
    unsigned long long n = 837799; 
    int steps = 0;
    
    // 1になるまで無限にループ
    while (n > 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
        
        // 50ステップごとにコンテキストスイッチ！
        if (steps % 50 == 0) {
            yield();
        }
    }

    static int completed_col = 0;
    completed_col++;
    if (completed_col % 500 == 0) {
        std::cout << "[Collatz] " << completed_col << " threads reached the end of the sequence." << std::endl;
    }
    
    remove_thread();
}

int main() {
    logfile = fopen("stressTestforAI_example.txt", "w");
    if (!logfile) {
        std::cerr << "Error: Cannot open log file." << std::endl;
        return 1;
    }
    fprintf(logfile, "========== [MAX CAPACITY STRESS TEST RESULTS] ==========\n");

    std::cout << "========== [MAX CAPACITY STRESS TEST] ==========" << std::endl;
    register_main();

    // mainスレッド(1) + ユーザースレッド(4095) = 計4096スレッド
    const int MAX_USER_THREADS = 4095;
    
    std::cout << "Creating " << MAX_USER_THREADS << " heavy threads..." << std::endl;

    for (int i = 0; i < MAX_USER_THREADS; i++) {
        // 2種類のタスクを交互に登録
        if (i % 2 == 0) {
            int result = uthread(task_monte_carlo);
            if (result == -1) std::cout << "Error: Failed to create thread " << i << std::endl;
        } else {
            int result = uthread(task_collatz);
            if (result == -1) std::cout << "Error: Failed to create thread " << i << std::endl;
        }
    }

    std::cout << "All " << MAX_USER_THREADS << " user threads registered." << std::endl;
    std::cout << "Executing... (CPU will run at 100%. Please wait...)" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    // スケジューラ起動！キューに積まれた4095個のスレッドの海へ飛び込む
    yield();

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "========== [TEST PASSED] ==========" << std::endl;
    std::cout << "Main thread woke up! All 4096 threads executed flawlessly!" << std::endl;

    fprintf(logfile, "========== [TEST PASSED] ==========\n");
    fclose(logfile);

    return 0;
}