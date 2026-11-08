cmake_minimum_required(VERSION 3.25)
project(etvr_carrier_firmware C)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CPU_PARAMETERS -mthumb -mcpu=cortex-m0plus -mfloat-abi=soft)
set(TINYUSB ${CMAKE_CURRENT_SOURCE_DIR}/tinyusb)
set(MCU_SDK ${TINYUSB}/hw/mcu/microchip/samd11)

# Linker Script
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/samd11d14am.ld)
#set(LINKER_SCRIPT ${MCU_SDK}/gcc/gcc/samd11d14am_flash.ld) # Default Microchip Linker

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source Files

set(TINYUSB_SRC_FILES
	${TINYUSB}/src/tusb.c
	${TINYUSB}/src/common/tusb_fifo.c
	${TINYUSB}/src/device/usbd.c
	${TINYUSB}/src/class/cdc/cdc_device.c
	${TINYUSB}/src/portable/microchip/samd/dcd_samd.c
)

set(MCU_SDK_SRC_FILES
	#${MCU_SDK}/gcc/gcc/startup_samd11.c # Default Microchip startup script
	${MCU_SDK}/gcc/system_samd11.c
	${MCU_SDK}/hal/src/hal_atomic.c
	${MCU_SDK}/hpl/gclk/hpl_gclk.c
	${MCU_SDK}/hpl/pm/hpl_pm.c
	${MCU_SDK}/hpl/sysctrl/hpl_sysctrl.c
)

file(GLOB_RECURSE PROJECT_SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Executable

add_executable(${CMAKE_PROJECT_NAME}
	${MCU_SDK_SRC_FILES}
	${TINYUSB_SRC_FILES}
	${PROJECT_SRC_FILES}
)

# Header Files

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${TINYUSB}/src
	${TINYUSB}/hw
	${TINYUSB}/lib/CMSIS_5/CMSIS/Core/Include
	${MCU_SDK}
	${MCU_SDK}/config
	${MCU_SDK}/include
	${MCU_SDK}/hal/include
	${MCU_SDK}/hal/utils/include
	${MCU_SDK}/hpl/pm
	${MCU_SDK}/hpl/port
	${MCU_SDK}/hri
)

# Compiler

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
	__SAMD11D14AM__
	CONF_DFLL_OVERWRITE_CALIBRATION=0
	OSC32K_OVERWRITE_CALIBRATION=0
)

set(WARN_FLAGS
	-Wall
	-Wextra
	-Werror
	-Wfatal-errors
	-Wdouble-promotion
	-Wstrict-prototypes
	-Wstrict-overflow
	-Werror-implicit-function-declaration
	-Wfloat-equal
	-Wundef
	-Wshadow
	-Wwrite-strings
	-Wsign-compare
	-Wmissing-format-attribute
	-Wunreachable-code
	-Wcast-align
	-Wcast-function-type
	-Wcast-qual
	-Wnull-dereference
	-Wuninitialized
	-Wunused
	-Wunused-function
	-Wreturn-type
	-Wredundant-decls
	-Wmissing-prototypes
	#-Wconversion
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    ${CPU_PARAMETERS}
    -fdata-sections
    -ffunction-sections
    -funsigned-char
    -funsigned-bitfields
	-ffreestanding
	--specs=nano.specs
	-g0
    -Os
	-Wall
    -Wextra
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
	${CPU_PARAMETERS}
	-Wl,--script=${LINKER_SCRIPT}
	-Wl,--print-memory-usage
	-Wl,--gc-sections
	-nostartfiles
    --specs=nosys.specs
	--specs=nano.specs
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
	COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
)
