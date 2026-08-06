#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    void distance_kernel_c(
        const float* X1,
        const float* X2,
        const float* Y1,
        const float* Y2,
        float* Z,
        long long n);

    void distance_kernel_asm(
        const float* X1,
        const float* X2,
        const float* Y1,
        const float* Y2,
        float* Z,
        long long n);

#ifdef __cplusplus
}
#endif