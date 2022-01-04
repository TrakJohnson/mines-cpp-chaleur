add_subdirectory(calc)
add_subdirectory(utils)
# subdirs(graphics)

# main
add_executable(${PROJECT_NAME} main.cpp)
# lib interne
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_calc)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_utils)
# lib externe
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
