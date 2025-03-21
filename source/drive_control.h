#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
#include "delay.h"
#include "led_button.h"
#include "interrupts.h"



#define BOARD_TPM_BASEADDR_MOTOR TPM2
#define BOARD_TPM_BASEADDR_SERVO TPM1
#define BOARD_TPM_CHANNEL_MOTOR0 0U
#define BOARD_TPM_CHANNEL_MOTOR1 1U
#define BOARD_TPM_CHANNEL_SERVO 0U

/*
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_INTERRUPT_HANDLER    TPM2_IRQHandler
*/

#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)


#ifndef TPM_MOTOR_ON_LEVEL
#define TPM_MOTOR_ON_LEVEL kTPM_HighTrue
#endif

#ifndef TPM_SERVO_ON_LEVEL
#define TPM_SERVO_ON_LEVEL kTPM_HighTrue
#endif

extern volatile uint8_t getCharValue;
extern volatile uint8_t angle;
extern float dutyCycle;



extern tpm_config_t tpmInfo_servo;
extern tpm_chnl_pwm_signal_param_t tpmParam_servo;

void motors_init(void);
void motor_set_check(void);
void servo_check(void);
void motor_set_speed(int8_t speed_level);

void steer_right(uint8_t pct);
void steer_left(uint8_t pct);
void steer_straight(void);

#define SERVO_R_MAX 4.5
#define SERVO_R3 5.0
#define SERVO_R2 6.0
#define SERVO_R1 7.0
#define SERVO_MIDDLE 7.9 //8.1
#define SERVO_L1 9.0
#define SERVO_L2 10.0
#define SERVO_L3 11.0
#define SERVO_L_MAX 11.7

#define SERVO_MAX 11.7
#define SERVO_MIN 5.9

#define MOTOR_MAX 9.9 //10% ==  2ms 5% = 1ms
#define MOTOR_MIN 5.8 //5.0

#define MAX_INIT 20
#define MIN_INIT -1

#endif /* DRIVE_CONTROL_H */
