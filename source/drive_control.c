/*
 * 	drive_control.c
 *
 * Soubor starající se o inicializace motoroů a serva  jejich ovládání
 *
 *
 *  Created on: 14. 1. 2025
 *  Author: Vojtěch Šíma
 *  Diplomová práce  Samořiditelný model autíčka pro NXP Cup
 *  2024/2025
 */

#include "drive_control.h"

float dutyCycle = 0.0;

#ifndef TPM_MOTOR_ON_LEVEL
#define TPM_MOTOR_ON_LEVEL kTPM_HighTrue
#endif

#ifndef TPM_SERVO_ON_LEVEL
#define TPM_SERVO_ON_LEVEL kTPM_HighTrue
#endif

//Konfig promeenne pro TTMP
tpm_config_t tpmInfo_motory;
tpm_chnl_pwm_signal_param_t tpmParam_motory[2];

tpm_config_t tpmInfo_servo;
tpm_chnl_pwm_signal_param_t tpmParam_servo;

//Inicializační funkce pro motory
void MotorsInit(void)
{
	//SERVO  - defaultne nastaveno na middle hodnotu
	tpmParam_servo.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO;
	tpmParam_servo.level            = TPM_SERVO_ON_LEVEL;
	tpmParam_servo.dutyCyclePercent = SERVO_MIDDLE;

	//MOTORY
	tpmParam_motory[0].chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR0;
	tpmParam_motory[0].level            = TPM_MOTOR_ON_LEVEL;

	tpmParam_motory[1].chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR1;
	tpmParam_motory[1].level            = TPM_MOTOR_ON_LEVEL;

    CLOCK_SetTpmClock(1U);

    //Nastaveni PWM pro motory
    TPM_GetDefaultConfig(&tpmInfo_motory);
    tpmInfo_motory.prescale = kTPM_Prescale_Divide_32;
    TPM_Init(BOARD_TPM_BASEADDR_MOTOR, &tpmInfo_motory);
    TPM_SetupPwm(BOARD_TPM_BASEADDR_MOTOR, tpmParam_motory, 2U, kTPM_EdgeAlignedPwm, 50U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR_MOTOR, kTPM_SystemClock);

    //Nastaveni PWM pro servo
    TPM_GetDefaultConfig(&tpmInfo_servo);
    tpmInfo_servo.prescale = kTPM_Prescale_Divide_32;
    TPM_Init(BOARD_TPM_BASEADDR_SERVO, &tpmInfo_servo);
    TPM_SetupPwm(BOARD_TPM_BASEADDR_SERVO, &tpmParam_servo, 1U, kTPM_EdgeAlignedPwm, 50U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR_SERVO, kTPM_SystemClock);

    //Funkce na projiti serva a inicializai motoru, pokud zde, provede se to vždy, spouštěno proto tlačítkem dle potřeby v led_button.c
    //MotorSetRange();
    //servo_check();
    MotorSetSpeed(0);
}

//Funkce, která nastaví ESC regulátoru motorům maximální a minimální hodnoty
void MotorSetRange(void)
{
    // INICIALIZACE MOTORU
	led_C();
   	MotorSetSpeed(100);
   	SDK_DelayAtLeastUs(1000*1000,MHZ48);
   	MotorSetSpeed(0);
   	SDK_DelayAtLeastUs(1000U*1000,MHZ48);
   	PRINTF("SET MOTOR DONE\r\n");
   	led_off();

}

//Funkce co projde rozsah serva pro priad ze by se nekde neco zaseklo
void ServoCheckRange(void)
{
	led_M();
	SteerStraight();
	SDK_DelayAtLeastUs(500U*1000,MHZ48);
	for(uint8_t i = 1; i < 10; i++)
	{
		SteerRight(i*10);
		SDK_DelayAtLeastUs(200U*1000,MHZ48);
	}
	SDK_DelayAtLeastUs(500U*1000,MHZ48);
	SteerStraight();
	SDK_DelayAtLeastUs(500U*1000,MHZ48);
	for(uint8_t i = 1; i < 10; i++)
	{
		SteerLeft(i*10);
		SDK_DelayAtLeastUs(200U*1000,MHZ48);
	}
	SDK_DelayAtLeastUs(500U*1000,MHZ48);
	SteerStraight();
	SDK_DelayAtLeastUs(500U*1000,MHZ48);
	PRINTF("SET SERVO DONE\r\n");
	led_off();

}

//Funkce pro nastaveni stredni hodnoty serva pro jizdvu vpred
void SteerStraight(void)
{
	//PRINTF("STRAIGHT\r\n");
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, SERVO_MIDDLE);
	pctServoR = 0;
	pctServoL = 0;

}

//Funkce co nastavuje procenta z maximalniho uhlu (0 stred, 100 maximalni zatoceni vlevo)
void SteerLeft(uint8_t pct)
{
	//Ulozeni do globalnich promennych
	//PRINTF("LEFT %d\r\n",pct);
	pctServoL = pct;
	pctServoR = 0;
	float set_steer;
	if(pct < 1) set_steer = SERVO_MIDDLE;
	else if(pct > 100) set_steer = SERVO_MAX;
			else set_steer = SERVO_MIDDLE + pct*0.023;
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, set_steer);
}
//Funkce co nastavuje procenta z maximalniho uhlu (0 stred, 100 maximalni zatoceni vpravo))
void SteerRight(uint8_t pct)
{
	//PRINTF("RIGHT %d\r\n",pct);
	pctServoR = pct;
	pctServoL = 0;
	float set_steer;
	if(pct < 1) set_steer = SERVO_MIDDLE;
	else if(pct > 100) set_steer = SERVO_MIN;
			else set_steer = SERVO_MIDDLE - pct*0.023;
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, set_steer);
}


//Funkce nastaví procentuální hodnotu na jednotlivé PWM channely pro kazdý motor zvlášť
//Každý motor reaguje jinak, proto jsou různé hodnotykteré jsou nastaveny v hlavičkovém souboru
void MotorSetSpeed(int8_t pct)
{
	//PRINTF("Zmena rychlosti motoru na %d\r\n",pct);
	pctMotory = pct;
	float speed1 = MOTOR1_MIN + (MOTOR1_MAX-MOTOR1_MIN)* (pct*0.01);
	float speed2 = MOTOR2_MIN + (MOTOR2_MAX-MOTOR2_MIN)* (pct*0.01);

	//MINIMALNI A MAXIMALNI HODNOTA (mela by byt rozdila od 0)
	if(pct <= 0)
		{
		speed1 = MOTOR1_MIN;
		speed2 = MOTOR2_MIN;
		}
	if(pct >= 100)
		{
		speed1 = MOTOR1_MAX;
		speed2 = MOTOR2_MAX;
		}
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR0, kTPM_EdgeAlignedPwm, speed1);
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR1, kTPM_EdgeAlignedPwm, speed2);
}










