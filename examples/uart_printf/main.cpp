/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
**
*****************************************************************************
*/

/* Includes */
#include "system.h"
#include "uart.h"

UART* uartPtr = NULL;

void rx_callback(uint8_t byte)
{
  uartPtr->put_byte(byte);
  uartPtr->flush();
}

int main()
{
  systemInit();

  UART uart(USART1);
  uartPtr = &uart;
  uart.register_rx_callback(rx_callback);

  uint8_t size = 30;
  while(1)
  {
    uint8_t hello_string[size];
    for (uint8_t i = 0; i < size; i++)
      hello_string[i] = i;
    uart.write(hello_string, size);
    delay(200);
  }
}
