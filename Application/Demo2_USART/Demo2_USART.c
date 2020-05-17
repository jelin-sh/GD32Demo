#define USART0_DATA_ADDRESS ((uint32_t)&USART_DATA(USART0))

#include "Demo2_USART.h"

#if (DEMO2_OPEN == 1)

struct Usart* usart0 = NULL;
Byte rxBuffer[100] = {0};

void USART0_Init() {
  dma_parameter_struct dma_init_struct;

  rcu_periph_clock_enable(RCU_DMA0);
  rcu_periph_clock_enable(RCU_USART0);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_AF);

  Port_Register(GPIOB, GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ);
  Port_Register(GPIOB, GPIO_PIN_7, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ);
  Port_Remap_Enable(GPIO_USART0_REMAP);

  usart0 = Usart.Register(USART0, 115200, USART_PM_NONE, USART_WL_8BIT,
                          USART_STB_1BIT);
  usart0->Open(usart0);
  usart_receive_config(USART0, USART_RECEIVE_ENABLE);

  usart_dma_receive_config(USART0, USART_DENR_ENABLE);
  /* deinitialize DMA channel4 (USART0 rx) */
  dma_deinit(DMA0, DMA_CH4);
  dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_addr = (uint32_t)rxBuffer;
  dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
  dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
  dma_init_struct.number = 20;
  dma_init_struct.periph_addr = USART0_DATA_ADDRESS;
  dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
  dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
  dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
  dma_init(DMA0, DMA_CH4, &dma_init_struct);
  /* configure DMA mode */
  dma_circulation_enable(DMA0, DMA_CH4);
  /* enable DMA channel4 */
  dma_channel_enable(DMA0, DMA_CH4);
}

void OnSystemStartup(void) {
  USART0_Init();
  Usart.BindingPrintf(usart0);
  printf("Hello Jelin!!\r\n");
}

void Loop(void) {
  if (dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF) == SET) {
    dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF);
    printf("Received ok!\r\n");
    dma_channel_enable(DMA0, DMA_CH4);
  }
  delay_1ms(1);
}

#endif
