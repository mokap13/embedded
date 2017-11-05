cmake_minimum_required(VERSION 3.8.1)

project(SERIAL_PRJ)

add_library(serial
    src/serial.c)
target_link_libraries(serial
    freertos
)

target_include_directories(serial
    PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${STM32_StdPeriphLib_INCLUDE_DIRS}
)
target_compile_definitions(serial
    PRIVATE
    STM32F10X_MD
)