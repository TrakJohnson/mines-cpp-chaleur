## References
# https://cliutils.gitlab.io/modern-cmake/
# https://github.com/ue12-p21/cpp-starter/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(mines-cpp-chaleur
	VERSION 1.0
	LANGUAGES CXX)

# compiler preferences (version, flags, etc.)
include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)

# Main project
include_directories(${PROJECT_SOURCE_DIR}/includes)

# subdir is deprecated !!
# subdirs(src)
add_subdirectory(src)

# Tests
# message("${BUILD_TESTING}")
# How is BUILD_TESTING configured ???
# if(${BUILD_TESTING})
# include(${PROJECT_SOURCE_DIR}/cmake/doctest.cmake)
subdirs(tests)
# endif(${BUILD_TESTING})
