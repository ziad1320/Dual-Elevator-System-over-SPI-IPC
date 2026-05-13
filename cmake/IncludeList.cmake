set(INCLUDE_LIST ${INCLUDE_LIST}
        ${ARM_DIR}/arm-none-eabi/include
        ${PROJECT_PATH}/STM32-base/startup
        ${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/ARM/inc
        ${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/${SERIES_FOLDER}/inc
        ${PROJECT_PATH}/include

        # MCAL Layer Include Paths
        ${PROJECT_PATH}/Mcal/Lib
        ${PROJECT_PATH}/Mcal/Rcc
        ${PROJECT_PATH}/Mcal/Gpio
        ${PROJECT_PATH}/Mcal/Nvic
        ${PROJECT_PATH}/Mcal/Exti
        ${PROJECT_PATH}/Mcal/DMA
        ${PROJECT_PATH}/Mcal/Spi
        ${PROJECT_PATH}/Mcal/Timer
        ${PROJECT_PATH}/Mcal/Usart

        # HAL Layer Include Paths (Add your specific hardware modules here)
        # ${PROJECT_PATH}/Hal/Motor
        # ${PROJECT_PATH}/Hal/Button

        # APP Layer Include Paths (Add application state machine logic folders here)
        # ${PROJECT_PATH}/App
)

if (USE_HAL)
    set(INCLUDE_LIST ${INCLUDE_LIST} ${PROJECT_PATH}/STM32-base-STM32Cube/HAL/${SERIES_FOLDER}/inc)
endif ()