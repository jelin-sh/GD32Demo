#include "demo1_GPIO.h"

#include "stdlib.h"

#if (DEMO1_OPEN == 1)
struct Port* PA1 = NULL;

void OnSystemStartup(void) {
  /*初始化引脚*/
  PA1 = Port_Register(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP, GPIO_OSPEED_MAX);

  /*打开外设时钟*/
  rcu_periph_clock_enable(RCU_GPIOA);
}

void Loop(void) {
  static unsigned loopCount = 0;

  PA1->Write(PA1, loopCount % 2);

  ++loopCount;
  delay_1ms(100);
}
#endif

