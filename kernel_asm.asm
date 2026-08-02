.code

; void distance_kernel_asm(const double* X1, const double* X2, const double* Y1, const double* Y2, double* Z, long long n)
; rcx = X1, rdx = X2, r8 = Y1, r9 = Y2, [rsp+40] = Z, [rsp+48] = n
distance_kernel_asm PROC
    mov r10, [rsp+40]                   ; r10 = Z pointer
    mov r11, [rsp+48]                   ; r11 = n

    xor rax, rax                        ; rax = i = 0

kernel_loop:
    cmp rax, r11
    jge kernel_done

    movsd xmm0, real8 ptr [rcx + rax*8] ; xmm0 = X1[i]
    movsd xmm1, real8 ptr [rdx + rax*8] ; xmm1 = X2[i]
    subsd xmm1, xmm0                    ; xmm1 = X2[i] - X1[i]
    mulsd xmm1, xmm1                    ; xmm1 = (X2[i]-X1[i])^2

    movsd xmm2, real8 ptr [r8 + rax*8]  ; xmm2 = Y1[i]
    movsd xmm3, real8 ptr [r9 + rax*8]  ; xmm3 = Y2[i]
    subsd xmm3, xmm2                    ; xmm3 = Y2[i] - Y1[i]
    mulsd xmm3, xmm3                    ; xmm3 = (Y2[i]-Y1[i])^2

    addsd xmm1, xmm3                    ; xmm1 = sum of squares
    sqrtsd xmm1, xmm1                   ; xmm1 = sqrt(sum) = distance

    movsd real8 ptr [r10 + rax*8], xmm1 ; Z[i] = distance

    inc rax
    jmp kernel_loop

kernel_done:
    ret
distance_kernel_asm ENDP

end