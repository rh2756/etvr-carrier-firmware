cmake_minimum_required(VERSION 3.25)
project(etvr_carrier_firmware)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain.
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CPU_PARAMETERS -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/samd11d14am_flash.ld)
# Default Microchip Linker:
#set(LINKER_SCRIPT ${TINYUSB}/hw/mcu/microchip/samd11/gcc/gcc/samd11d14am_flash.ld)
set(TINYUSB ${CMAKE_CURRENT_SOURCE_DIR}/tinyusb)
set(MCU_SDK ${TINYUSB}/hw/mcu/microchip/samd11)

set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

### Sources

file(GLOB TINYUSB_SRC_FILES
    ${TINYUSB}/src/tusb.c
    ${TINYUSB}/src/common/*.c
    ${TINYUSB}/src/device/*.c
    ${TINYUSB}/src/class/cdc/*.c
    ${TINYUSB}/src/portable/microchip/samd/dcd_samd.c
)

file(GLOB PROJECT_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Executable

add_executable(${CMAKE_PROJECT_NAME}
	${TINYUSB_SRC_FILES}
	${MCU_SDK}/gcc/system_samd11.c
	${MCU_SDK}/gcc/gcc/startup_samd11.c
    ${PROJECT_SRC_FILES}
)

# Headers

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	${TINYUSB}/src
	${TINYUSB}/lib/CMSIS_5/CMSIS/Core/Include
	${MCU_SDK}
	${MCU_SDK}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -fdata-sections
    -ffunction-sections
    -funsigned-char
    -funsigned-bitfields
	--specs=nano.specs
	-g0
    -Os
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
	${CPU_PARAMETERS}
	-Wl,--script=${LINKER_SCRIPT}
	-Wl,--gc-sections
	-Wl,--print-memory-usage
	--specs=nano.specs
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
	COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
)

#cmake -B build -G Ninja
#cd ./build/
#ninja

# TODO:
# Add C version C17
