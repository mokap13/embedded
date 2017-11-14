cmake_minimum_required(VERSION 3.8.1)

project(MODBUS_TASK)

add_library(modbus_task
    src/modbusTask.c
)

target_link_libraries(modbus_task
    modbus
)
target_include_directories(modbus_task
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)