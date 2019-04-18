# stm32-tobacco-mcu

This is a stm32f103vet project. Use 4 gpio ports to get temperature from 4 DS18B20 sensors.

FreeRTOS used, perfect for realtime MCU applications.

## Important Points

Use taskENTER_CRITICAL, taskEXIT_CRITICAL to mask interrupts and keep sensor reading time sequence.

