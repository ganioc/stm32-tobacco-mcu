#include "cmdparser.h"
#include <string.h>
#include "gpio.h"
#include "message.h"
#include "commthread.h"
#include "rtc.h"
#include "usart.h"
#include <stdlib.h>

extern TaskThread_t mSensorThread;

uint8_t typeSensor;


void UART3_Parse(char * instr)
{
    char tempStr[128];

    char str[10];
    uint8_t year, month, date, hour, minute, second, index;

    strncpy(str,instr, 8);


    if(strcmp(str, "GET*TEMP") == 0)
    {

        osSignalSet(mSensorThread.idThread, 0x1);

        typeSensor = SENSOR_TEMPERATURE;
    }
    else if(strcmp(str, "GET*ADC*")== 0)
    {

        osSignalSet(mSensorThread.idThread, 0x2);

        typeSensor = SENSOR_ADC;
    }
    else if(strcmp(str, "GET*TIME")== 0)
    {
        CommPrint("parser:GET*TIME\r\n");
        CustGetTime();
    }
    else if(strcmp(str, "SET*TIME") == 0)
    {
        index = 8;

        tempStr[0] = instr[index++];
        tempStr[1] = instr[index++];
        tempStr[2] = '\0';

        year = atoi(tempStr);

        tempStr[0] = instr[index++];
        tempStr[1] = instr[index++];
        tempStr[2] = '\0';

        month = atoi(tempStr);

        tempStr[0] = instr[index++];
        tempStr[1] = instr[index++];
        tempStr[2] = '\0';

        date = atoi(tempStr);

        tempStr[0] = instr[index++];
        tempStr[1] = instr[index++];
        tempStr[2] = '\0';

        hour = atoi(tempStr);

        tempStr[0] = instr[index++];
        tempStr[1] = instr[index++];
        tempStr[2] = '\0';

        minute = atoi(tempStr);

        tempStr[0] = instr[index++];
        tempStr[1] = instr[index++];
        tempStr[2] = '\0';

        second = atoi(tempStr);

        sprintf(tempStr, "%d-%d-%d-%d-%d-%d\r\n", year, month, date, hour, 
        minute, second);
        CommPrint(tempStr);

        CustSetTime(year,  month,  date,  hour,  minute,  second);

    }
    else
    {
        sprintf(tempStr, "RESPERRO\r\n");
        UART3_Transmit(tempStr, strlen(tempStr));
    }

}

void UART1_Parse(char * str)
{
    char tempStr[100];

    if(strcmp(str, "quickled") == 0)
    {
        sprintf(tempStr, "Parser:set to quick led\r\n");
        //CommPrint(tempStr);
        Set_LED1_Quick();
    }
    else     if(strcmp(str, "normalled") == 0)
    {
        sprintf(tempStr,"Parser:set to normal led\r\n");
        //CommPrint(tempStr);
        Set_LED1_Normal();
    }
    else     if(strcmp(str, "slowled") == 0)
    {
        sprintf(tempStr,"Parser:set to slow led\r\n");
        //CommPrint(tempStr);
        Set_LED1_Normal();
    }
    else     if(strcmp(str, "veryquickled") == 0)
    {
        sprintf(tempStr,"Parser:set to veryquick led\r\n");
        //CommPrint(tempStr);
        Set_LED1_Very_Quick();
    }
    else     if(strcmp(str, "veryslowled") == 0)
    {
        sprintf(tempStr,"Parser:set to veryslow led\r\n");
        //CommPrint(tempStr);
        Set_LED1_Very_Slow();
    }
    else     if(strcmp(str, "buzzeron") == 0)
    {
        sprintf(tempStr,"Parser:buzzer on\r\n");
        //CommPrint(tempStr);
       // Buzzer1_On();
    }
    else     if(strcmp(str, "buzzeroff") == 0)
    {
        sprintf(tempStr,"Parser:buzzer off\r\n");
        //CommPrint(tempStr);
        //Buzzer1_Off();
    }
    else if(strcmp(str, "readtemp")==0)
    {
        sprintf(tempStr,"Parser:read temp\r\n");
        osSignalSet(mSensorThread.idThread, 0x1);

        typeSensor = SENSOR_TEMPERATURE;
    }
    else if(strcmp(str, "readadc")==0)
    {
        sprintf(tempStr,"Parser:read adc\r\n");
        CommPrint(tempStr);

        osSignalSet(mSensorThread.idThread, 0x2);

        typeSensor = SENSOR_ADC;
    }
    else
    {
        sprintf(tempStr,"Unrecognized cmd:%s\r\n", (char*)str);
        //CommPrint(tempStr);
    }
}

