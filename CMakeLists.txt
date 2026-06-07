cmake_minimum_required(VERSION 3.10)

project(LaboratorioEstru2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Agregamos todos los archivos .cpp al ejecutable
add_executable(LabEstru 
    main.cpp 
    Index.cpp 
    FileManager.cpp
)