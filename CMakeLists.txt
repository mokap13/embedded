cmake_minimum_required(VERSION 3.8.1)

include(stm32.cmake)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_ERROR_DEPRECATED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(STM32_PROJECT LANGUAGES C ASM)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${STM32_StdPeriphLib_INCLUDE_DIRS}
)

set(STM32_FLASH_SIZE "128K")
set(STM32_RAM_SIZE "20K")
set(STM32_STACK_ADDRESS "0x20005000")

set(STM32_FLASH_ORIGIN "0x08000000")
set(STM32_RAM_ORIGIN "0x20000000")

# Compiler definitions for std. periph. library
# Select your chip type here
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER")
set(CMAKE_ASM_FLAGS  "${CMAKE_ASM_FLAGS}")

set(STARTUP_SOURCE ${STM32_STARTUP_MD})
set(FREERTOS_DIR c:/Embedded/FreeRTOS/FreeRTOSv9.0.0/FreeRTOS/Source)

set(MODBUS_DIR C:/Embedded/freemodbus-v1.5.0/modbus)
set(MODBUS_PORT_DIR ${CMAKE_CURRENT_LIST_DIR}/modbus_port)
add_subdirectory(${MODBUS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/modbus)

add_subdirectory(${FREERTOS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/freertos)
add_subdirectory(serial)
add_subdirectory(xprintf)
add_subdirectory(modbus_task)

# Uncomment needed modules
set(MOD_SOURCES
 ${STM32_MISC_SOURCE} 
#  ${STM32_ADC_SOURCE}
#  ${STM32_BKP_SOURCE}
#  ${STM32_CAN_SOURCE}
#  ${STM32_CEC_SOURCE}
#  ${STM32_CRC_SOURCE}
#  ${STM32_DAC_SOURCE}
#  ${STM32_DBGMCU_SOURCE}
#  ${STM32_DMA_SOURCE}
#  ${STM32_EXTI_SOURCE}
#  ${STM32_FLASH_SOURCE}
#  ${STM32_FSMC_SOURCE}
  ${STM32_GPIO_SOURCE}
#  ${STM32_I2C_SOURCE}
#  ${STM32_IWDG_SOURCE}
#  ${STM32_PWR_SOURCE}
  ${STM32_RCC_SOURCE}
#  ${STM32_RTC_SOURCE}
#  ${STM32_SDIO_SOURCE}
#  ${STM32_SPI_SOURCE}
  ${STM32_TIM_SOURCE}
  ${STM32_USART_SOURCE}
#  ${STM32_WWDG_SOURCE}
)

# Project sources
set(PROJECT_SOURCES
  main.c
)

# Default interrupts and system init
set(SERVICE_SOURCES
  stm32f10x_it.c
  ${STM32_SYSTEM_SOURCE}
)

# Configuring linker script for our target
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/stm32_flash.ld.in ${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.ld)
set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.ld ${CMAKE_EXE_LINKER_FLAGS}")


# Compile executable (*.elf)
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf 
  ${PROJECT_SOURCES} 
  ${MOD_SOURCES} 
  ${STARTUP_SOURCE} 
  ${SERVICE_SOURCES}
  FreeRTOS-openocd.c
  )

target_link_libraries(${CMAKE_PROJECT_NAME}.elf
  modbus_task
  xprintf
  serial  
  freertos
)

add_custom_command(
  TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} 
  ARGS ${CMAKE_PROJECT_NAME}.elf -Oihex ${CMAKE_PROJECT_NAME}.hex
  COMMENT "Create ${CMAKE_PROJECT_NAME}.hex")

add_custom_command(
  TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} 
  ARGS ${CMAKE_PROJECT_NAME}.elf -Obinary ${CMAKE_PROJECT_NAME}.bin
  COMMENT "Create ${CMAKE_PROJECT_NAME}.bin")

add_custom_command(
  TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD
  COMMAND arm-none-eabi-size 
  ARGS --format=berkeley "${CMAKE_PROJECT_NAME}.elf")