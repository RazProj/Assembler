cmake_minimum_required(VERSION 3.25)
project(Mmn14 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Mmn14
        Assembler.c
        pre_assembler.c
        pre_assembler.h
        syntax_pass.c
        syntax_pass.h
        syntax_pass.h
        error_checker.c
        error_checker.h
        creat_files.c
        general.c
        second_pass.c
        set_code.c
        set_tree.c
        tree.c
)
