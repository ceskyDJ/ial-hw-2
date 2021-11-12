cmake_minimum_required(VERSION 3.20)
project(IAL\ |\ 2nd\ homework C)
set(CMAKE_SOURCE_DIR src/)
set(PROJECT_BINARY_DIR ../bin/)
set(CMAKE_BINARY_DIR ../bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -lm -fcommon")

add_executable(hashtable src/hashtable/hashtable.c src/hashtable/test.c src/hashtable/test_util.c)
add_executable(bree-iter src/btree/btree.c src/btree/test.c src/btree/test_util.c src/btree/iter/btree.c src/btree/iter/stack.c)
add_executable(bree-rec src/btree/btree.c src/btree/test.c src/btree/test_util.c src/btree/rec/btree.c)
