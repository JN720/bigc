cmake_minimum_required(VERSION 3.22)
project(Lemurr-Engine LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 
set(CMAKE_CXX_STANDARD 17)

set(CPU OFF CACHE BOOL "Build CPU Target")
set(GPU OFF CACHE BOOL "Build GPU Target")
set(WEB OFF CACHE BOOL "Build Web Target")


if(CPU)
    file(GLOB SOURCES "src/*.cpp" "src/Libraries/*.cpp" "src/Libraries/*/*.cpp")
    add_executable(BigC ${SOURCES})
    target_compile_definitions(BigC PRIVATE CPU=1)
endif()

if(GPU)
    file(GLOB SOURCES "src/*.cpp" "src/Libraries/*.cpp" "src/Libraries/*/*.cpp" "src/*.cu" "src/Libraries/*.cu" "src/Libraries/*/*.cu")
    find_package(CUDA REQUIRED COMPONENTS cudart)
    set(CMAKE_CUDA_STANDARD 17)
    add_executable(BigC_GPU ${SOURCES})
    target_compile_definitions(BigC_GPU PRIVATE GPU=1)
endif()

if(WEB)
    find_program(EMCC emcc)
    file(GLOB SOURCES "src/*.cpp" "src/Libraries/*.cpp" "src/Libraries/*/*.cpp")
    set(CMAKE_CXX_COMPILER emcc)
    add_executable(BigC_Web ${SOURCES})
    target_compile_definitions(BigC_Web PRIVATE WEB=1)
endif()
