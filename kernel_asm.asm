.code

; void distance_kernel_asm(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, long long n)
; rcx = X1, rdx = X2, r8 = Y1, r9 = Y2, [rsp+40] = Z, [rsp+48] = n

distance_kernel_asm PROC

    mov r10, QWORD PTR [rsp+40]    ; r10 = Z
    mov r11, QWORD PTR [rsp+48]    ; r11 = n

    xor rax, rax                   ; i = 0

kernel_loop:

    cmp rax, r11
    jge kernel_done

    ; dx = X2[i] - X1[i]
    movss xmm0, DWORD PTR [rcx + rax*4]
    movss xmm1, DWORD PTR [rdx + rax*4]

    subss xmm1, xmm0
    mulss xmm1, xmm1

    ; dy = Y2[i] - Y1[i]
    movss xmm2, DWORD PTR [r8 + rax*4]
    movss xmm3, DWORD PTR [r9 + rax*4]

    subss xmm3, xmm2
    mulss xmm3, xmm3

    ; dx^2 + dy^2
    addss xmm1, xmm3

    ; sqrt(dx^2 + dy^2)
    sqrtss xmm1, xmm1

    ; Z[i] = result
    movss DWORD PTR [r10 + rax*4], xmm1

    inc rax
    jmp kernel_loop

kernel_done:

    ret

distance_kernel_asm ENDP

END