#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
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

void MotorsInit(void);
void MotorSetRange(void);
void ServoCheckRange(void);
void MotorSetSpeed(int8_t speed_level);

void SteerRight(uint8_t pct);
void SteerLeft(uint8_t pct);
void SteerStraight(void);


#define SERVO_MIDDLE 7.9 //Prostredni hodnota serva po jizdu rovne
#define SERVO_MAX 10.2
#define SERVO_MIN 5.6

#define MOTOR1_MAX 9.9 //10% ==  2ms 5% = 1ms
#define MOTOR1_MIN  6.3//6.2 //5.0

#define MOTOR2_MAX 9.9 //10% ==  2ms 5% = 1ms
#define MOTOR2_MIN 5.9 //5.8 //5.0

#define MAX_INIT 20
#define MIN_INIT -1

#endif /* DRIVE_CONTROL_H */
