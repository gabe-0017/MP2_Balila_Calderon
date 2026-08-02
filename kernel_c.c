#include <emmintrin.h> // SSE2 library for SIMD instructions
#include "kernel.h"

// Z[i] = sqrt((X2[i]-X1[i])^2 + (Y2[i]-Y1[i])^2)
// Uses SSE2 scalar-double instructions (_sd) [movsd,subsd,mulsd,addsd,sqrtsd] on XMM registers
void distance_kernel_c(const double* X1, const double* X2, const double* Y1, const double* Y2, double* Z, long long n) {
    for (long long i = 0; i < n; i++) {
        __m128d x1 = _mm_load_sd(&X1[i]);
        __m128d x2 = _mm_load_sd(&X2[i]);
        __m128d y1 = _mm_load_sd(&Y1[i]);
        __m128d y2 = _mm_load_sd(&Y2[i]);

        __m128d dx = _mm_sub_sd(x2, x1);
        dx = _mm_mul_sd(dx, dx); // (X2-X1)^2

        __m128d dy = _mm_sub_sd(y2, y1);
        dy = _mm_mul_sd(dy, dy); // (Y2-Y1)^2

        __m128d sum = _mm_add_sd(dx, dy);
        __m128d dist = _mm_sqrt_sd(sum, sum);

        _mm_store_sd(&Z[i], dist);
    }
}