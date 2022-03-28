/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   STM32F4xx HAL API Template project 
  *
  * @note    modified by ARM
  *          The modifications allow to use this file as User Code Template
  *          within the Device Family Pack.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_LED.h"                  // ::Board Support:LED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Driver_USART.h>

extern ARM_DRIVER_USART Driver_USART3;

void Init_UART(void) {
    Driver_USART3.Initialize(NULL);
    Driver_USART3.PowerControl(ARM_POWER_FULL);
    Driver_USART3.Control(	ARM_USART_MODE_ASYNCHRONOUS |			// liaison asynchrone
                            ARM_USART_DATA_BITS_8		|           // donnée 8 bits
                            ARM_USART_STOP_BITS_1		|           // 1 bit de stop
                            ARM_USART_PARITY_NONE		|			// pas de bit de parité
                            ARM_USART_FLOW_CONTROL_NONE,            // aucun contrôle du flux
                            9600);                                  // default baud rate 9,6 kBD
    Driver_USART3.Control(ARM_USART_CONTROL_TX,1);                  // validation émission
    Driver_USART3.Control(ARM_USART_CONTROL_RX,1);                  // validation réception
}





int main (void){
    char data[100];                   
    char carac;                            // tableau pour récéption du symbole $
    char trameIntro[7]="GPGGA";            // trame GPS qu'on souhaite récupérer
		char i;                                // position
    Init_UART();

    while(1){
        // analyse de la trame

        // récupération un caractère à la fois dès qu'il arrive (on écrase l'ancien)
        Driver_USART3.Receive(&carac,1);       // remplir une case
        while(Driver_USART3.GetRxCount()<1); // on attend que 1 case soit pleine

        if(carac=='$')
        {   
						Driver_USART3.Receive(data,5); // la fonction remplira jusqu'à 6 cases
            while (Driver_USART3.GetRxCount()<5 ); // on attend que 6 cases soit pleine
						data[5]=NULL;
					
						if (strncmp(data,trameIntro,5)==0)
            {	
								// récupérer le reste 
								Driver_USART3.Receive(data,37); // la fonction remplira jusqu'à 6 cases
								while (Driver_USART3.GetRxCount()<37); // on attend que 6 cases soit pleine
							
								
						}

         }
					  
					
        }
		return 0;
}

