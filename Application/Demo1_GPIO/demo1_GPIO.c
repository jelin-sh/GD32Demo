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

void GPIO_Port_Write(struct Port* this, bool val) {
  if (val) {
    gpio_bit_set(this->port, this->pin);
  } else {
    gpio_bit_reset(this->port, this->pin);
  }
}

void GPIO_Port_Logout(struct Port* this) {
  gpio_bit_reset(this->port, this->pin);
  free(this);
}

struct Port* Port_Register(GPIO_Port port,
                           GPIO_Pin pin,
                           GPIO_Mode mode,
                           GPIO_Speed speed) {
  struct Port* res = calloc(1, sizeof(struct Port));
  res->Write = GPIO_Port_Write;
  res->Logout = GPIO_Port_Logout;

  res->mode = mode;
  res->pin = pin;
  res->port = port;
  res->speed = speed;

  gpio_init(res->port, res->mode, res->speed, res->pin);
  return res;
}
