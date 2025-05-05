# Disable conversion warnings in test builds
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-conversion")

# Show detailed test output
list(APPEND CMAKE_CTEST_ARGUMENTS --verbose)

# Collect all test source files in this directory
file(GLOB TEST_SOURCES "*.c")

# Define test executable
add_executable(unit_tests ${TEST_SOURCES})

# Add include paths for Server headers
target_include_directories(unit_tests
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Server
)

# Link against Criterion and your actual application target (Server)
target_link_libraries(unit_tests
    PRIVATE ${CRITERION}
    PRIVATE Server
)

# Register with CTest
add_test(
    NAME unit_tests
    COMMAND unit_tests ${CRITERION_FLAGS}
)
