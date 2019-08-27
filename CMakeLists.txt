cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
set(THREADS_PREFER_PTHREAD_FLAG ON)

project(CollisionSim)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(CollisionSim src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp src/particle.h src/mathtools.h src/mathtools.cpp src/particle.cpp src/physic.h src/physic.cpp src/simulationObject.h src/particlePhysics2D.h src/particlePhysics2D.cpp src/synchronizedList.h)
target_link_libraries(CollisionSim Threads::Threads ${SDL2_LIBRARIES})
