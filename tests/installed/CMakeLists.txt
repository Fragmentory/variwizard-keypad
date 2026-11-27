# SPDX-FileCopyrightText: 2026 Roman Koch
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.22)
project(VariKeyHostConsumer LANGUAGES CXX)
find_package(VariKeyHost CONFIG REQUIRED)
add_executable(check-installed main.cpp)
target_link_libraries(check-installed PRIVATE VariKey::host)
