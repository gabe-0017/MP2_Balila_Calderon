#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	void distance_kernel_c(const double* X1, const double* X2, const double* Y1, const double* Y2, double* Z, long long n);
	void distance_kernel_asm(const double* X1, const double* X2, const double* Y1, const double* Y2, double* Z, long long n);

#ifdef __cplusplus
}
#endif