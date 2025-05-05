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
target_include_directories(server PRIVATE Server)
target_link_libraries(server PRIVATE SQLite::SQLite3 microhttpd)

target_link_libraries(database_create PRIVATE SQLite::SQLite3)
target_link_libraries(database_insert PRIVATE SQLite::SQLite3)

add_executable(main main.c)

target_link_libraries(main
    PRIVATE
      sensor
      database_create
      database_insert
      server
)
