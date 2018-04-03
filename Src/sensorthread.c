#include "sensorthread.h"
#include "main.h"
#include "onewire.h"
#include "commthread.h"
#include "cmdparser.h"
#include "adc.h"
#include "usart.h"
#include "string.h"

TaskThread_t mSensorThread;

void (*arrTask[4])() =
{
    TrigureSensor1,
    TrigureSensor2,
    TrigureSensor3,
    TrigureSensor4,


};
void (*arrReadTask[4])() =
{

    ReadSensor1,
    ReadSensor2,
    ReadSensor3,
    ReadSensor4


};

//static uint8_t lenTask = 2;
extern uint8_t  typeSensor;

// extern uint32_t *ADCSensor;

extern uint32_t ADCSensor1;
extern uint32_t ADCSensor2;
extern uint32_t ADCSensor3;
extern uint32_t ADCSensor4;
extern uint32_t ADCSensor5;
extern uint32_t ADCSensor6;
extern uint32_t ADCSensor7;


extern uint16_t tempSensor1;
extern uint16_t tempSensor2;
extern uint16_t tempSensor3;
extern uint16_t tempSensor4;


void CopyTemp2Str(char *val, uint16_t temp)
{
    //char tempStr[60];
    uint8_t len;

    sprintf(val, "%.3f", transTemp(temp));

    len = strlen(val);

    //sprintf(tempStr, "%s\r\n", val);
    //CommPrint(tempStr);

    while(len < 8)
    {
        val[len++] = '0';

    }
    val[len] = '\0';

}

void SendOutTemp()
{
    char tempStr[80];
    //uint8_t len;

    char val1 [10], val2[10], val3[10], val4[10];

    /*
        sprintf(val, "%.3f", transTemp( tempSensor1));

        len = strlen(val);

        sprintf(tempStr, "%s\r\n", val);
        CommPrint(tempStr);

        while (len < 8){
            val[len++] = '0';

        }
        val[len] = '\0';
        */
    CopyTemp2Str(val1, tempSensor1);
    CopyTemp2Str(val2, tempSensor2);
    CopyTemp2Str(val3, tempSensor3);
    CopyTemp2Str(val4, tempSensor4);

    sprintf(tempStr, "RESPTEMP%s%s%s%s\r\n", val1, val2, val3, val4);
    CommPrint(tempStr);

    //sprintf(tempStr, "RESPTEMP%s%s%s%s\r\n", val1, val2, val3, val4);

    UART3_Transmit(tempStr, strlen(tempStr));

}

void CopyAdc2Str(char *val, uint32_t adc)
{
    uint8_t len;

    sprintf(val, "%.2f", 3.3*adc/4096);

    len = strlen(val);
    while(len <8)
    {
        val[len++] = '0';
    }
    val[len] = '\0';
}
void SendOutADC()
{
    char tempStr[100];


    char val1 [10], val2 [10],val3 [10], val4 [10], val5 [10], val6 [10], val7 [10];

    /*
        sprintf(val, "%.2f", 3.3*ADCSensor1/4096);

        len = strlen(val);
        while(len <8)
        {
            val[len++] = '0';
        }
        val[len] = '\0';
    */
    CopyAdc2Str(val1, ADCSensor1);
    CopyAdc2Str(val2, ADCSensor2);
    CopyAdc2Str(val3, ADCSensor3);
    CopyAdc2Str(val4, ADCSensor4);
    CopyAdc2Str(val5, ADCSensor5);
    CopyAdc2Str(val6, ADCSensor6);
    CopyAdc2Str(val7, ADCSensor7);

    sprintf(tempStr, "RESPADC*%s%s%s%s%s%s%s\r\n", val1, val2, val3, val4,
            val5,val6, val7);

    CommPrint(tempStr);

    UART3_Transmit(tempStr, strlen(tempStr));

}


static void TaskLoop(void const * argument)
{
    //uint8_t i = 0;
    osEvent ret;
    //char tempStr[50];


    while(1)
    {

        ret = osSignalWait(0x3,500);

        if(ret.status ==  osEventSignal)
        {
            if(typeSensor == SENSOR_TEMPERATURE)
            {

                arrTask[0]();
                osDelay(50);
                arrTask[1]();
                osDelay(50);
                arrTask[2]();
                osDelay(50);
                arrTask[3]();

                osDelay(700);

                arrReadTask[0]();
                osDelay(50);
                arrReadTask[1]();
                osDelay(150);
                arrReadTask[2]();
                osDelay(150);
                arrReadTask[3]();
                // send out the result to UART3
                osDelay(20);
                SendOutTemp();

            }
            else if(typeSensor == SENSOR_ADC)
            {
                // CalibrateADC();
                
                CommPrint("sensorthread:0x2\r\n");
                ReadADC_A();
                osDelay( 5);
                ReadADC_B();
                osDelay( 5);
                ReadADC_C();
                osDelay( 5);
                ReadADC_30AC();
                osDelay( 5);
                ReadADC_WindDet();
                osDelay( 5);
                ReadADC_5V();
                osDelay( 5);
                ReadADC_BAT();

                // send out the result to UART3
                SendOutADC();
            }

        }

    }

}

void SensorThreadInit()
{

    osThreadDef(sensorthread, TaskLoop,osPriorityHigh, 0, 256);
    mSensorThread.idThread = osThreadCreate(osThread(sensorthread), NULL);

    if(mSensorThread.idThread == NULL)
    {

        printf("SensorThread create fail\r\n");
    }
    else
    {
        printf("SensorThread create\r\n");
    }
}

