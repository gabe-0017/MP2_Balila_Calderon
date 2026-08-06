#include <emmintrin.h> // SSE2 library for SIMD instructions
#include "kernel.h"

// Z[i] = sqrt((X2[i]-X1[i])^2 + (Y2[i]-Y1[i])^2)
// Uses scalar single-precision SSE instructions on XMM registers.
void distance_kernel_c(
    const float* X1,
    const float* X2,
    const float* Y1,
    const float* Y2,
    float* Z,
    long long n)
{
    for (long long i = 0; i < n; i++)
    {
        __m128 x1 = _mm_load_ss(&X1[i]);
        __m128 x2 = _mm_load_ss(&X2[i]);
        __m128 y1 = _mm_load_ss(&Y1[i]);
        __m128 y2 = _mm_load_ss(&Y2[i]);

        __m128 dx = _mm_sub_ss(x2, x1);
        dx = _mm_mul_ss(dx, dx);

        __m128 dy = _mm_sub_ss(y2, y1);
        dy = _mm_mul_ss(dy, dy);

        __m128 sum = _mm_add_ss(dx, dy);
        __m128 dist = _mm_sqrt_ss(sum);

        _mm_store_ss(&Z[i], dist);
    }
}