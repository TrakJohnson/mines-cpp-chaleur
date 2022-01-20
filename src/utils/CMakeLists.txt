file(GLOB UTILS_SOURCES "*.cpp")

add_library(${PROJECT_NAME}_utils ${UTILS_SOURCES})
# target_link_libraries(${PROJECT_NAME}_utils python3.9)
target_link_libraries(${PROJECT_NAME}_utils python3.9)
target_link_libraries(${PROJECT_NAME}_utils Python3::NumPy)
