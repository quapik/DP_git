/*
 * led_button.c
 *
 *  Created on: 19. 2. 2025
 *      Author: xsimav01
 */
#include "led_button.h"

void led_set(uint8_t R,uint8_t G,uint8_t B)
{
	if(R == 1) R = 0;
	else R = 1;
	if(G == 1) G = 0;
	else G = 1;
	if(B == 1) B = 0;
	else B = 1;
	GPIO_PinWrite(BOARD_INITPINS_LED_GREEN_GPIO, BOARD_INITPINS_LED_GREEN_PIN, G);
	GPIO_PinWrite(BOARD_INITPINS_LED_RED_GPIO, BOARD_INITPINS_LED_RED_PIN, R);
	GPIO_PinWrite(BOARD_INITPINS_LED_BLUE_GPIO, BOARD_INITPINS_LED_BLUE_PIN, B);
}
void led_off(void){led_set(0,0,0);}
void led_on(void){led_set(1,1,1);}
void led_G(void){led_set(0,1,0);}
void led_R(void){led_set(1,0,0);}
void led_B(void){led_set(0,0,1);}
void led_M(void){led_set(1,0,1);}
void led_C(void){led_set(0,1,1);}
void led_Y(void){led_set(1,1,0);}


void SW1_pressed(void)
{
	//PRINTF("SW1 stisknuto!\r\n");

	if(startMotorsButtonPressed==false)
	{
		LPTMR_StartPosilejUART();
		//UART2_SendTextToHC05("START");
		HallResetValues();
		startMotorsButtonPressed=true;
	    led_G();
	    motor_set_speed(20);
	    driving = true;
	    dokoncenoKolo = false;


	    jedePixy = true;
	    PIT_StartPixyZpracovavatVektory();
	    PixyGetVectors();
	}
	else if(startMotorsButtonPressed==true)
		{
			LPTMR_StopPosilejUART();
			startMotorsButtonPressed=false;
		    led_R();
		    motor_set_speed(0);
		    driving = false;

		    jedePixy = false;
		    PIT_StopPixyZpracovavatVektory();
		    //UART2_SendTextToHC05("STOP ");

		}


}

void SW3_pressed(void)
{
	if(!jedePixy)
	{
		led_M();
		PRINTF("Pixy2 detekce spustena\r\n");
		jedePixy = true;
		PIT_StartPixyZpracovavatVektory();
		PixyGetVectors();

	}
	else
	{
		led_R();
		PRINTF("Pixy2 detekce vypnuta\r\n");
		jedePixy = false;
		PIT_StopPixyZpracovavatVektory();
	}

	//motor_set_check();
	//servo_check();

}

