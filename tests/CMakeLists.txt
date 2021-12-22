# Using doctest for tests

# the header for doctest already in directory (good idea jsp ?)
add_executable(${PROJECT_NAME}_tests
  test_main.cpp
  test_matrix.cpp
  test_ode.cpp)
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_calc)
