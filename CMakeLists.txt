cmake_minimum_required(VERSION 3.28)
project(Calibration)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Calibration calibrate.cc
        include/DataFile.h
        src/DataFile.cc
        include/windows_string.h
        include/CalibrationSolverBase.h
        include/AntennaSolve.h
        src/AntennaSolve.cc
        include/CalibrationSolverBase.h
)
