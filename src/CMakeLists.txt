add_subdirectory(calc)
add_subdirectory(utils)
# subdirs(graphics)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_calc)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_utils)
