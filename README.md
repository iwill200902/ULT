# User Level Thread

## 概要 (Overview)
C++で制作した、x86_64上で動作するユーザレベルスレッド(ULT)ライブラリです。\
協調的スケジューリングで動作し、スケジューリングアルゴリズムは現在FIFOのみを用いています。将来的には、様々なアルゴリズムを選択できるようにするつもりです。\
`pthread`などの他のスレッドライブラリは使用せず、全てを`C++`, `Assembly`で作成しています。\
スレッドの生成、破棄、スケジューリングを全てユーザ空間で行います。

将来的に難読化ライブラリに用いることを想定しています。


## 使い方 (How to use)
`register_main`, `uthread`, `yield`, `remove_thread` の4つのAPIを提供しています。

* **`register_main()`**
    * main関数のスレッドをULTのThread Cotrol Block (TCB)に登録するための関数です。
    * main関数の先頭で**絶対に**呼ぶ必要があります。
* **`uthread(*func)`**\
    * 新たなスレッドを作成するための関数です。
    * 引数には実行したい関数のポインタを渡します。
    * 返り値は成功するとスレッドIDが、エラー時は`-1`が返ってきます。
* **`yield()`**\
    * 自発的にCPUを手放し、キューで待機している次のスレッドに処理を譲るための関数です。
    * コンテキストスイッチが発生します。
* **`remove_thread()`**\
    * 今現在のスレッドを安全に破棄し、メモリを解放するための関数です。
    * スレッドとして動かした関数の最後で**絶対に**呼ぶ必要があります。

### サンプルプログラム (sample program)
[`simple_task.cpp`](/examples/simple_task.cpp)
```cpp
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
```

### 実行結果
```Plaintext
Main: Going to sleep...
Task A - Start
Task B - Start
Task A - End
Task B - End
Main: All tasks completed. Shutting down.
```

## 負荷テストの実施
### **テスト概要**
多数のスレッド生成・コンテキストスイッチの確実性を検証するために、負荷テストを実施した。

*   **テスト内容**: Geminiを用いて生成したシナリオに基づき、生成可能な最大スレッド数(4096個)を生成・破棄し、スタックオーバーフローやメモリリークが発生しないかを確認。
*  **ソースコード**: [`stressTestforAI.cpp`](/tests/stressTestforAI.cpp)
*   **検証項目**:
    *   高頻度な `yield()` によるコンテキストスイッチの正確性。
    *   大量スレッド終了時の `remove_thread()` によるメモリ解放の整合性。

### 確認されたミス (修正済み)
* [呼び出し規約(ABI)](https://gitlab.com/x86-psABIs/x86-64-ABI)の違反が発覚した。\
    16ビットアラインメントを違反していたことにより、セグメンテーション フォルトで落散ることが確認できた。

## ビルド方法 (How to build)
```Bash
$ git clone https://github.com/iwill200902/ULT.git
$ cd ULT
$ cmake -S . -B build
$ cmake --build build
```

## 性能評価 (Performance Evaluation)

### **実験概要**
カーネルレベルスレッド（KLT）である `std::thread` と、本ライブラリ（ULT）のコンテキストスイッチおよび実行速度の性能比較を実施した。

* **検証内容**: 4,000個のスレッドを生成・実行し、各スレッドが計算の途中で必ず1回明示的に `yield()` を呼び出してコンテキストスイッチを発生させた。また、ソースコードはAIで生成し、最適化はなし(-O0)に設定した。
* **ソースコード**: [`benchmark_ult.cpp`](/tests/benchmark_ult.cpp), [`benchmark_klt.cpp`](/tests/benchmark_klt.cpp)

### **計測結果**
4,000スレッドの同時実行において、すべてのスレッドが実行を開始し、コンテキストスイッチを経て安全に全処理を終了するまでの総時間をマイクロ秒（μs）単位で計測した。

| スレッドモデル | 計測時間 (Time Taken) | 速度倍率 |
| :--- | :--- | :--- |
| **KLT (`std::thread`)** | **26,931 us** | 基準 (1.0x) |
| **本ライブラリ (ULT)** | **3,134 us** | **約 8.59x 高速** |