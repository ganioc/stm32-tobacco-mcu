#ifndef CMDPARSER_H
#define CMDPARSER_H

#include "main.h"

#include "stm32f1xx_hal.h"

#define SENSOR_TEMPERATURE  1
#define SENSOR_ADC                  2


void UART1_Parse(char * str);
void UART3_Parse(char * str);
#endif

