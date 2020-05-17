#ifndef __USART_H
#define __USART_H

#define USART_NUMS 5
#define USART_RX_BUFFER_SIZE 200
#include "main.h"
#include "stdio.h"

typedef uint32_t USARTx;
typedef uint32_t Baudrate;
typedef uint32_t Parity;
typedef uint32_t WordLength;
typedef uint32_t StopBit;
typedef unsigned char Byte;

struct UsartRxSign {
  Byte* Sign;
  uint16_t length;
};
struct UsartRxData {
  Byte rxBuffer[USART_RX_BUFFER_SIZE];
  volatile uint32_t systick;
  volatile uint32_t count;
  volatile bool isEnd;
  uint32_t length;
};

struct Usart {
  USARTx handle;
  Baudrate baud;
  Parity parity;
  WordLength wordLength;
  StopBit stopBit;

  struct UsartRxData rxData;

  uint16_t rxTimeout;

  void (*Init)(struct Usart*);
  void (*Deinit)(struct Usart*);

  void (*Open)(struct Usart*);
  void (*Close)(struct Usart*);

  void (*SendByte)(struct Usart*, Byte);
  void (*SendBytes)(struct Usart*, Byte*, uint16_t);
  void (*SendString)(struct Usart*, char*);
  void (*ReceiveListener)(Byte);

  uint16_t (*Read)(struct Usart*);
  uint16_t (*ReadOfSign)(struct Usart*,
                         Byte* startSign,
                         uint16_t startSignLength,
                         Byte* endSign,
                         uint16_t endSignLength,
                         Byte* rxBuffer,
                         bool wait);
  uint16_t (*ReadLine)(struct Usart*);
  uint16_t (*ReadOfFormat)(struct Usart*, const char* format, ...);
};

struct UsartManager {
  int (*const GetMapIndex)(USARTx handle);
  struct Usart* (*const Register)(USARTx handle,
                                  Baudrate baud,
                                  Parity parity,
                                  WordLength wd,
                                  StopBit stopbit);
  void (*BindingPrintf)(struct Usart*);
  void (*BindingReceiveListener)(struct Usart*);
};

extern struct UsartManager Usart;

/*中断服务函数*/
void USART0_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void UART3_IRQHandler(void);
void UART4_IRQHandler(void);

#endif
