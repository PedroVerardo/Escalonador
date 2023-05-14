cmake_minimum_required(VERSION 3.25)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled Queue.c Queue.h escalonador.c LinkedList.c p1.c p2.c p3.c bago.c)
