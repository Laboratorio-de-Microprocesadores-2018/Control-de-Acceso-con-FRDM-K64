################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/CardReader.c \
../source/CircularBuffer.c \
../source/Display.c \
../source/GPIO.c \
../source/Keyboard.c \
../source/Multiplexer.c \
../source/SysTick.c 

OBJS += \
./source/App.o \
./source/CardReader.o \
./source/CircularBuffer.o \
./source/Display.o \
./source/GPIO.o \
./source/Keyboard.o \
./source/Multiplexer.o \
./source/SysTick.o 

C_DEPS += \
./source/App.d \
./source/CardReader.d \
./source/CircularBuffer.d \
./source/Display.d \
./source/GPIO.d \
./source/Keyboard.d \
./source/Multiplexer.d \
./source/SysTick.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


