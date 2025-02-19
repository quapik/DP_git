################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/delay.c \
../source/dp_main.c \
../source/drive_control.c \
../source/led_button.c \
../source/mtb.c \
../source/pixy.c \
../source/pixySPI.c \
../source/semihost_hardfault.c \
../source/sensors_control.c \
../source/sonic.c 

C_DEPS += \
./source/delay.d \
./source/dp_main.d \
./source/drive_control.d \
./source/led_button.d \
./source/mtb.d \
./source/pixy.d \
./source/pixySPI.d \
./source/semihost_hardfault.d \
./source/sensors_control.d \
./source/sonic.d 

OBJS += \
./source/delay.o \
./source/dp_main.o \
./source/drive_control.o \
./source/led_button.o \
./source/mtb.o \
./source/pixy.o \
./source/pixySPI.o \
./source/semihost_hardfault.o \
./source/sensors_control.o \
./source/sonic.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"D:\Skola\DP\workspace_reinstall\DP\utilities" -I"D:\Skola\DP\workspace_reinstall\DP\drivers" -I"D:\Skola\DP\workspace_reinstall\DP\device" -I"D:\Skola\DP\workspace_reinstall\DP\component\uart" -I"D:\Skola\DP\workspace_reinstall\DP\component\lists" -I"D:\Skola\DP\workspace_reinstall\DP\CMSIS" -I"D:\Skola\DP\workspace_reinstall\DP\source" -I"D:\Skola\DP\workspace_reinstall\DP\board" -I"D:\Skola\DP\workspace_reinstall\DP\frdmkl27z\driver_examples\tpm\simple_pwm" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/delay.d ./source/delay.o ./source/dp_main.d ./source/dp_main.o ./source/drive_control.d ./source/drive_control.o ./source/led_button.d ./source/led_button.o ./source/mtb.d ./source/mtb.o ./source/pixy.d ./source/pixy.o ./source/pixySPI.d ./source/pixySPI.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sensors_control.d ./source/sensors_control.o ./source/sonic.d ./source/sonic.o

.PHONY: clean-source

