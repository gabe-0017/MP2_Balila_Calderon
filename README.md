**1. Initialize the Visual Studio environment**
```cmd
"<insert the path to your vcvarsall.bat file>" x64
```
Sample:
```bash
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```
**2. Assemble the asm file**
```cmd
ml64 /c /Fo kernel_asm.obj kernel_asm.asm
```
**3. Compile the C files & link everything to `MP2.exe`**
```cmd
cl /O2 /Fe:MP2.exe main.c kernel_c.c kernel_asm.obj
```
**Run `MP2.exe`**
```cmd
MP2.exe
```
