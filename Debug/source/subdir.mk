################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/delay.c \
../source/dp_main.c \
../source/drive_control.c \
../source/globals.c \
../source/hall_sensors.c \
../source/interrupts.c \
../source/ir_sensors.c \
../source/led_button.c \
../source/mtb.c \
../source/pixySPI.c \
../source/semihost_hardfault.c \
../source/sonic_color.c \
../source/timers.c \
../source/uart_control.c 

C_DEPS += \
./source/delay.d \
./source/dp_main.d \
./source/drive_control.d \
./source/globals.d \
./source/hall_sensors.d \
./source/interrupts.d \
./source/ir_sensors.d \
./source/led_button.d \
./source/mtb.d \
./source/pixySPI.d \
./source/semihost_hardfault.d \
./source/sonic_color.d \
./source/timers.d \
./source/uart_control.d 

OBJS += \
./source/delay.o \
./source/dp_main.o \
./source/drive_control.o \
./source/globals.o \
./source/hall_sensors.o \
./source/interrupts.o \
./source/ir_sensors.o \
./source/led_button.o \
./source/mtb.o \
./source/pixySPI.o \
./source/semihost_hardfault.o \
./source/sonic_color.o \
./source/timers.o \
./source/uart_control.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -I"D:\Skola\DP\workspace_reinstall\DP\utilities" -I"D:\Skola\DP\workspace_reinstall\DP\drivers" -I"D:\Skola\DP\workspace_reinstall\DP\device" -I"D:\Skola\DP\workspace_reinstall\DP\component\uart" -I"D:\Skola\DP\workspace_reinstall\DP\component\lists" -I"D:\Skola\DP\workspace_reinstall\DP\CMSIS" -I"D:\Skola\DP\workspace_reinstall\DP\component\timer" -I"D:\Skola\DP\workspace_reinstall\DP\source" -I"D:\Skola\DP\workspace_reinstall\DP\board" -I"D:\Skola\DP\workspace_reinstall\DP\frdmkl27z\driver_examples\tpm\simple_pwm" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/delay.d ./source/delay.o ./source/dp_main.d ./source/dp_main.o ./source/drive_control.d ./source/drive_control.o ./source/globals.d ./source/globals.o ./source/hall_sensors.d ./source/hall_sensors.o ./source/interrupts.d ./source/interrupts.o ./source/ir_sensors.d ./source/ir_sensors.o ./source/led_button.d ./source/led_button.o ./source/mtb.d ./source/mtb.o ./source/pixySPI.d ./source/pixySPI.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sonic_color.d ./source/sonic_color.o ./source/timers.d ./source/timers.o ./source/uart_control.d ./source/uart_control.o

.PHONY: clean-source

