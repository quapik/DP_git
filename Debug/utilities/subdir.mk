################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -I"D:\Skola\DP\workspace_reinstall\DP\utilities" -I"D:\Skola\DP\workspace_reinstall\DP\drivers" -I"D:\Skola\DP\workspace_reinstall\DP\device" -I"D:\Skola\DP\workspace_reinstall\DP\component\uart" -I"D:\Skola\DP\workspace_reinstall\DP\component\lists" -I"D:\Skola\DP\workspace_reinstall\DP\CMSIS" -I"D:\Skola\DP\workspace_reinstall\DP\component\timer" -I"D:\Skola\DP\workspace_reinstall\DP\source" -I"D:\Skola\DP\workspace_reinstall\DP\board" -I"D:\Skola\DP\workspace_reinstall\DP\frdmkl27z\driver_examples\tpm\simple_pwm" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o

.PHONY: clean-utilities

