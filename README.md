## Comparative Execution Time and Performance Analysis

The C and x86-64 assembly kernels were tested using vector sizes of \(2^{20}\), \(2^{24}\), and \(2^{27}\). Each kernel was executed 30 times for each vector size, and the average execution time was recorded.

Both kernels calculate the Euclidean distance between corresponding points:

\[
Z[i] = \sqrt{(X2[i]-X1[i])^2 + (Y2[i]-Y1[i])^2}
\]

The C implementation serves as the reference implementation, while the x86-64 assembly implementation uses scalar single-precision floating-point instructions and XMM registers.

### Execution Time Results

| Vector Size | Elements | C Average Time (s) | x86-64 ASM Average Time (s) | C / ASM |
|---|---:|---:|---:|---:|
| \(2^{20}\) | 1,048,576 | 0.001541 | 0.001585 | 0.973× |
| \(2^{24}\) | 16,777,216 | 0.023629 | 0.024276 | 0.973× |
| \(2^{27}\) | 134,217,728 | 0.189477 | 0.189741 | 0.999× |


### Performance Analysis

Unlike an unoptimized/debug build, the Release build shows that the C implementation performs approximately the same as, and in these tests slightly better than, the x86-64 assembly implementation.

For 2^20
, the C kernel took an average of 0.001541 seconds, while the assembly kernel took 0.001585 seconds. The resulting C/ASM ratio was 0.973×, meaning the C implementation was approximately 2.8% faster.

For 2^24
, the C kernel averaged 0.023629 seconds, compared with 0.024276 seconds for assembly. This produced a ratio of 0.973×, again showing that C was approximately 2.7% faster.

For the largest test size, 2^27
, the difference became almost negligible. The C kernel averaged 0.189477 seconds, while the assembly kernel averaged 0.189741 seconds, producing a ratio of 0.999×. The two implementations therefore had nearly identical performance.

These results show that manually writing the kernel in x86-64 assembly does not necessarily guarantee better performance than C. In a Release build, the C compiler can perform optimizations and generate highly efficient machine code. As a result, the compiler-generated implementation can be competitive with assembly.

The difference between the C and assembly implementations also becomes less significant as the vector size increases. For 2^27
, the execution times are nearly the same, with only a small difference between the two kernels.

### Correctness

All three test sizes passed the correctness check:

```text
2^20: PASSED
2^24: PASSED
2^27: PASSED
```

## Screenshot of the program output with the correctness check of both C and ASM

<img width="1757" height="601" alt="output_c-and-asm" src="https://github.com/user-attachments/assets/4b3244f7-5462-4ad5-9b60-8087e9c6f85e" />

## Video Link

YouTube Link: https://www.youtube.com/watch?v=994Zh0Q1ye4
