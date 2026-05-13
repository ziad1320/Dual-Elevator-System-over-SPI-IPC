cmake_minimum_required(VERSION 3.28)

# Force absolute path for the toolchain file BEFORE project()
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/ArmToolchain.cmake")

project(Dual_Elevator_System C ASM)

set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)

set(PROJECT_PATH                         ${CMAKE_CURRENT_SOURCE_DIR})
include(cmake/Device.cmake)
include(cmake/SourceList.cmake)
include(cmake/IncludeList.cmake)

set(LINKER_SCRIPTS_BASE_DIR              ${PROJECT_PATH}/STM32-base/linker/)
set(STARTUP_BASE_DIR                     ${PROJECT_PATH}/STM32-base/startup/)

# Prevent "multiple definition of main" by removing the entry points from the global list
list(REMOVE_ITEM SOURCE_LIST
        ${PROJECT_PATH}/src/master.c
        ${PROJECT_PATH}/src/slave.c
)

# ==============================================================================
# Target 1: Master Elevator
# ==============================================================================
set(MASTER_TARGET Master_Elevator)

add_executable(${MASTER_TARGET}
        ${PROJECT_PATH}/src/master.c
        ${SOURCE_LIST}
        ${STARTUP_BASE_DIR}/${SERIES_FOLDER}/${MAPPED_DEVICE}.s
)

target_include_directories(${MASTER_TARGET} PUBLIC ${INCLUDE_LIST})
target_compile_definitions(${MASTER_TARGET} PUBLIC ${COMPILER_DEFINE})
target_compile_options(${MASTER_TARGET} PUBLIC
        ${CPU_PARAMETERS}
        -Wall
        -Wpedantic
        -Wno-unused-parameter
)

target_link_options(${MASTER_TARGET} PRIVATE
        ${CPU_PARAMETERS}
        -L${LINKER_SCRIPTS_BASE_DIR}
        -T${LINKER_SCRIPTS_BASE_DIR}/${SERIES_FOLDER}/${DEVICE}.ld
        -nostartfiles
        -Xlinker --gc-sections
        -Wl,-Map=Master_Elevator.map
        --specs=nano.specs
        --specs=nosys.specs
)

# Explicitly link standard libraries and nosys to resolve the _init error
target_link_libraries(${MASTER_TARGET} PRIVATE c m nosys)

# Output size, hex, and bin for the Master target
add_custom_command(TARGET ${MASTER_TARGET} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${MASTER_TARGET}>
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${MASTER_TARGET}> Master_Elevator.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${MASTER_TARGET}> Master_Elevator.bin
)

# ==============================================================================
# Target 2: Slave Elevator
# ==============================================================================
set(SLAVE_TARGET Slave_Elevator)

add_executable(${SLAVE_TARGET}
        ${PROJECT_PATH}/src/slave.c
        ${SOURCE_LIST}
        ${STARTUP_BASE_DIR}/${SERIES_FOLDER}/${MAPPED_DEVICE}.s
)

target_include_directories(${SLAVE_TARGET} PUBLIC ${INCLUDE_LIST})
target_compile_definitions(${SLAVE_TARGET} PUBLIC ${COMPILER_DEFINE})
target_compile_options(${SLAVE_TARGET} PUBLIC
        ${CPU_PARAMETERS}
        -Wall
        -Wpedantic
        -Wno-unused-parameter
)

target_link_options(${SLAVE_TARGET} PRIVATE
        ${CPU_PARAMETERS}
        -L${LINKER_SCRIPTS_BASE_DIR}
        -T${LINKER_SCRIPTS_BASE_DIR}/${SERIES_FOLDER}/${DEVICE}.ld
        -nostartfiles
        -Xlinker --gc-sections
        -Wl,-Map=Slave_Elevator.map
        --specs=nano.specs
        --specs=nosys.specs
)

# Explicitly link standard libraries and nosys to resolve the _init error
target_link_libraries(${SLAVE_TARGET} PRIVATE c m nosys)

# Output size, hex, and bin for the Slave target
add_custom_command(TARGET ${SLAVE_TARGET} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${SLAVE_TARGET}>
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${SLAVE_TARGET}> Slave_Elevator.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${SLAVE_TARGET}> Slave_Elevator.bin
)