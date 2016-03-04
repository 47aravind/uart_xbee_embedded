#include "platform.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>



void initUART(long int baud, int parityEnable, int parityMode, int stopBits); // This function initializes the UART for us
int newDataAvailable(void); // Checks the UART for new data
char rx_UART(void); // This function reads the UART data register
void tx_UART(char); // This function transmits data through the UART

