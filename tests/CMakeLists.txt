# Using doctest for tests
add_compile_definitions(SPARSE)

# the header for doctest already in directory (good idea jsp ?)
add_executable(${PROJECT_NAME}_tests
  test_main.cpp
  test_matrix.cpp
  test_ode.cpp
  test_heat.cpp)

target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_calc)
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_utils)
target_link_libraries(${PROJECT_NAME}_tests Eigen3::Eigen)
