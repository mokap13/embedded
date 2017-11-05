# # CodeSourcery toolchain directory
# SET(TOOLCHAIN_DIR /usr)
# # STM Library directory
# SET(STM32_StdPeriphLib_DIR /opt/STM32F10x_StdPeriph_Lib_V3.5.0)
set(TOOLCHAIN_DIR C:/GNUToolsARMEmbedded/ARM)
set(STM32_StdPeriphLib_DIR C:/Embedded/STM32F10x_StdPeriph_Lib_V3.5.0)

SET(STM32_StdPeriphLib_INCLUDE_DIRS 
    ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/inc/
    ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
    ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/CoreSupport/
)

SET(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_DIR}/bin)
SET(TOOLCHAIN_LIBC_DIR ${TOOLCHAIN_DIR}/arm-none-eabi/lib)
SET(TOOLCHAIN_INC_DIR ${TOOLCHAIN_LIBC_DIR}/include)
SET(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_LIBC_DIR}/usr/lib)

SET(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "system name")
SET(CMAKE_SYSTEM_PROCESSOR arm CACHE INTERNAL "processor")

SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-gcc.exe)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-g++.exe)
SET(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-as.exe)


SET(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-objcopy.exe)
SET(CMAKE_OBJDUMP ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-objdump.exe)

SET(CMAKE_C_FLAGS "-isystem ${TOOLCHAIN_INC_DIR} -fdiagnostics-color=always -mthumb -mcpu=cortex-m3 -fno-builtin -Wall -std=gnu99" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "-isystem ${TOOLCHAIN_INC_DIR} -fdiagnostics-color=always -mthumb -mcpu=cortex-m3 -fno-builtin -Wall " CACHE INTERNAL "cxx compiler flags")
SET(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m3" CACHE INTERNAL "asm compiler flags")

SET(CMAKE_C_FLAGS_DEBUG  "-O0 -g -gstabs")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -gstabs")
SET(CMAKE_ASM_FLAGS_DEBUG "-g -gstabs")

SET(CMAKE_C_FLAGS_RELEASE  "-Os")
SET(CMAKE_CXX_FLAGS_RELEASE  "-Os")
SET(CMAKE_ASM_FLAGS_RELEASE "")

SET(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs -nostartfiles -Wl,-Map -Wl,main.map -mthumb -mcpu=cortex-m3" CACHE INTERNAL "exe link flags")

SET(CMAKE_MODULE_LINKER_FLAGS "-L${TOOLCHAIN_LIB_DIR}" CACHE INTERNAL "module link flags")
SET(CMAKE_SHARED_LINKER_FLAGS "-L${TOOLCHAIN_LIB_DIR}" CACHE INTERNAL "shared lnk flags")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Startup sources
SET(STM32_STARTUP_CL ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_cl.s)
SET(STM32_STARTUP_HD ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd.s)
SET(STM32_STARTUP_HD_VL ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd_vl.s)
SET(STM32_STARTUP_LD ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_ld.s)
SET(STM32_STARTUP_LD_VL ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_ld_vl.s)
SET(STM32_STARTUP_MD ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.S)
SET(STM32_STARTUP_MD_VL ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md_vl.S)
SET(STM32_STARTUP_XL ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_xl.S)

# System source
SET(STM32_SYSTEM_SOURCE 
    ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
    ${STM32_StdPeriphLib_DIR}/Libraries/CMSIS/CM3/CoreSupport/core_cm3.c
)

SET(STM32_MISC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c)
SET(STM32_ADC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c)
SET(STM32_BKP_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c)
SET(STM32_CAN_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c)
SET(STM32_CEC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c)
SET(STM32_CRC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c)
SET(STM32_DAC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c)
SET(STM32_DBGMCU_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c)
SET(STM32_DMA_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c)
SET(STM32_EXTI_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c)
SET(STM32_FLASH_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c)
SET(STM32_FSMC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c)
SET(STM32_GPIO_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c)
SET(STM32_I2C_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c)
SET(STM32_IWDG_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c)
SET(STM32_PWR_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c)
SET(STM32_RCC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c)
SET(STM32_RTC_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c)
SET(STM32_SDIO_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c)
SET(STM32_SPI_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c)
SET(STM32_TIM_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c)
SET(STM32_USART_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c)
SET(STM32_WWDG_SOURCE ${STM32_StdPeriphLib_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c)

