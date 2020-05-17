#include "usart.h"

#include "stdlib.h"
#include "string.h"

int USART_GetUsartIndex(USARTx handle);
struct Usart* USART_Register(USARTx handle,
                             Baudrate baud,
                             Parity parity,
                             WordLength wd,
                             StopBit stopBit);
void USART_BindingPrintf(struct Usart* handle);
void USATR_EnableReceive(struct Usart* this);
void USATR_DisableReceive(struct Usart* this);

struct Usart* UsartMap[USART_NUMS] = {0};
static struct Usart* PrintUsart = NULL;

struct UsartManager Usart = {USART_GetUsartIndex, USART_Register,
                             USART_BindingPrintf};

void USART_Deinit(struct Usart* this) {
  usart_deinit(this->handle);
}

void USART_Init(struct Usart* this) {
  this->Deinit(this);
  usart_baudrate_set(this->handle, this->baud);
  usart_parity_config(this->handle, this->parity);
  usart_word_length_set(this->handle, this->wordLength);
  usart_stop_bit_set(this->handle, this->stopBit);

  if (this->handle == USART0) {
    nvic_irq_enable(USART0_IRQn, 0, 1);
  } else if (this->handle == USART1) {
    nvic_irq_enable(USART1_IRQn, 0, 1);
  } else if (this->handle == USART2) {
    nvic_irq_enable(USART2_IRQn, 0, 1);
  } else if (this->handle == UART3) {
    nvic_irq_enable(UART3_IRQn, 0, 1);
  } else if (this->handle == UART4) {
    nvic_irq_enable(UART4_IRQn, 0, 1);
  }
}

void USART_Open(struct Usart* this) {
  usart_enable(this->handle);
  usart_transmit_config(this->handle, USART_TRANSMIT_ENABLE);
}
void USART_Close(struct Usart* this) {
  usart_disable(this->handle);
  usart_transmit_config(this->handle, USART_TRANSMIT_DISABLE);
}
void USATR_SendByte(struct Usart* this, Byte byte) {
  usart_data_transmit(this->handle, byte);
  while (RESET == usart_flag_get(this->handle, USART_FLAG_TBE))
    ;
}
void USATR_SendBytes(struct Usart* this, Byte* data, uint16_t length) {
  int i = 0;
  for (i = 0; i < length; i++) {
    this->SendByte(this, data[i]);
  }
}
void USATR_SendString(struct Usart* this, char* data) {
  this->SendBytes(this, (Byte*)data, strlen(data));
}
uint16_t USATR_ReadOfSign(struct Usart* this,
                          Byte* startSign,
                          uint16_t startSignLength,
                          Byte* endSign,
                          uint16_t endSignLength,
                          Byte* rxBuffer,
                          bool wait) {
  int i = 0;
  bool canRes = false;
  do {
    this->Read(this);
    if (this->rxData.length > 0) {
      /*判断StartSign*/
      if (startSign != NULL) {
        for (i = 0; i < startSignLength; i++) {
          if (this->rxData.rxBuffer[i] != startSign[i]) {
            break;
          }
        }
        canRes = (i == startSignLength);
      } else {
        canRes = true;
      }
      if (!canRes) {
        continue;
      }
      /*判断EndSign*/
      if (startSign != NULL) {
        for (i = 0; i < endSignLength; i++) {
          if (this->rxData.rxBuffer[(this->rxData.length - 1) - i] !=
              endSign[endSignLength - 1 - i]) {
            break;
          }
        }
        canRes = (i == endSignLength);
      } else {
        canRes = true;
      }
      if (canRes) {
        break;
      }
    }
  } while (wait);
  for (i = 0; i < this->rxData.length - (endSignLength + startSignLength);
       i++) {
    rxBuffer[i] = this->rxData.rxBuffer[startSignLength + i];
  }
  return i;
}
uint16_t USATR_ReadLine(struct Usart* this, bool wait) {
  return 0;
}
uint16_t USATR_Read(struct Usart* this) {
  uint16_t timeout = 0;
  this->rxData.count = 0;
  this->rxData.systick = SystemSystick;
  this->rxData.length = 0;
  this->rxData.isEnd = false;
  USATR_EnableReceive(this);
  memset(this->rxData.rxBuffer, 0, USART_RX_BUFFER_SIZE);

  while (true) {
    if ((SystemSystick - this->rxData.systick) > 2) {
      timeout++;
    }
    if (timeout > this->rxTimeout) {
      break;
    }
    delay_1ms(1);
  }

  this->rxData.isEnd = true;
  this->rxData.length = this->rxData.count;
  return this->rxData.length;
}
void USATR_EnableReceive(struct Usart* this) {
  usart_receive_config(this->handle, USART_RECEIVE_ENABLE);
  usart_interrupt_enable(this->handle, USART_INT_RBNE);
}
void USATR_DisableReceive(struct Usart* this) {
  usart_receive_config(this->handle, USART_RECEIVE_DISABLE);
  usart_interrupt_disable(this->handle, USART_INT_RBNE);
}
int USART_GetUsartIndex(USARTx handle) {
  int index = -1;
  int i;
  for (i = 0; i < USART_NUMS; ++i) {
    if (UsartMap[i]->handle == handle) {
      index = i;
      break;
    }
  }
  return index;
}
void USART_BindingPrintf(struct Usart* handle) {
  PrintUsart = handle;
}
struct Usart* USART_Register(USARTx handle,
                             Baudrate baud,
                             Parity parity,
                             WordLength wd,
                             StopBit stopBit) {
  int index = Usart.GetMapIndex(handle);
  struct Usart* res = NULL;
  if (index == -1) {
    res = calloc(1, sizeof(struct Usart));
    for (index = 0; index < USART_NUMS; ++index) {
      if (UsartMap[index] == 0) {
        break;
      }
    }
    UsartMap[index] = res;
  } else {
    res = UsartMap[index];
  }

  res->baud = baud;
  res->wordLength = wd;
  res->handle = handle;
  res->stopBit = stopBit;
  res->parity = parity;
  res->rxTimeout = 1;

  memset(res->rxData.rxBuffer, 0, USART_RX_BUFFER_SIZE);
  res->rxData.count = 0;
  res->rxData.systick = 0;
  res->rxData.length = 0;
  res->rxData.isEnd = true;

  res->Deinit = USART_Deinit;
  res->Init = USART_Init;
  res->Open = USART_Open;
  res->Close = USART_Close;
  res->SendByte = USATR_SendByte;
  res->SendBytes = USATR_SendBytes;
  res->SendString = USATR_SendString;
  
  res->ReceiveListener = NULL;
  res->ReadOfSign = USATR_ReadOfSign;

  res->Read = USATR_Read;

  res->Init(res);
  return res;
}


int fputc(int ch, FILE* f) {
  if (PrintUsart == NULL) {
    return ch;
  }
  PrintUsart->SendByte(PrintUsart, ch);
  return ch;
}

void USART0_IRQHandler(void) {
  int index = Usart.GetMapIndex(USART0);
  Byte ch = usart_data_receive(UsartMap[index]->handle);
  if (index == -1 || UsartMap[index]->rxData.isEnd) {
    return;
  }
  if (UsartMap[index]->ReceiveListener != NULL) {
    UsartMap[index]->ReceiveListener(ch);
    return;
  } else {
    UsartMap[index]->rxData.systick = SystemSystick;
    UsartMap[index]->rxData.rxBuffer[UsartMap[index]->rxData.count++] = ch;
    UsartMap[index]->rxData.isEnd =
        (UsartMap[index]->rxData.count >= USART_RX_BUFFER_SIZE);
  }
}
