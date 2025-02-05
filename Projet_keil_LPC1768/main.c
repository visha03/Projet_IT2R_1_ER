/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
// Author: Cl�ment Loiseau
// Date: 15/03/2022






#define osObjectsPublic                     // define objects in main module
//#include "osObjects.h"                      // RTOS object definitions


//#include <cmsis_os.h>                   // ARM::CMSIS:RTOS:Keil RTX
#include "cmsis_os.h"
#include "LPC17xx.h"                    // Device header
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "Board_LED.h"                  // ::Board Support:LED
#include "PIN_LPC17xx.h"                // Keil::Device:PIN
#include "GPIO.h"
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
#include "LIDAR.h"
#include "Driver_SPI.h"                 // ::CMSIS Driver:SPI
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD


#define VITESSE_LIDAR 300;

extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

void Receive_Data(void const * arguments);
void Process_Data(void const * arguments);
void synchro_LIDAR(void);


osThreadId ID_Receive_Data,ID_Process_Data;

osThreadDef(Receive_Data, osPriorityHigh,1,0);
osThreadDef(Process_Data, osPriorityNormal,1,0);

int TRAME_START = 0x20 ;
int TRAME_STOP = 0x25 ;
int TRAME_RESET = 0x40 ;
int PRE_TRAME = 0xA5 ;
bool etatled = 0;
char value[5000];
int counter = 0;
short process1[4][360];
int process2[360];
int qualite;
int ANGLE_MESURE = 0 ;
int ANGLE_VISEE = 90;
int add = 0;
int passage = 0;
int passage2 = 0;
int poub = 1;
char val[20];

char text[50];
int i=0,y=0,z=0;
int turn = 0;
int angle;
int distance;
int distance_fini=0;
int angle_fini = 0;



int main (void){
	Initialise_GPIO();
	Init_UART1();
	initPwm();
	GLCD_Initialize();
	GLCD_ClearScreen();
	Initialise_GPIO();
	GLCD_SetFont(&GLCD_Font_16x24);
	LPC_PWM1->MR3 = VITESSE_LIDAR; // ceci ajuste la duree de l'�tat haut
	
	osDelay(5000);
	
	osKernelInitialize();
	ID_Receive_Data = osThreadCreate( osThread ( Receive_Data ), NULL);
	ID_Process_Data = osThreadCreate( osThread ( Process_Data ), NULL);

	osKernelStart();
	
	Start_LIDAR();
	
	osDelay(osWaitForever);
	
	return 0;
}



void Receive_Data(void const * arguments){ // Code de la tache 1
	while(1){
		

		synchro_LIDAR();	
		
		
		
		Driver_USART1.Receive(value,5000);
		while(Driver_USART1.GetRxCount()<5000);
		
		
		passage++;
		
			

		osSignalSet(ID_Process_Data,0x0001);
		osDelay(200);	

		
	
	}
	
}

void Process_Data(void const * arguments){ // Code de la tache 1
	while(1){
		osSignalWait(0x0001,osWaitForever);
		
		for(z=0;z<=360;z++){
			process1[0][z] = 0;
			process1[1][z] = 0;
			process1[2][z] = 0;
			process1[3][z] = 0;
		}
			
		passage2++;
		for(y=0;y<1000;y++){
			counter = y*5;
			//sprintf(text,"value = %x       ",value[counter]);
			//GLCD_DrawString(10,20,text);
		
			angle=(arrayToAngle(value[counter + 1],value[counter +2]))>>6;
			distance=((arrayToRange(value[counter + 3],value[ counter + 4]))>>2)/10;
			qualite = value[counter]>>2;
			process1[0][angle] = qualite;
			process1[1][angle] = angle;
			process1[2][angle] += distance;
			//process1[2][angle] = distance;
			process1[3][angle]++;
			}
		while(ANGLE_MESURE == 0){
				if((process1[2][ANGLE_VISEE+add]!= 0) & (process1[0][ANGLE_VISEE + add] >= 7)){ANGLE_MESURE = ANGLE_VISEE + add;}
				else{add++;}
				
			}
			distance_fini = (process1[2][ANGLE_MESURE])/process1[3][ANGLE_MESURE];
			//distance_fini = process1[2][ANGLE_MESURE];
			
			for(z=0;z<=360;z++){
				process2[z] = (process1[2][z])/(process1[3][z]);
			}
			
		  //sprintf(text,"distance = %d       ",distance_fini);
			//GLCD_DrawString(10,140,(char *)distance_fini);
			//sprintf(text,"angle = %d       ",ANGLE_MESURE);
			//GLCD_DrawString(10,160,text);
			
		
	}
}

void synchro_LIDAR(void){
	char first_val[1];
	char poubelle[4];
	char synch[1];
	first_val[0] = 0xAA;
	
	Allumer_1LED(0);
	while(first_val[0] != 0x3e){
		
		Driver_USART1.Receive(synch,1);
		while(Driver_USART1.GetRxCount()<1);
		
		if(synch[0] == 0x3e){
			
			Driver_USART1.Receive(poubelle,4);
			while(Driver_USART1.GetRxCount()<4);
			
			Driver_USART1.Receive(val,20);
			while(Driver_USART1.GetRxCount()<20);
			
			if((val[0] == 0x3e) & (val[5] == 0x3e) & (val[10] == 0x3e) & (val[15] == 0x3e)){
				first_val[0] = 0x3e;
				
			}
			
		}
	}
	Eteindre_1LED(0);
	
}

