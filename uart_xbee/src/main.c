/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : main.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Empty application project
*                 This application provides a handy framework to serve as a
*                 generic basis for any application. The MCU startup procedures
*                 are provided, as well as some basic means for I/O. The LCD is 
*                 initialized, board switches and their interrupt ISR stubs are
*                 defined, and a simple loop blinks an LED to indicate that the
*                 board is operating.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release              
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "r_switches.h"
#include "customUART.h"

void receiveData(void);
char receivedData2[28];
char stringb[30];
int l;
int j;
char stringc[30];
int arr1[20];
int currentcharacter = 0x20;
int upperBound = 0x7E;
int lowerBound = 0x20;
char currentcharacterarray[19] = { 0x7E,0x00,0x0F,0x10,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x23,0xCB }; 	
/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/

/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
	
	/*Array with all the baud rates*/
	long int A,N;
	int baud=0;
    uint32_t led_counter;
    unsigned char rdata,character;
	char stringa[30];
	char d;	
	int i;
	char receivedData2[28];
	/* Initialize LCD */
    lcd_initialize();
    
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    lcd_display(LCD_LINE1, "  Lab 6   ");        
    /* The three pushbuttons on the YRDK board are tied to interrupt lines, set them up here */
    R_SWITCHES_Init();
    l=0;
    /* This is the main loop.  It does nothing but toggle LED4 periodically */
	initUART(4999,1,1,2);
	i=0;
    while (1)
    {			
		while(!newDataAvailable())				/*If newdata is available execute the following*/
		{
			;   
		} 		  
		  receivedData2[i]= rx_UART(); 
		  printf("%x ",receivedData2[i]);		/*receive buffer*/
		  i++; 
		  
		 if(i==28)
		{
			sprintf(stringa,"%c",receivedData2[26]);
			printf("\n%x\n",receivedData2[26]);	/*To display the received data*/
			lcd_display(LCD_LINE3, stringa);
			i=0;
		}
	}		
    
} /* End of function main() */


/******************************************************************************
* Function name: sw1_callback
* Description  : Callback function that is executed when SW1 is pressed.
*                Called by sw1_isr in r_switches.c.To increment the current character and printing it
				 on LCD.
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_callback(void)
{
	char stringa[20];
	if(currentcharacter<upperBound)
	currentcharacter++;
	
	sprintf(stringa,"%c",currentcharacter);
	lcd_display(LCD_LINE2, stringa);

} 
/* End of function sw1_callback() */


/******************************************************************************
* Function name: sw2_callback
* Description  : Callback function that is executed when SW2 is pressed.
*                Called by sw2_isr in r_switches.c.To decrement the current character.
* Arguments    : none
* Return value : none
******************************************************************************/
void sw2_callback(void)
{
	char stringa[20];
	if(currentcharacter>lowerBound)
		currentcharacter--;

	sprintf(stringa,"%c",currentcharacter);
	lcd_display(LCD_LINE2, stringa);	
} /* End of function sw2_callback() */
/******************************************************************************
* Function name: sw3_callback
* Description  : Callback function that is executed when SW3 is pressed.
*                Called by sw3_isr in r_switches.c.The current character is transmitted 
				 through the xbee to the coordinator.
* Arguments    : none
* Return value : none
******************************************************************************/
void sw3_callback(void)
{
    int i;
	initUART(4999,1,1,2);				/*for transmitting at 9600 baudrate*/
	
	for(i=0;i<1000000;i++)
		;
	currentcharacterarray[17] = currentcharacter;
	currentcharacterarray[18] = 0xFF - (currentcharacterarray[3] + currentcharacterarray[4] + + currentcharacterarray[5] + currentcharacterarray[6] + currentcharacterarray[7] + currentcharacterarray[8] + currentcharacterarray[9] + currentcharacterarray[10] + currentcharacterarray[11] + currentcharacterarray[12] + currentcharacterarray[13] + currentcharacterarray[14] + currentcharacterarray[15] + currentcharacterarray[16] + currentcharacterarray[17]);
		
	for(i=0;i<19;i++)
	{
		
		sprintf(stringb,"%c",currentcharacterarray[i]);
		lcd_display(LCD_LINE7, stringb);	
		tx_UART(currentcharacterarray[i]);
		
	}
	 /* Add your code here. Note: this is executing inside an ISR. */
} /* End of function sw3_callback() */

