# Matrix
# Build
## Requirements
* CMake version 3.14 or later.
## Linux / macOS
```bash
cd /path/to/Matrix
```
* Build tests
```bash
mkdir build
cd build
cmake -G "Unix Makefiles" ../
cmake --build .
```
* Run tests
```bash
ctest
```
