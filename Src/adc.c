/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "commthread.h"

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

//uint32_t ADCSensor[7];
uint32_t ADCSensor1;
uint32_t ADCSensor2;
uint32_t ADCSensor3;
uint32_t ADCSensor4;
uint32_t ADCSensor5;
uint32_t ADCSensor6;
uint32_t ADCSensor7;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig;

    /**Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;

    if(HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    // Added by Yang
    if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }


}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(adcHandle->Instance==ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        /**ADC1 GPIO Configuration
        PA0-WKUP     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */
        /**  ADC1 GPIO
        PB0  --> ADC1_IN8
        PB1  --> ADC1_IN9

        PC0  --> ADC1_IN10

        **/
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* USER CODE END ADC1_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

    if(adcHandle->Instance==ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA0-WKUP     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3

        PB0  --> ADC1_IN8
        PB1  --> ADC1_IN9

        PC0  --> ADC1_IN10
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

        /* USER CODE BEGIN ADC1_MspDeInit 1 */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
void AvgADC(uint32_t *store, uint32_t ch, uint32_t sampletime)
{
    HAL_StatusTypeDef status;
    uint32_t data, dataSum=0;
    ADC_ChannelConfTypeDef sConfig;
    int i;

    char tempStr[50];

    sConfig.Channel = ch;
    sConfig.Rank = 1;
    sConfig.SamplingTime = sampletime;

    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }



    for(i =0 ; i < AVG_NUM; i++)
    {
        // Added by Yang
        if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        HAL_ADC_Start(&hadc1);

        status = HAL_ADC_PollForConversion(&hadc1, 50);

        if(status == HAL_OK)
        {

            data = HAL_ADC_GetValue(&hadc1);
            sprintf(tempStr, "ADC%d:%d  voltage:%f v\r\n", ch, data, 3.3*data/4096);
            printf(tempStr);


            dataSum += data;

        }
        else if(status == HAL_TIMEOUT)
        {
            sprintf(tempStr, "temp:timeout\r\n");
            printf(tempStr);


            *store = 0;
            return;

        }

        //osDelay(5);

    }
    *store = dataSum/AVG_NUM;

}

void PeakADC(uint32_t *store, uint32_t ch, uint32_t sampletime, uint32_t
             forTimes)
{

    HAL_StatusTypeDef status;
    uint32_t data, dataTemp = 0;
    ADC_ChannelConfTypeDef sConfig;
    int i;

    char tempStr[50];

    sConfig.Channel = ch;
    sConfig.Rank = 1;
    sConfig.SamplingTime = sampletime;

    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    for(i =0 ; i < forTimes; i++)
    {
        if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        HAL_ADC_Start(&hadc1);

        status = HAL_ADC_PollForConversion(&hadc1, 50);

        if(status == HAL_OK)
        {

            data = HAL_ADC_GetValue(&hadc1);
            sprintf(tempStr, "ADC%d:%d  voltage:%f v\r\n", ch, data, 3.3*data/4096);
            printf(tempStr);

            if(data > dataTemp)
            {
                dataTemp = data;
            }

            *store = dataTemp;

        }
        else if(status == HAL_TIMEOUT)
        {
            sprintf(tempStr, "temp:timeout\r\n");
            printf(tempStr);


            *store = 0;
            break;

        }

        //osDelay(5);

    }

}

// 250 ms delay
void ReadADC(uint32_t *store, uint32_t ch)
{
    HAL_StatusTypeDef status;
    uint32_t data;
    ADC_ChannelConfTypeDef sConfig;

    char tempStr[50];

    sConfig.Channel = ch;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    // Added by Yang
    if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }


    HAL_ADC_Start(&hadc1);

    status = HAL_ADC_PollForConversion(&hadc1, 50);

    if(status == HAL_OK)
    {

        data = HAL_ADC_GetValue(&hadc1);
        sprintf(tempStr, "ADC%d:%d  voltage:%f v\r\n", ch, data, 3.3*data/4096);
        printf(tempStr);


        *store = data;

    }
    else if(status == HAL_TIMEOUT)
    {
        sprintf(tempStr, "temp:timeout\r\n");
        printf(tempStr);


        *store = 0;


    }
}

void ReadADC_A()
{
    //ReadADC(&ADCSensor1, CHANNEL_AIN0);
    AvgADC(&ADCSensor1, CHANNEL_AIN0, ADC_SAMPLETIME_28CYCLES_5);

}
void ReadADC_B()
{
    // ReadADC(&ADCSensor2, CHANNEL_AIN1);
    AvgADC(&ADCSensor2, CHANNEL_AIN1, ADC_SAMPLETIME_28CYCLES_5);

}
void ReadADC_C()
{
    //ReadADC(&ADCSensor3, CHANNEL_AIN2);
    AvgADC(&ADCSensor3, CHANNEL_AIN2, ADC_SAMPLETIME_28CYCLES_5);

}
void ReadADC_30AC()
{
    //ReadADC(&ADCSensor4, CHANNEL_AIN3);
    PeakADC(&ADCSensor4, CHANNEL_AIN3, ADC_SAMPLETIME_28CYCLES_5,  20);

}
void ReadADC_WindDet()
{
    //ReadADC(&ADCSensor5, CHANNEL_AIN8);
    AvgADC(&ADCSensor5, CHANNEL_AIN8, ADC_SAMPLETIME_28CYCLES_5);

}
void ReadADC_5V()
{
    //ReadADC(&ADCSensor6, CHANNEL_AIN9);
    AvgADC(&ADCSensor6, CHANNEL_AIN9, ADC_SAMPLETIME_28CYCLES_5);

}
void ReadADC_BAT()
{
    //
    //ReadADC(&ADCSensor7, CHANNEL_AIN10);
    AvgADC(&ADCSensor7, CHANNEL_AIN10, ADC_SAMPLETIME_28CYCLES_5);

}

void CalibrateADC()
{
    // Added by Yang
    if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
