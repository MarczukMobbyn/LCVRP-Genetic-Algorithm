cmake_minimum_required(VERSION 4.0)
project(projekt)

set(CMAKE_CXX_STANDARD 14)

include_directories(projekt)

add_executable(lcvrp_ga_v4
        projekt/CEvaluator.cpp
        projekt/CEvaluator.h
        projekt/CGeneticAlgorithm.cpp
        projekt/CGeneticAlgorithm.h
        projekt/CIndividual.cpp
        projekt/CIndividual.h
        projekt/CMySmartPointer.h
        projekt/Constants.h
        projekt/CRefCounter.h
        projekt/main.cpp)
