# LCVRP Genetic Algorithm Solver

A robust C++14 application that implements a Genetic Algorithm (GA) to solve the Line-haul Capacitated Vehicle Routing Problem (LCVRP). The project demonstrates advanced object-oriented design, custom memory management, and metaheuristic optimization techniques.

## Table of Contents
- [Overview](#overview)
- [Architecture](#architecture)
- [Key Features](#key-features)
- [Building](#building)
- [Execution](#execution)

## Overview
This project evaluates and optimizes delivery routes for a fleet of vehicles with capacity constraints. The solution represents candidates as integer arrays mapping customers to specific routes. The algorithm utilizes standard genetic operators alongside an experimental "Turbo Mutation" mechanism to explore the solution space.

## Architecture
- **CEvaluator**: Parses `.lcvrp` instance files, calculates Euclidean distances between nodes, builds valid routes based on genotype permutations, and computes total fitness. Capacity violations are handled by forcing a return to the depot.
- **CGeneticAlgorithm**: Orchestrates the evolutionary loop. Manages a population of smart pointers to `CIndividual` objects. Implements tournament selection, single-point crossover, standard mutation, and turbo mutation.
- **CIndividual**: Represents a single candidate solution. Handles its own crossover, mutation, and turbo mutation logic.
- **CMySmartPointer & CRefCounter**: A custom, intrusive smart pointer template ensuring memory safety and leak-free dynamic allocation for population members, bypassing the standard library's memory header.

## Key Features
- **Custom Smart Pointers**: Deeply integrated reference counting (`CRefCounter`) that tracks all live instances across the application.
- **Turbo Mutation**: An experimental operator where a single individual draws genetic material from a randomly selected group of donor individuals.
- **Configurable GA Parameters**: Support for variable population size, crossover probability, mutation probability, and generation limits via predefined constants.

## Building
The project is configured using CMake (minimum version 4.0) and requires a C++14 compliant compiler. Note that the CMake configuration expects source files to be located within a `projekt` subdirectory.

1. Clone the repository.
2. Ensure the directory structure matches the CMake requirements.
3. Run the following build commands:

    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

## Execution
Run the compiled executable. By default, the application looks for the `A-n32-k5.lcvrp` instance file in the working directory.

```bash
./lcvrp_ga_v4
