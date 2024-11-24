#include "drive_control.h"

#define BOARD_TPM_BASEADDR_MOTOR TPM2
#define BOARD_TPM_BASEADDR_SERVO TPM2
#define BOARD_TPM_CHANNEL_MOTOR 0U
#define BOARD_TPM_CHANNEL_SERVO 1U


/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_INTERRUPT_HANDLER    TPM2_IRQHandler


/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)

volatile uint8_t getCharValue = 0U;
volatile uint8_t angle = 0U; //Uhel natoceni serva - rozsah 0 - 180 stupnu
float dutyCycle = 0.0;


tpm_config_t tpmInfo;
tpm_chnl_pwm_signal_param_t tpmParam;

tpm_config_t tpmInfo_servo;
tpm_chnl_pwm_signal_param_t tpmParam_servo;

#ifndef TPM_MOTOR_ON_LEVEL
#define TPM_MOTOR_ON_LEVEL kTPM_HighTrue
#endif

#ifndef TPM_SERVO_ON_LEVEL
#define TPM_SERVO_ON_LEVEL kTPM_HighTrue
#endif

#ifndef TPM_MOTOR_ON_LEVEL
#define TPM_MOTOR_ON_LEVEL kTPM_HighTrue
#endif

#ifndef TPM_SERVO_ON_LEVEL
#define TPM_SERVO_ON_LEVEL kTPM_HighTrue
#endif

tpm_config_t tpmInfo;
tpm_chnl_pwm_signal_param_t tpmParam;

tpm_config_t tpmInfo_servo;
tpm_chnl_pwm_signal_param_t tpmParam_servo;


void motors_init(void)
{

	//SERVO  - defaultne nastaveno na middle hodnotu
	tpmParam_servo.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO;
	tpmParam_servo.level            = TPM_SERVO_ON_LEVEL;
	tpmParam_servo.dutyCyclePercent = SERVO_MIDDLE;

	//MOTOR - defaultne se nastavi max (pak se to predela pro jistotu)
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR;
	tpmParam.level            = TPM_MOTOR_ON_LEVEL;
	tpmParam.dutyCyclePercent = MAX_INIT;


    CLOCK_SetTpmClock(1U);

    /* Initialize TPM module */
    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = kTPM_Prescale_Divide_32;
    TPM_Init(BOARD_TPM_BASEADDR_MOTOR, &tpmInfo);
    TPM_SetupPwm(BOARD_TPM_BASEADDR_MOTOR, &tpmParam, 1U, kTPM_EdgeAlignedPwm, 50U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR_MOTOR, kTPM_SystemClock);

    TPM_GetDefaultConfig(&tpmInfo_servo);
    tpmInfo_servo.prescale = kTPM_Prescale_Divide_32;
    TPM_Init(BOARD_TPM_BASEADDR_SERVO, &tpmInfo_servo);
    TPM_SetupPwm(BOARD_TPM_BASEADDR_SERVO, &tpmParam_servo, 1U, kTPM_EdgeAlignedPwm, 50U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR_SERVO, kTPM_SystemClock);

    // INICIALIZACE MOTORU
   	PRINTF("SET MAX\r\n");
   	motor_set_speed(MAX_INIT);
   	SysTick_DelayTicks(2000U);
   	PRINTF("SET MIN\r\n");
   	motor_set_speed(MIN_INIT);
   	SysTick_DelayTicks(1000U);

   	//NASTAVENI RIDICIHO SERVA DO MIDLE POLOHY
   	steer_straight();

}

void steer_straight(void)
{
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, SERVO_MIDDLE);
}

void steer_left(int range)
{
	float set_steer;
	if(range < 1) set_steer = SERVO_MIDDLE;
	if(range == 1) set_steer = SERVO_L1;
	if(range == 2) set_steer = SERVO_L2;
	if(range == 3) set_steer = SERVO_L3;
	if(range > 3) set_steer = SERVO_L3;
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, set_steer);
}

void steer_right(int range)
{
	float set_steer;
	if(range < 1) set_steer = SERVO_MIDDLE;
	if(range == 1) set_steer = SERVO_R1;
	if(range == 2) set_steer = SERVO_R2;
	if(range == 3) set_steer = SERVO_R3;
	if(range > 3) set_steer = SERVO_R3;
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, set_steer);
}

void motor_set_speed(int8_t speed_level)
{	float speed = 0.0;
	if(speed_level == 0 || speed_level > 6) speed = 5.0;
	else speed = 6.0 + speed_level;

	//MINIMALNI A MAXIMALNI HODNOTA (mela by byt rozdila od 0)
	if(speed_level == MIN_INIT) speed = 5.9;
	if(speed_level == MAX_INIT) speed = 13.0;

	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR, kTPM_EdgeAlignedPwm, speed);

}











