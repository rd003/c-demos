# tutorial

`1. vcpkg.json

```sh
cat > vcpkg.json << 'EOF'
{
  "dependencies": [
    "libmicrohttpd"
  ]
}
EOF
```

2. Create a basic CMakeLists.txt (if you don't have one):

```sh
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.15)
project(rest_apis C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MICROHTTPD REQUIRED IMPORTED_TARGET libmicrohttpd)

add_executable(rest_apis main.c)
target_link_libraries(rest_apis PRIVATE PkgConfig::MICROHTTPD)
EOF
```

2.1 Create main.c file

```sh
cat > main.c << 'EOF'
#include <microhttpd.h>
#include <stdio.h>

int main() {
    printf("libmicrohttpd installed successfully!\n");
    return 0;
}
EOF
```

3. Configure with CMake (this will automatically install libmicrohttpd via vcpkg).

```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

```
Note: Needed First time only (or when you change CMakeLists.txt or add dependencies

4. Build

`cmake --build build`

5. Run

 ./build/<project_name>

 eg: ./build/rest_apis
