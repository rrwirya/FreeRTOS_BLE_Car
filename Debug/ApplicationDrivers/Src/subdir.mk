################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ApplicationDrivers/Src/adxl343.c \
../ApplicationDrivers/Src/adxl343_io.c \
../ApplicationDrivers/Src/motordriver.c \
../ApplicationDrivers/Src/motordriver_io.c 

OBJS += \
./ApplicationDrivers/Src/adxl343.o \
./ApplicationDrivers/Src/adxl343_io.o \
./ApplicationDrivers/Src/motordriver.o \
./ApplicationDrivers/Src/motordriver_io.o 

C_DEPS += \
./ApplicationDrivers/Src/adxl343.d \
./ApplicationDrivers/Src/adxl343_io.d \
./ApplicationDrivers/Src/motordriver.d \
./ApplicationDrivers/Src/motordriver_io.d 


# Each subdirectory must supply rules for building sources it contributes
ApplicationDrivers/Src/adxl343.o: ../ApplicationDrivers/Src/adxl343.c ApplicationDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../BlueNRG-2/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/BlueNRG-2/hci/hci_tl_patterns/Basic -I../Middlewares/ST/BlueNRG-2/utils -I../Middlewares/ST/BlueNRG-2/includes -I"D:/Reggie/Projects/FreeRTOS-BLE-Car/F411RE_Car_FW/ApplicationDrivers/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ApplicationDrivers/Src/adxl343.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
ApplicationDrivers/Src/adxl343_io.o: ../ApplicationDrivers/Src/adxl343_io.c ApplicationDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../BlueNRG-2/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/BlueNRG-2/hci/hci_tl_patterns/Basic -I../Middlewares/ST/BlueNRG-2/utils -I../Middlewares/ST/BlueNRG-2/includes -I"D:/Reggie/Projects/FreeRTOS-BLE-Car/F411RE_Car_FW/ApplicationDrivers/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ApplicationDrivers/Src/adxl343_io.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
ApplicationDrivers/Src/motordriver.o: ../ApplicationDrivers/Src/motordriver.c ApplicationDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../BlueNRG-2/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/BlueNRG-2/hci/hci_tl_patterns/Basic -I../Middlewares/ST/BlueNRG-2/utils -I../Middlewares/ST/BlueNRG-2/includes -I"D:/Reggie/Projects/FreeRTOS-BLE-Car/F411RE_Car_FW/ApplicationDrivers/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ApplicationDrivers/Src/motordriver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
ApplicationDrivers/Src/motordriver_io.o: ../ApplicationDrivers/Src/motordriver_io.c ApplicationDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../BlueNRG-2/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/BlueNRG-2/hci/hci_tl_patterns/Basic -I../Middlewares/ST/BlueNRG-2/utils -I../Middlewares/ST/BlueNRG-2/includes -I"D:/Reggie/Projects/FreeRTOS-BLE-Car/F411RE_Car_FW/ApplicationDrivers/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ApplicationDrivers/Src/motordriver_io.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

