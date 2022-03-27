cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 17)

# Notre projet est étiqueté hello
project(raw-decoder)



include_directories(${CMAKE_SOURCE_DIR})

# Crée des variables avec les fichiers à compiler
set(SRCS
    main.cpp
    src/image.cpp
    )
    
set(HEADERS
    src/image.h
    )

add_compile_options(-g)
add_executable(raw-decoder ${SRCS} ${HEADERS})