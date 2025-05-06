# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(sensor_tests C)

# 1) Enable testing
include(CTest)
enable_testing()

# 2) Turn off those annoying conversion warnings for tests
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-conversion")

# 3) Use pkg-config to find Criterion
find_package(PkgConfig REQUIRED)
pkg_check_modules(CRITERION REQUIRED criterion)

# 4) Gather your test sources
file(GLOB TEST_SOURCES "*.c")

# 5) Point at the real sensor implementation
set(SENSOR_SOURCE
  "${CMAKE_SOURCE_DIR}/src/Sensor/sensor.c"
)

# 6) Build the test runner, compiling both tests and sensor.c
add_executable(unit_tests
  ${TEST_SOURCES}
  ${SENSOR_SOURCE}
)

# 7) Make headers visible
target_include_directories(unit_tests PRIVATE
  "${CMAKE_SOURCE_DIR}/src/Sensor"
  ${CRITERION_INCLUDE_DIRS}
)

# 8) Link in the Criterion libraries via pkg-config
target_link_libraries(unit_tests PRIVATE
  ${CRITERION_LIBRARIES}
)

# 9) Ensure the linker knows where to find them at runtime
target_link_directories(unit_tests PRIVATE
  ${CRITERION_LIBRARY_DIRS}
)

# 10) Register with CTest
add_test(
  NAME sensor_unit_tests
  COMMAND unit_tests
)
