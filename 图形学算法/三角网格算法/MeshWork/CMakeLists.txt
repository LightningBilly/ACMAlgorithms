cmake_minimum_required(VERSION 3.15)
project(MeshWork)

set(CMAKE_CXX_STANDARD 14)

add_executable(MeshWork
        PolyMesh/PolyMesh.cpp PolyMesh/PolyMeshIterators.cpp PolyMesh/PolyMesh_Base.cpp PolyMesh/IOManger.cpp
        src/hw1/Algorithm.cpp
        src/hw1/hw1.cpp
        main.cpp src/hw2/hw2.h src/hw2/hw2.cpp src/hw3/hw3.h src/hw3/hw3.cpp)
