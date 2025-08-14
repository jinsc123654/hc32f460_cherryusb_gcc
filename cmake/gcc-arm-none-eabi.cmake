# Toolchain for HC32F460 using arm-none-eabi GCC
set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  arm)

set(CMAKE_C_COMPILER_ID     GNU)
set(CMAKE_CXX_COMPILER_ID   GNU)

# -----------------------------
# Toolchain directory
# 修改这里即可切换整个工具链
# -----------------------------
set(TOOLCHAIN_DIR /home/jinsc/SDK/gcc_arm/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux/gcc-arm-none-eabi-9-2020-q2-update/bin)

# Compiler and tools
set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/arm-none-eabi-g++)
set(CMAKE_LINKER       ${TOOLCHAIN_DIR}/arm-none-eabi-g++)
set(CMAKE_OBJCOPY      ${TOOLCHAIN_DIR}/arm-none-eabi-objcopy)
set(CMAKE_SIZE         ${TOOLCHAIN_DIR}/arm-none-eabi-size)

# Executable suffix
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# -----------------------------
# MCU specific flags
# -----------------------------
set(TARGET_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS_DEBUG   "-O2 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG   "-O2 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

# -----------------------------
# Linker flags
# -----------------------------
set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/linker/HC32F460xE.ld\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")
