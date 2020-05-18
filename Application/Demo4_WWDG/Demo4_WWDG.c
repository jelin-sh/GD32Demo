#include "Demo4_WWDG.h"

#if (DEMO4_OPEN == 1)

struct Port* key = NULL;
struct Port* led = NULL;
struct Usart* usart0 = NULL;

void OnSystemStartup(void) {
  rcu_apb1_clock_config(RCU_APB1_CKAHB_DIV2);

  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOC);
  rcu_periph_clock_enable(RCU_USART0);
  rcu_periph_clock_enable(RCU_AF);
  rcu_periph_clock_enable(RCU_WWDGT);

  key = Port_Register(GPIOA, GPIO_PIN_0, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ);
  led = Port_Register(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ);

  Port_Register(GPIOB, GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ);
  Port_Register(GPIOB, GPIO_PIN_7, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ);
  Port_Remap_Enable(GPIO_USART0_REMAP);

  usart0 = Usart.Register(USART0, 115200, USART_PM_NONE, USART_WL_8BIT,
                          USART_STB_1BIT);
  usart0->Open(usart0);
  Usart.BindingPrintf(usart0);

  printf("Restarted!!\r\n");
  delay_1ms(1000);
  wwdgt_deinit();
  wwdgt_config(100, 127, WWDGT_CFG_PSC_DIV8);
  wwdgt_enable();
  led->Write(led, 0);
}

void Loop(void) {
  static uint32_t count = 0;

  if (!(count % 1000)) {
    led->Write(led, (count / 1000) % 2);
  }
  if (!key->Read(key)) {
    wwdgt_counter_update(100);
  }
  ++count;
  delay_1ms(1);
}

#endif
