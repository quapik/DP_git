################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc16.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_dma.c \
../drivers/fsl_dmamux.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lptmr.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_pit.c \
../drivers/fsl_smc.c \
../drivers/fsl_spi.c \
../drivers/fsl_spi_cmsis.c \
../drivers/fsl_spi_dma.c \
../drivers/fsl_tpm.c \
../drivers/fsl_uart.c 

C_DEPS += \
./drivers/fsl_adc16.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_dma.d \
./drivers/fsl_dmamux.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lptmr.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_pit.d \
./drivers/fsl_smc.d \
./drivers/fsl_spi.d \
./drivers/fsl_spi_cmsis.d \
./drivers/fsl_spi_dma.d \
./drivers/fsl_tpm.d \
./drivers/fsl_uart.d 

OBJS += \
./drivers/fsl_adc16.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_dma.o \
./drivers/fsl_dmamux.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lptmr.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_pit.o \
./drivers/fsl_smc.o \
./drivers/fsl_spi.o \
./drivers/fsl_spi_cmsis.o \
./drivers/fsl_spi_dma.o \
./drivers/fsl_tpm.o \
./drivers/fsl_uart.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -I"D:\Skola\DP\workspace_reinstall\DP\utilities" -I"D:\Skola\DP\workspace_reinstall\DP\drivers" -I"D:\Skola\DP\workspace_reinstall\DP\device" -I"D:\Skola\DP\workspace_reinstall\DP\component\uart" -I"D:\Skola\DP\workspace_reinstall\DP\component\lists" -I"D:\Skola\DP\workspace_reinstall\DP\CMSIS" -I"D:\Skola\DP\workspace_reinstall\DP\component\timer" -I"D:\Skola\DP\workspace_reinstall\DP\source" -I"D:\Skola\DP\workspace_reinstall\DP\board" -I"D:\Skola\DP\workspace_reinstall\DP\frdmkl27z\driver_examples\tpm\simple_pwm" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_adc16.d ./drivers/fsl_adc16.o ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_dma.d ./drivers/fsl_dma.o ./drivers/fsl_dmamux.d ./drivers/fsl_dmamux.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lptmr.d ./drivers/fsl_lptmr.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_pit.d ./drivers/fsl_pit.o ./drivers/fsl_smc.d ./drivers/fsl_smc.o ./drivers/fsl_spi.d ./drivers/fsl_spi.o ./drivers/fsl_spi_cmsis.d ./drivers/fsl_spi_cmsis.o ./drivers/fsl_spi_dma.d ./drivers/fsl_spi_dma.o ./drivers/fsl_tpm.d ./drivers/fsl_tpm.o ./drivers/fsl_uart.d ./drivers/fsl_uart.o

.PHONY: clean-drivers

