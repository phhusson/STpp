STpp
====

This is a C++ framework for STM32F4.
It is based on ST-provided stm32f4xx.h
The folders are (most of the time) organized as follow:

- plat-\* contain low level drivers. There is plat-unix which is are just stubs, and plat-stm with actual code.
plat-inc are the matching headers.
The philosphy behind them is to be a close match to actual STM32F4 registers.
It hides the access to RCC clocks though, and there are some helper functions.

- drivers contain higher level drivers. They are for components external to the STM32F4 itself.

Current state
=============

Supported drivers:
- AX-12 (one half-duplex wire, three wires, En + Tx + Rx)
- Bluetooth
- Three wires Half H-Bridge (PWM + EnA + EnB)
- HD44780-like lcd
- WS82(1112b)
- Neato XV11 Lidar

Supported STM32F4 function:
- Adc (only one shot, no DMA)
- DMA
- EXTI
- GPIO
- NVIC
- SPI
- Timer
- Uart
- UsbSerial
- Watchdog
