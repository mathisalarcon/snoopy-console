cmake_minimum_required(VERSION 3.26)
project(snoopy C)

set(CMAKE_C_STANDARD 11)

add_executable(snoopy main.c
        src/tools/tools.h
        src/tools/tools.c
        src/menu/menu.c
        src/niveau/niveau.c
        src/partie/partie.c
        src/chrono/chrono.c
        src/sauvegardes/sauvegardes.c
        src/deplacements/commandes.h
        src/deplacements/commandes.c
        src/deplacements/deplacements.c
)
