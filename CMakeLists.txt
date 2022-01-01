cmake_minimum_required(VERSION 3.18)
project(Sem_Praca_UDSP C)

set(CMAKE_C_STANDARD 99)

add_executable(Sem_Praca_UDSP main.c table.h table.c manager.h manager.c client.h client.c server.h server.c)
