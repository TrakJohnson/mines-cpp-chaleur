## Références
# https://cliutils.gitlab.io/modern-cmake/
# https://github.com/ue12-p21/cpp-starter/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # compile_commands.json pour clangd
project(mines-cpp-chaleur
	VERSION 1.0
	LANGUAGES CXX)
      
# compiler preferences (version, flags, etc.)
include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)

# to use gdb
# set(CMAKE_BUILD_TYPE Debug)

# Libraries externes
find_package(Eigen3 3.4 REQUIRED NO_MODULE)

if (TARGET Eigen3::Eigen)
  message(STATUS )
else()
  message(STATUS "HELP")
endif (TARGET Eigen3::Eigen)

include_directories(${PROJECT_SOURCE_DIR}/includes)
add_subdirectory(src)

# TODO build les tests que si l'utilisateur le demande
# if(${BUILD_TESTING})
# include(${PROJECT_SOURCE_DIR}/cmake/doctest.cmake)
subdirs(tests)
# endif(${BUILD_TESTING})


