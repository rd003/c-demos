cmake_minimum_required(VERSION 3.15)
project(rest_apis C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MICROHTTPD REQUIRED IMPORTED_TARGET libmicrohttpd)

add_executable(rest_apis main.c)
target_link_libraries(rest_apis PRIVATE PkgConfig::MICROHTTPD)
