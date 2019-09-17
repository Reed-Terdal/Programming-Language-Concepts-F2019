# cmake_minimum_required(VERSION <specify CMake version here>)
project(Jott_Interpretter C)

set(CMAKE_C_STANDARD 11)

add_executable(Jott_Interpretter main.c Data_Structures/stack.c Data_Structures/stack.h Data_Structures/heap.c Data_Structures/heap.h Data_Structures/tree.c Data_Structures/tree.h Data_Structures/map.c Data_Structures/map.h Data_Structures/double_linked_list.c Data_Structures/double_linked_list.h Parser/parser.c Parser/parser.h Scanner/scanner.c Scanner/scanner.h)