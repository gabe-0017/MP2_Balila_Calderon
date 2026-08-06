#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "kernel.h"

static double* alloc_vector(long long n) {
    double* v = (double*)malloc(n * sizeof(double));
    if (!v) {
        fprintf(stderr, "malloc failed for n = %lld\n", n);
        exit(1);
    }
    return v;
}

static void init_random(double* v, long long n) {
    for (long long i = 0; i < n; i++) {
        v[i] = (double)rand() / RAND_MAX * 100.0; // random value 0-100
    }
}

static double time_kernel(
    void (*kernel)(const double*, const double*, const double*, const double*, double*, long long),
    const double* X1, const double* X2, const double* Y1, const double* Y2, double* Z, long long n,
    int runs)
{
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    double total_seconds = 0.0;
    for (int r = 0; r < runs; r++) {
        QueryPerformanceCounter(&start);
        kernel(X1, X2, Y1, Y2, Z, n);
        QueryPerformanceCounter(&end);
        total_seconds += (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    }
    return total_seconds / runs;
}

static int check_correctness(const double* Z_c, const double* Z_asm, long long n) {
    const double EPSILON = 1e-6;
    for (long long i = 0; i < n; i++) {
        if (fabs(Z_c[i] - Z_asm[i]) > EPSILON) {
            printf("Mismatch at index %lld: C=%.9f, ASM=%.9f\n", i, Z_c[i], Z_asm[i]);
            return 0;
        }
    }
    return 1;
}

static void print_first_ten(const char* label, const double* Z) {
    printf("%s first 10: ", label);
    for (int i = 0; i < 10; i++) {
        printf("%.9f ", Z[i]);
    }
    printf("\n");
}

int main(void) {
    srand(42); // fixed seed so runs are reproducible

    // 2^26 = 67,108,864 doubles = 536,870,912 bytes = 512MB per vector = approx 3GB total
	// 2^27 = 134,217,728 doubles = 1,073,741,824 bytes = 1GB per vector = approx 6GB total
	// 2^28 = 268,435,456 doubles = 2,147,483,648 bytes = 2GB per vector = approx 12GB total
	// 2^29 = 536,870,912 doubles = 4,294,967,296 bytes = 4GB per vector = approx 24GB total
	// 2^30 = 1,073,741,824 doubles = 8,589,934,592 bytes = 8GB per vector = approx 48GB total
    
	// 2^28 will require me to close all other programs and keep at least 12GB of RAM free
	// 2^27 is the most I would like to push my 16GB RAM laptop
	// 2^26 would run comfotably on most modern machines
    long long sizes[] = { 1LL << 20, 1LL << 24, 1LL << 27 };
    int num_sizes = 3;
    int runs = 30;

    for (int s = 0; s < num_sizes; s++) {
        long long n = sizes[s];
        printf("\n=== n = %lld (2^%d) ===\n", n, (int)(log2((double)n) + 0.5));

        double* X1 = alloc_vector(n);
        double* X2 = alloc_vector(n);
        double* Y1 = alloc_vector(n);
        double* Y2 = alloc_vector(n);
        double* Z_c = alloc_vector(n);
        double* Z_asm = alloc_vector(n);

        init_random(X1, n);
        init_random(X2, n);
        init_random(Y1, n);
        init_random(Y2, n);

        double time_c = time_kernel(distance_kernel_c, X1, X2, Y1, Y2, Z_c, n, runs);
        double time_asm = time_kernel(distance_kernel_asm, X1, X2, Y1, Y2, Z_asm, n, runs);

        int correct = check_correctness(Z_c, Z_asm, n);

        print_first_ten("C  ", Z_c);
        print_first_ten("ASM", Z_asm);

        printf("Correctness check (C vs ASM): %s\n", correct ? "PASSED" : "FAILED");
        printf("Avg time over %d runs -- C: %.6f sec | ASM: %.6f sec\n", runs, time_c, time_asm);
        printf("Speedup (C / ASM): %.3fx\n", time_c / time_asm);

        free(X1); free(X2); free(Y1); free(Y2); free(Z_c); free(Z_asm);
    }

    return 0;
}
