#include <customUART.h>
#include <stdint.h>
int N,A;
int i;
unsigned char data = 0;

/*********************************Initialization of UART*************************************/

void initUART(long int baud, int parityEnable, int parityMode, int stopBits)
{
#ifdef PLATFORM_BOARD_RDKRX63N
SYSTEM.PRCR.WORD = 0xA50B; 		/* Protect off */
#endif

SYSTEM.MSTPCRA.BIT.ACSE = 0;	/* clear ACSE Bit (All-Module Clock Stop Mode Enable) */	
MSTP(SCI6) = 0;

#ifdef PLATFORM_BOARD_RDKRX63N
SYSTEM.PRCR.WORD = 0xA500; 		/* Protect on  */
#endif	


SCI6.SCR.BYTE = 0x00;
								/* Set up the UART I/O port and pins. */
MPC.P32PFS.BYTE = 0x4A;			/* P50 is TxD2 */
MPC.P33PFS.BYTE = 0x4A;			/* P52 is RxD2 */
PORT3.PDR.BIT.B2 = 1;			/* TxD2 is output. */
PORT3.PDR.BIT.B3 = 0;			/* RxD2 is input. */
PORT3.PMR.BIT.B2 = 1;			/* TxD2 is peripheral. */
PORT3.PMR.BIT.B3 = 1;			/* RxD2 is peripheral. */
					
								/* Set data transfer format in Serial Mode Register (SMR)*/ 
								/*  -Asynchronous Mode`
	    							-8 bits
	    							-no parity
	    							-1 stop bit
	    							-PCLK clock (n = 0) */
		
SCI6.SMR.BYTE =0x00;		
SCI6.SMR.BIT.PE = 0;			// Parity is disabled.
SCI6.SMR.BIT.PM = 1; 			//setting this to 1 causes the parity bit to be high when the parity of the data is odd
SCI6.SMR.BIT.STOP =0;			// No STOP bit.
SCI6.SCMR.BIT.SMIF=0;			/* Set to 0 for serial communications interface mode. */
if(baud==319999)				//baudrate=150
{				
	SCI6.BRR = 155;
	SCI6.SMR.BYTE |=0x03;
}

if(baud==159999)
{				
	SCI6.BRR = 77;				//baudrate=300
	SCI6.SMR.BYTE |=0x03;	
}


if(baud==79999)					//baudrate=600
{				
	SCI6.BRR = 155;
	SCI6.SMR.BYTE |=0x02;
}

if(baud==39999)					//baudrate=1200
{				
	SCI6.BRR = 77;
	SCI6.SMR.BYTE |=0x02;
}

if(baud==19999)
{				
	SCI6.BRR = 155;				//baudrate=2400
	SCI6.SMR.BYTE |=0x01;
}

if(baud==9999)
{				
	SCI6.BRR = 77;				//baudrate=4800
	SCI6.SMR.BYTE |=0x01;
}

if(baud==6665)					//baudrate=7200
{				
	SCI6.BRR = 51;
	SCI6.SMR.BYTE |=0x01;
}

if(baud==4999)					//baudrate=9600
{				
	SCI6.BRR = 155;
	SCI6.SMR.BYTE |=0x00;
}
if(baud==3332)					//baudrate=14400		
{
	SCI6.BRR = 25;
	SCI6.SMR.BYTE |=0x01;
}
if(baud==2499)					//baudrate=19200
{				
	SCI6.BRR = 4;
	SCI6.SMR.BYTE |=0x02;	
}
if(baud==1665)					//baudrate=28800
{				
	SCI6.BRR = 51;
	SCI6.SMR.BYTE |=0x00;	
}
if(baud==1249)					//baudrate=38400
{				
	SCI6.BRR = 38;
	SCI6.SMR.BYTE |=0x00;	
}
if(baud==856)					//baudrate=56000
{				
	SCI6.BRR = 26;
	SCI6.SMR.BYTE |=0x00;	
}
if(baud==832)					//baudrate=57600
{				
	SCI6.BRR = 25;
	SCI6.SMR.BYTE |=0x00;	
}
if(baud==415)					//baudrate=115200
{				
	SCI6.BRR = 12;
	SCI6.SMR.BYTE |=0x00;	
}
if(baud==207)					//baudrate=230400
{				
SCI6.BRR = 6;
SCI6.SMR.BYTE |=0x00;	
}
								
SCI6.SCR.BIT.RIE = 1;    		//Receiver Interrupt bit is enabled.
SCI6.SCR.BIT.TIE = 0;			//Transmit Interrupt bit is enabled.
SCI6.SCR.BIT.TEIE = 0;			//Transmit End Interrupt is enabled.
IR(SCI6, RXI6) = 0;				//Clears Interrupt request bits for RXI6.
IR(SCI6, TXI6) = 0;				//Clears Interrupt request bits for TXI6.
SCI6.SCR.BYTE |= 0x30;			//Enables Transmission and Reception.
}

/**************************Function to check the data availability**************************/

int newDataAvailable(void)
{
	int status = 0;
	SCI6.SSR.BIT.ORER = 0;	// Reset in case of serial lockup due to overrun
	if(SCI6.SSR.BYTE & 0x40)
	{
		return 1;				//if new data is available then return 1 else 0.
	}
	else
	return 0;
}

/*****************************Character Transmitting Function*************************/

void tx_UART(char data)
{	
	
	if(SCI6.SSR.BIT.TEND==1)
 	SCI6.TDR = data;		//when the TEND is set store the data in variable data.
	
}

/******************************Character Receiving Function********************************/

char rx_UART(void)
{	
				
	unsigned char rcvd_data = 0;
	rcvd_data = SCI6.RDR;	//The data received is returned to main.
	return(rcvd_data);
			
}

	