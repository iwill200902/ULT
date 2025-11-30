#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cstdint>

//コンテキストスイッチに必要なレジスタデータを入れるの構造体
struct Context{
    uint64_t rip;
    uint64_t rsp;
};

extern "C" {
    void switch_context();
}
#endif // CONTEXT_HPP