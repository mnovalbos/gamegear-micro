cmake_minimum_required(VERSION 3.5)
project(
  elf_patcher
  VERSION 1.0
  LANGUAGES CXX)

add_executable(elf_patcher elf.cpp)
add_executable(elf_search patcherSearch.cpp)
