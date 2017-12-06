#include "onewire.h"
#include "message.h"
#include "commthread.h"

static char tempStr[NET_MESSAGE_LEN];


uint16_t tempSensor1;
uint16_t tempSensor2;
uint16_t tempSensor3;
uint16_t tempSensor4;
uint16_t tempSensor[4]= {0,0,0,0};

float transTemp(uint16_t temp){
    uint8_t minusFlag = 0;
    uint16_t tempInt;

    minusFlag = temp & 0x8000;

    if(minusFlag == 0){// positive number
        return temp * 0.0625;

    }else{
        tempInt = ~temp + 1;

        return -tempInt * 0.0625;
    }
    
}


void Enable_RX(uint16_t pin)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SENSOR_PORT, &GPIO_InitStruct);

}


void Enable_TX(uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SENSOR_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SENSOR_PORT, pin, GPIO_PIN_SET);
}

void Wire_On(uint16_t pin)
{
    HAL_GPIO_WritePin(SENSOR_PORT, pin, GPIO_PIN_SET);
}

void Wire_Off(uint16_t pin)
{
    HAL_GPIO_WritePin(SENSOR_PORT, pin, GPIO_PIN_RESET);
}

GPIO_PinState Wire_Read(uint16_t pin)
{

    return HAL_GPIO_ReadPin(SENSOR_PORT,  pin);
}

void DelayUs(uint32_t time)
{

    uint32_t i, j;

    uint32_t times = time * 10;

    for(j =0 ; j< times; j++)
    {

        i = 100000;

        do
        {
            i--;
        }
        while(i>0);

    }


}

uint8_t InitSensor(uint16_t pin){

    GPIO_PinState pinState;

    Enable_TX(pin);
    Wire_Off(pin);

    DelayUs(480); // delay 480us

    Wire_On(pin);
    Enable_RX(pin);


    DelayUs(60);

    pinState = Wire_Read(pin);
    //printf("OneWire: sensor1 init read:%d\n", pinState);

    if(pinState != 0)
    {
        return False;
    }
    else
    {
        return True;
    }
}

//uint8_t InitSensor1()
//{
//    return InitSensor(SENSOR_PIN1);
//}
void WriteBit(uint16_t pin, uint8_t bit)
{
    Enable_TX(pin);
    Wire_Off(pin);

    DelayUs(DELAY_PULL_LOW);

    if(bit != 0)
    {

        Wire_On(pin);
        DelayUs(58);
        Enable_RX(pin);

    }
    else
    {

        DelayUs(58);
        Wire_On(pin);
        Enable_RX(pin);
    }

}

void WriteSensor(uint16_t pin, uint8_t cmd)
{
    uint8_t i;
    uint8_t bit;

    for(i =0 ; i< 8; i++)
    {

        bit = (cmd >>i)&0x01;
        WriteBit(pin, bit);
        DelayUs(DELAY_BETWEEN_BIT);
    }
}
void TrigureSensor(uint16_t pin){

    uint8_t status = False;

    // Init
    status = InitSensor(pin);

    if(status != True)
    {
        sprintf(tempStr, "TrigureSensor: %d Init fail\r\n", pin);
        //CommPrint(tempStr);
        return;
    }

    DelayUs(DELAY_AFTER_INIT);

        //osDelay( DELAY_BETWEEN_OPERATION);

    // Skip ROM
    WriteSensor(pin, 0xCC);

    DelayUs(DELAY_BETWEEN_BYTE);

        //osDelay( DELAY_BETWEEN_OPERATION);

    // Convert T

    WriteSensor(pin, 0x44);

    
    sprintf(tempStr, "OneWire:trigure sensor %d\r\n", pin);
    CommPrint(tempStr);
    
}
void TrigureSensor1()
{
    TrigureSensor(SENSOR_PIN1 );

}
void TrigureSensor2()
{
    TrigureSensor(SENSOR_PIN2 );

}
void TrigureSensor3()
{
    TrigureSensor(SENSOR_PIN3 );

}
void TrigureSensor4()
{
    TrigureSensor(SENSOR_PIN4 );

}
uint8_t ReadBit(uint16_t pin){
    GPIO_PinState bit;
    
    Enable_TX(pin);
    Wire_Off(pin);

    DelayUs(DELAY_PULL_LOW);

    Wire_On(pin);
    Enable_RX(pin);

    DelayUs(DELAY_READ_BIT);

    bit =  Wire_Read( pin);

    return bit;    
}
void ReadSensorByte(uint16_t pin, uint8_t * b){
    uint8_t bit, i;

    *b = 0x00;

    for(i =0 ; i< 8; i++)
    {

        bit = ReadBit(pin);
        
        if(bit == 1){
            *b = *b |(1<<i);
        }
        DelayUs(DELAY_BETWEEN_READ_BIT);
    }    
}
void ReadSensor(uint16_t *store, uint16_t pin){
    
    uint8_t status = False;
    uint8_t bDataHi=0xff;
    uint8_t bDataLo=0xff;
    uint16_t temp;

    // Init
    status = InitSensor(pin);

    if(status != True)
    {
        sprintf(tempStr,"TrigureSensor: %d Init fail\r\n", pin);
        CommPrint(tempStr);
        return;
    }

    DelayUs(DELAY_AFTER_INIT);

    osDelay( DELAY_BETWEEN_OPERATION);

    // Skip ROM
    WriteSensor(pin, 0xCC);

    DelayUs(DELAY_BETWEEN_BYTE);

    // Read ScratchPad

    WriteSensor(pin, 0xBE);

    DelayUs(DELAY_BETWEEN_BYTE);

    ReadSensorByte(pin, &bDataLo);

    DelayUs(DELAY_BETWEEN_BYTE);

    ReadSensorByte(pin, &bDataHi);

    temp = (uint16_t)(bDataHi<<8 | bDataLo);

    sprintf(tempStr, "OneWire: sensor %d - temp:%x  degree:%.3f\r\n", pin,  temp, 
    transTemp(temp));

    CommPrint( tempStr);
    *store = (uint16_t)temp;
    
}
void ReadSensor1()
{
    ReadSensor( &tempSensor1, SENSOR_PIN1);

}
void ReadSensor2()
{
    ReadSensor( &tempSensor2, SENSOR_PIN2);

}
void ReadSensor3()
{
    ReadSensor( &tempSensor3, SENSOR_PIN3);

}

void ReadSensor4()
{
    ReadSensor( &tempSensor4, SENSOR_PIN4);

}



