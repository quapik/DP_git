#include "drive_control.h"

volatile uint8_t getCharValue = 0U;
volatile uint8_t angle = 0U; //Uhel natoceni serva - rozsah 0 - 180 stupnu
float dutyCycle = 0.0;

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
	//PRINTF("CLK %d\r\n",TPM_SOURCE_CLOCK);

	//SERVO  - defaultne nastaveno na middle hodnotu
	tpmParam_servo.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO;
	tpmParam_servo.level            = TPM_SERVO_ON_LEVEL;
	tpmParam_servo.dutyCyclePercent = SERVO_MIDDLE;

	//MOTOR - defaultne se nastavi max (pak se to predela pro jistotu)
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR;
	tpmParam.level            = TPM_MOTOR_ON_LEVEL;



    CLOCK_SetTpmClock(1U);

    //Nastaveni PWM pro motory
    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = kTPM_Prescale_Divide_32;
    TPM_Init(BOARD_TPM_BASEADDR_MOTOR, &tpmInfo);
    TPM_SetupPwm(BOARD_TPM_BASEADDR_MOTOR, &tpmParam, 1U, kTPM_EdgeAlignedPwm, 50U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR_MOTOR, kTPM_SystemClock);

    //Nastaveni PWM pro srvo
    TPM_GetDefaultConfig(&tpmInfo_servo);
    tpmInfo_servo.prescale = kTPM_Prescale_Divide_32;
    TPM_Init(BOARD_TPM_BASEADDR_SERVO, &tpmInfo_servo);
    TPM_SetupPwm(BOARD_TPM_BASEADDR_SERVO, &tpmParam_servo, 1U, kTPM_EdgeAlignedPwm, 50U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR_SERVO, kTPM_SystemClock);



    motor_set_check();
    servo_check();

}


void motor_set_check(void)
{
    // INICIALIZACE MOTORU

   	motor_set_speed(100);
   	SysTick_DelayTicks(2000U*1000);
   	motor_set_speed(0);
   	SysTick_DelayTicks(1000U*1000);
   	PRINTF("SET MOTOR DONE\r\n");

}

//Funkce co projde rozsah serva - stred, max vpravo, stred, max v vlevo, stred
//Óbcas se krajni hodnoty zasekly, proto potreba vyzkouset pred spustenim na drahu
void servo_check(void)
{
	steer_straight();
	SysTick_DelayTicks(200U*1000);
	for(uint8_t i = 1; i < 11; i++)
	{
		steer_right(i*10);
		SysTick_DelayTicks(100U*1000);
	}
	SysTick_DelayTicks(500U*1000);
	steer_straight();
	SysTick_DelayTicks(500U*1000);
	for(uint8_t i = 1; i < 11; i++)
	{
		steer_left(i*10);
		SysTick_DelayTicks(100U*1000);
	}
	SysTick_DelayTicks(500U*1000);
	steer_straight();
	SysTick_DelayTicks(200U*1000);
	PRINTF("SET SERVO DONE\r\n");

}

//Funkce pro nastaveni stredni hodnoty serva pro jizdvu vpred
void steer_straight(void)
{
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, SERVO_MIDDLE);

}

//Funkce co nastavuje procenta z maximalniho uhlu (0 stred, 100 maximalni zatoceni vlevo)
void steer_left(uint8_t pct)
{
	float set_steer;
	if(pct < 1) set_steer = SERVO_MIDDLE;
	else if(pct > 100) set_steer = SERVO_L_MAX;
			else set_steer = SERVO_MIDDLE + pct*0.036;
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, set_steer);
}
//Funkce co nastavuje procenta z maximalniho uhlu (0 stred, 100 maximalni zatoceni vpravo))
void steer_right(uint8_t pct)
{
	float set_steer;
	if(pct < 1) set_steer = SERVO_MIDDLE;
	else if(pct > 100) set_steer = SERVO_R_MAX;
			else set_steer = SERVO_MIDDLE - pct*0.036;
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, set_steer);
}

void motor_set_speed(int8_t pct)
{	float speed = MOTOR_MIN + (MOTOR_MAX-MOTOR_MIN)* (pct*0.01);

	//MINIMALNI A MAXIMALNI HODNOTA (mela by byt rozdila od 0)
	if(pct == 0) speed = MOTOR_MIN;
	if(pct == 100) speed = MOTOR_MAX;

	PRINTF("%i\r\n", (int)speed);

	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR, kTPM_EdgeAlignedPwm, speed);

}










