cmake_minimum_required(VERSION 3.8.1)

project(XPRINTF_PRJ)

add_library(xprintf
    src/xprintf.c
)

target_include_directories(xprintf
    PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${STM32_StdPeriphLib_INCLUDE_DIRS}
)
target_compile_definitions(xprintf
    PRIVATE
    STM32F10X_MD
)