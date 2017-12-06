/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
#include "string.h"
#include "commthread.h"
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef DateToUpdate;

    /**Initialize RTC Only
    */
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
    if(HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initialize RTC and set the Time and Date
    */
    if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
    {
        sTime.Hours = 0x1;
        sTime.Minutes = 0x0;
        sTime.Seconds = 0x0;

        if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
        DateToUpdate.Month = RTC_MONTH_JANUARY;
        DateToUpdate.Date = 0x1;
        DateToUpdate.Year = 0x0;

        if(HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0x32F2);
    }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

    if(rtcHandle->Instance==RTC)
    {
        /* USER CODE BEGIN RTC_MspInit 0 */

        /* USER CODE END RTC_MspInit 0 */
        HAL_PWR_EnableBkUpAccess();
        /* Enable BKP CLK enable for backup registers */
        __HAL_RCC_BKP_CLK_ENABLE();
        /* Peripheral clock enable */
        __HAL_RCC_RTC_ENABLE();
        /* USER CODE BEGIN RTC_MspInit 1 */

        /* USER CODE END RTC_MspInit 1 */
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

    if(rtcHandle->Instance==RTC)
    {
        /* USER CODE BEGIN RTC_MspDeInit 0 */

        /* USER CODE END RTC_MspDeInit 0 */
        /* Peripheral c disable */
        __HAL_RCC_RTC_DISABLE();
        /* USER CODE BEGIN RTC_MspDeInit 1 */

        /* USER CODE END RTC_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
void CustGetTime()
{
    //
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    char tempStr[64];
    
    
    if(HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        sprintf(tempStr, "RESPERRO\r\n");
        CommPrint(tempStr);

        return;

    }

    if(HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        sprintf(tempStr, "RESPERRO\r\n");
        CommPrint(tempStr);

        return;

    }
    
    sprintf(tempStr, "RESPTIME%02d-%02d-%02d:%02d-%02d-%02d\r\n",
            sDate.Year,
            sDate.Month,
            sDate.Date,
            sTime.Hours,
            sTime.Minutes,
            sTime.Seconds
           );
    CommPrint(tempStr);

    sprintf(tempStr, "RESPTIME%02d%02d%02d%02d%02d%02d\r\n",
            sDate.Year,
            sDate.Month,
            sDate.Date,
            sTime.Hours,
            sTime.Minutes,
            sTime.Seconds
           );    
           
    //sprintf(tempStr, "Hello\r\n");       
    
    UART3_Transmit( tempStr,  strlen(tempStr));

}

void CustSetTime(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t 
minute, uint8_t second ){

    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    char tempStr[50];

    sTime.Hours = hour;
    sTime.Minutes = minute;
    sTime.Seconds = second;



    if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        sprintf(tempStr, "RESPERRO\r\n");
        CommPrint(tempStr);

        return;

    }


    sDate.Year = year;
    sDate.Month = month;
    sDate.Date = date;
    
    if(HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        sprintf(tempStr, "RESPERRO\r\n");
        CommPrint(tempStr);

        return;

    }

    sprintf(tempStr,"RESPOK**\r\n");
    UART3_Transmit(tempStr, strlen(tempStr));

}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
