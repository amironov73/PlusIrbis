cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

if(IRBIS_MINGW)
    add_definitions(-DIRBIS_MINGW)
endif(IRBIS_MINGW)

project(PlusIrbis)

# where artifacts will be gathered
set(ARTIFACTS ${PROJECT_SOURCE_DIR}/artifacts)

# headers of third-party libraries
include_directories(${PROJECT_SOURCE_DIR}/third)

# subdirectories
add_subdirectory(libs)
add_subdirectory(tests)
add_subdirectory(apps)
