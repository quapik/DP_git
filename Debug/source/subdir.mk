################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/dp_main.c \
../source/motory.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/dp_main.d \
./source/motory.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/dp_main.o \
./source/motory.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"D:\Skola\DP\workspace\DP_git\utilities" -I"D:\Skola\DP\workspace\DP_git\drivers" -I"D:\Skola\DP\workspace\DP_git\device" -I"D:\Skola\DP\workspace\DP_git\component\uart" -I"D:\Skola\DP\workspace\DP_git\component\lists" -I"D:\Skola\DP\workspace\DP_git\CMSIS" -I"D:\Skola\DP\workspace\DP_git\source" -I"D:\Skola\DP\workspace\DP_git\board" -I"D:\Skola\DP\workspace\DP_git\frdmkl27z\driver_examples\tpm\simple_pwm" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/dp_main.d ./source/dp_main.o ./source/motory.d ./source/motory.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

