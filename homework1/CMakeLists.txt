cmake_minimum_required(VERSION 3.0.0)
project(exchange)

add_subdirectory(exchange_lib)

message("EXCHANGE_LIB_INCLUDE_DIRS = ${EXCHANGE_LIB_INCLUDE_DIRS}")
message("EXCHANGE_LIB_LIBRARIES = ${EXCHANGE_LIB_LIBRARIES}")

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

target_include_directories(${PROJECT_NAME} PUBLIC ${EXCHANGE_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXCHANGE_LIB_LIBRARIES})

add_subdirectory(tests)
