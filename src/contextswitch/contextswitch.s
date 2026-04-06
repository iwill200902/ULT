    .file "contextswitch.s"
    .text
    .global context_switch
    .type context_switch, @function
context_switch:
endbr64
movq %rbx, 0x0(%rdi)
movq %rbp, 0x8(%rdi)
movq %r12, 0x10(%rdi)
movq %r13, 0x18(%rdi)
movq %r14, 0x20(%rdi)
movq %r15, 0x28(%rdi)


movq 0x0(%rsi), %rbx
movq 0x8(%rsi), %rbp
movq 0x10(%rsi), %r12
movq 0x18(%rsi), %r13
movq 0x20(%rsi), %r14
movq 0x28(%rsi), %r15
ret

.section .note.GNU-stack,"",@progbits