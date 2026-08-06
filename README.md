# Euclidean Distance Kernel (C and x86-64 Assembly)

**Course:** *LBYARCH*

**Section:** *S25G*

**Members:**
- *Balila, Dale Vernard*
- *Calderon, John Gabriel*

Implementation of a Euclidean distance kernel in (1) C using SSE2 scalar-double intrinsics and (2) x86-64 assembly (MASM) using scalar SSE2 instructions directly. Both versions compute:

```
Z[i] = sqrt((X2[i] - X1[i])^2 + (Y2[i] - Y1[i])^2)
```

---

## Project Source Code Files

| File | Description |
|---|---|
| `main.c` | C main program — allocates and randomly initializes vectors, calls both kernels, times execution, checks correctness, prints results |
| `kernel.h` | Shared function declarations for both kernel versions |
| `kernel_c.c` | C kernel using SSE2 scalar-double intrinsics (`_mm_sub_sd`, `_mm_mul_sd`, `_mm_add_sd`, `_mm_sqrt_sd`) |
| `kernel_asm.asm` | x86-64 MASM assembly kernel using scalar SSE2 instructions directly (`movsd`, `subsd`, `mulsd`, `addsd`, `sqrtsd`) |
| `MP2_Balila_Calderon.slnx` / `.vcxproj` / `.vcxproj.filters` / `.vcxproj.user` | Visual Studio 2026 project files |

Both kernel versions use **functional scalar SIMD registers and scalar SIMD floating-point instructions**, per the assignment's hard requirement:

- **C version:** SSE2 intrinsics operating on the `_sd` ("scalar double") lane of XMM registers — this guarantees the compiler emits real scalar SSE2 instructions rather than legacy x87 code or auto-vectorized packed instructions.
- **Assembly version:** hand-written scalar SSE2 instructions (`movsd`, `subsd`, `mulsd`, `addsd`, `sqrtsd`) operating one double at a time on XMM registers, following the Windows x64 calling convention.

---

## Test Configuration

- **Vector sizes tested:** n = 2^20, 2^24, 2^27
- **Runs per size:** 30 (averaged)
- **Data type:** double-precision float, randomly initialized (0–100 range)
- **Correctness check:** C kernel output used as the answer key; ASM output compared element-wise against it (epsilon = 1e-6)

**Note on scaling down from 2^30:** the assignment allows reducing vector size if 2^30 is infeasible. At 2^30, six vectors (X1, X2, Y1, Y2, Z_c, Z_asm) at 8 bytes/double would require ~48GB of RAM — far beyond what a 16GB laptop can provide. Even 2^28 (~12.9GB total) is unsafe given typical free RAM availability (~8GB free out of 16GB total under normal usage). **2^27 (~6.4GB total) was chosen as the largest tier**, since it fits comfortably within available memory without triggering swap/paging, while still providing a meaningfully larger data point than 2^24.

| n | Doubles | Bytes/vector | Total (6 vectors) |
|---|---|---|---|
| 2^26 | 67,108,864 | 512 MB | ~3 GB |
| 2^27 | 134,217,728 | 1 GB | ~6 GB |
| 2^28 | 268,435,456 | 2 GB | ~12 GB |
| 2^29 | 536,870,912 | 4 GB | ~24 GB |
| 2^30 | 1,073,741,824 | 8 GB | ~48 GB |

---

## Comparative Execution Time

Average of 30 runs per size, Release build (`/O2`):

| n | C Kernel (avg) | ASM Kernel (avg) | Speedup (C/ASM) |
|---|---|---|---|
| 2^20 (1,048,576) | 0.002476 sec | 0.002546 sec | 0.972x |
| 2^24 (16,777,216) | 0.036163 sec | 0.037063 sec | 0.976x |
| 2^27 (134,217,728) | 0.316193 sec | 0.328660 sec | 0.962x |

---

## Analysis

At the two smaller sizes (2^20, 2^24), the C and assembly kernels perform essentially identically. This makes sense: MSVC's `/O2` optimizer does an excellent job translating the SSE2 intrinsics in `kernel_c.c` into efficient scalar SIMD instructions, leaving little room for hand-written assembly to outperform it. At these sizes, both kernels are also likely bound more by memory bandwidth (streaming four input arrays and writing one output array) than by raw instruction throughput, further narrowing any gap between the two.

At the largest size (2^27), a more interesting pattern emerged across repeated runs. The assembly kernel's timing stayed roughly consistent across all runs (~0.31–0.36 sec), while the C kernel's timing dropped sharply and consistently across consecutive runs of the program (~0.86 sec on the first run after a period of inactivity, settling to ~0.32 sec by the third and fourth run). This produced a large apparent speedup (~2.3–2.45x) on "cold" runs, shrinking to near parity (~0.96–1.16x) on "warm" runs.

This is most plausibly explained by **memory-subsystem effects rather than instruction-level performance differences**. At 2^27, each of the six allocated vectors is roughly 1GB, and a freshly-started process must fault in physical pages for newly allocated memory as it's first touched. Immediately after a period where the system's memory was under more pressure (e.g. right after another large run, or other background activity), the OS is more likely to need to reclaim or zero physical frames on demand — an expensive path. Once the system's page cache and physical frame allocator "settle," subsequent process runs pay less of this penalty. The C kernel appears to be disproportionately affected by this compared to the assembly kernel — a plausible explanation is that the intrinsics-based loop, even under `/O2`, retains a few more instructions per iteration (register shuffling the compiler didn't fully eliminate) than the hand-written assembly, making it slightly more exposed to memory-subsystem latency when the system hasn't yet stabilized.

**Conclusion:** for this workload, hand-written scalar SSE2 assembly does not provide a consistent, reproducible performance advantage over compiler-generated code from SSE2 intrinsics once the system reaches steady state. The one scenario where assembly showed a clear, repeated advantage (~2.3x+) was under cold-start memory conditions at the largest tested size — suggesting the benefit observed there is more attributable to the assembly kernel's smaller instruction footprint being less sensitive to memory-subsystem overhead, rather than to raw compute throughput.

---

## How to Build and Run

### Option 1 — Visual Studio
1. Open `MP2_Balila_Calderon.slnx`
2. Set configuration to **Release**, platform to **x64**
3. Build (Ctrl+Shift+B), then Run (Ctrl+F5)

### Option 2 — Command line
1. Initialize the Visual Studio environment
```cmd
"<insert the path to your vcvarsall.bat file>" x64
```
```bash
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```
> sample path above
2. Assemble the asm file
```cmd
ml64 /c /Fo kernel_asm.obj kernel_asm.asm
```
3. Compile the C files & link everything to `MP2.exe`
```cmd
cl /O2 /Fe:MP2.exe main.c kernel_c.c kernel_asm.obj
```
4. Run `MP2.exe`
```cmd
MP2.exe
```

---

## Demo Video
 
Both kernels are compiled and executed together in a single run of `MP2.exe`, so one video covers source code, compilation, and execution for both the C and x86-64 versions:
 
- Demo video: <insert link later>
