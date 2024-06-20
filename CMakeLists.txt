cmake_minimum_required(VERSION 3.22)

project(arithmatoy)

# Add the arithmatoy library
add_library(
  arithmatoy
  src/arithmatoy.c
)

# Include directories for the arithmatoy library
target_include_directories(
  arithmatoy
  PUBLIC src/include/
  PRIVATE src/
)

# Set properties for the arithmatoy library
set_target_properties(
  arithmatoy
  PROPERTIES
  PUBLIC_HEADER src/include/arithmatoy.h
)

# Add the arithmatoy-cli executable
add_executable(
  arithmatoy-cli
  src/cli.c
)

# Include directories and link libraries for arithmatoy-cli
target_include_directories(
  arithmatoy-cli
  PRIVATE src/include/ src/
)
target_link_libraries(arithmatoy-cli PRIVATE arithmatoy)

# Install targets
install(
  TARGETS arithmatoy arithmatoy-cli
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)

# Enable testing if this is the main project and BUILD_TESTING is enabled
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
  enable_testing()
  find_package(CMocka REQUIRED)

  # Add tests
  foreach(test_file
    test_utils
    test_add
    test_sub
    test_mul
  )
    add_executable(${test_file} src/tests/${test_file}.c)
    target_link_libraries(${test_file} PRIVATE arithmatoy cmocka)
    target_include_directories(${test_file} PRIVATE src/)

    add_test(NAME ${test_file} COMMAND ${test_file})
  endforeach()
endif()
