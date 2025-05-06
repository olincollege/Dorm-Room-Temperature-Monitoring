# If you have a program that's just a .c file and it has a main method, define
# an executable. If it has both a .c and .h file, make it a library. Also, link
# libraries appropriately here. Targets should be added in dependency order - if
# executable bar needs library foo, then you should do
#   add_library(foo foo.c foo.h)
#   add_executable(bar bar.c)
#   target_link_libraries(bar PRIVATE foo)
# or similar. The PRIVATE/INTERFACE/PUBLIC keyword will depend on whether the
# library is used only in function bodies (PRIVATE), only in function
# signatures/types (INTERFACE), or both (PUBLIC).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


find_package(SQLite3 REQUIRED)

# Add Libraries
add_library(sensor Sensor/sensor.c Sensor/sensor.h)
add_library(database_create database_create.c database_create.h)
add_library(database_insert database_insert.c database_insert.h)
add_library(server
    Server/api_handler.c
    Server/db_handle.c
    Server/http_server.c
    Server/sensor_feeder.c
    Server/static_handler.c
    Server/utils.c
)

#Link Libraries
target_link_libraries(server PRIVATE microhttpd SQLite::SQLite3)
target_link_libraries(sensor PRIVATE SQLite::SQLite3)
target_link_libraries(database_create PRIVATE SQLite::SQLite3)
target_link_libraries(database_insert PRIVATE SQLite::SQLite3)


# Executable 1: Sensor logger
add_executable(sensor_logger main.c)
target_link_libraries(sensor_logger
    PRIVATE sensor
    PRIVATE database_create
    PRIVATE database_insert
)

# Executable 2: HTTP server
add_executable(http_server Server/main.c)
target_link_libraries(http_server PRIVATE server)

# Target to run the sensor logger
add_custom_target(run_logger
    COMMAND sensor_logger
    DEPENDS sensor_logger
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running Sensor Logger..."
)

# Target to run the HTTP server
add_custom_target(run_server
    COMMAND http_server
    DEPENDS http_server
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running HTTP Server..."
)

# Run both executables seqyentially
add_custom_target(run_all
    COMMAND ./sensor_logger &
    COMMAND ./http_server
    DEPENDS sensor_logger http_server
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running both Sensor Logger and HTTP Server together..."
)
