#include "commthread.h"
#include "main.h"
#include "cmdparser.h"
#include <string.h>
#include "usart.h"

TaskThread_t mCommThread;

uint8_t signalUART1 = False;
uint8_t signalUART3 = False;

extern char  UART1_RX[];
extern char  UART3_RX[];

static char tempStr[NET_MESSAGE_LEN];

void CommPrint(char* str)
{
    Mail_t * pMail;
    //osEvent  evt;
    uint16_t i,len;

    len = strlen(str);

    pMail = (Mail_t*)osMailAlloc(mCommThread.idMail, 30);

    if(pMail != NULL)
    {

        pMail->type= 0;
        pMail->length= len;


        for(i=0; i< len; i++)
        {

            pMail->data[i] = str[i];
        }

        pMail->data[i] = '\0';

        osMailPut(mCommThread.idMail, pMail);

    }
}

static void TaskLoop(void const * argument)
{
    Mail_t   *pMail;
    osEvent  evt;

    for(;;)
    {

        if(signalUART1 == True)
        {
            signalUART1 = False;

            sprintf(tempStr, "CommThread Rx:%s\r\n", UART1_RX);

            CommPrint(tempStr);

            UART1_Parse(UART1_RX);

        }
        else if(signalUART3 == True){
            signalUART3 = False;

            sprintf(tempStr, "commThread U3:%s len:%d\r\n", UART3_RX, 
            strlen(UART3_RX));

            CommPrint(tempStr);

            UART3_Parse(UART3_RX);
        }
        else
        {

            evt = osMailGet(mCommThread.idMail, 5);

            if(evt.status == osEventMail)//邮箱内有数据
            {
                pMail = (Mail_t*)evt.value.p;

                // safe printf
                //printf("::%s\r\n", pMail->data);

                UART1_Transmit((char*)pMail->data, strlen((char*)pMail->data));

                osMailFree(mCommThread.idMail, pMail);
            }

        }

        osDelay(10);
        

    }

}


void CommThreadInit()
{


    osThreadDef(commthread, TaskLoop,osPriorityHigh, 0, 256);
    mCommThread.idThread = osThreadCreate(osThread(commthread), NULL);

    if(mCommThread.idThread == NULL)
    {

        printf("CommThread create fail\r\n");
    }
    else
    {
        printf("CommThread create\r\n");
    }

    mCommThread.lenMail = 15; // 1KB

    osMailQDef(commmail, mCommThread.lenMail, Mail_t);
    mCommThread.idMail = osMailCreate(osMailQ(commmail), NULL);

    if(mCommThread.idMail  != NULL)
    {
        printf("commmail created\r\n");
    }
    else
    {
        printf("commmail create fail\r\n");
    }

}






