cmake_minimum_required(VERSION 3.5)
project(
  assets_extract
  VERSION 1.0
  LANGUAGES CXX)

add_executable(assets_extract assets.cpp pngsearch.cpp pngsearch.h filesearch.cpp zstdsearch.h zstdsearch.cpp filesearch.h txtargs.cpp txtargs.h)
add_executable(assets_pack assets_pack.cpp)
