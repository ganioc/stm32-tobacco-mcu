#ifndef ONEWIRE_H
#define ONEWIRE_H

#include "stm32f1xx_hal.h"

#define DELAY_AFTER_INIT 200
#define DELAY_BETWEEN_BYTE  100 // 50
#define DELAY_BETWEEN_BIT  20 // 5
#define DELAY_BETWEEN_READ_BIT  75  //45
#define DELAY_PULL_LOW         1
#define DELAY_READ_BIT        ( 15-DELAY_PULL_LOW)
#define DELAY_BETWEEN_OPERATION  5 // 1

#define SENSOR_PORT GPIOC
#define SENSOR_PIN1 GPIO_PIN_3
#define SENSOR_PIN2 GPIO_PIN_4
#define SENSOR_PIN3 GPIO_PIN_5
#define SENSOR_PIN4 GPIO_PIN_6


#define ENABLE_SENSOR1_TX() do{Enable_TX(SENSOR_PIN1);}while(0) 
#define ENABLE_SENSOR2_TX() do{Enable_TX(SENSOR_PIN2);}while(0) 
#define ENABLE_SENSOR3_TX() do{Enable_TX(SENSOR_PIN3);}while(0) 
#define ENABLE_SENSOR4_TX() do{Enable_TX(SENSOR_PIN4);}while(0) 

#define ENABLE_SENSOR1_RX() do{Enable_RX(SENSOR_PIN1);}while(0) 
#define ENABLE_SENSOR2_RX() do{Enable_RX(SENSOR_PIN2);}while(0) 
#define ENABLE_SENSOR3_RX() do{Enable_RX(SENSOR_PIN3);}while(0) 
#define ENABLE_SENSOR4_RX() do{Enable_RX(SENSOR_PIN4);}while(0) 


void Enable_RX(uint16_t pin);
void Enable_TX(uint16_t pin);
void Wire_Off(uint16_t pin);
void Wire_On(uint16_t pin);
GPIO_PinState Wire_Read(uint16_t pin);

void TrigureSensor1();
void TrigureSensor2();
void TrigureSensor3();
void TrigureSensor4();
void ReadSensor1();
void ReadSensor2();
void ReadSensor3();
void ReadSensor4();
float transTemp(uint16_t temp);

#endif