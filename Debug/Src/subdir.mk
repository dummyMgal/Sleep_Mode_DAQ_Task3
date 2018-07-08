################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/LSM303DLHC.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/rtc.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/usart.c 

OBJS += \
./Src/LSM303DLHC.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/rtc.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/LSM303DLHC.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/rtc.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F411xE -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Inc" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/main.o: ../Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F411xE -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Inc" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/CMSIS/Include"  -O0 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/rtc.o: ../Src/rtc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F411xE -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Inc" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/marko/embedded_programming/stm32workbench/Sleep_Mode_DAQ_Task3/Drivers/CMSIS/Include"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


