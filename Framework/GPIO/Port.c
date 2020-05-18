#include "Port.h"

#include "stdlib.h"

void GPIO_Port_Write(struct Port* this, bool val) {
  if (val) {
    gpio_bit_set(this->port, this->pin);
  } else {
    gpio_bit_reset(this->port, this->pin);
  }
}

bool GPIO_Port_Read(struct Port* this) {
  return gpio_input_bit_get(this->port, this->pin);
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
  res->Read = GPIO_Port_Read;

  res->mode = mode;
  res->pin = pin;
  res->port = port;
  res->speed = speed;

  gpio_init(res->port, res->mode, res->speed, res->pin);
  return res;
}
void Port_Remap_Enable(GPIO_REMAP map) {
  gpio_pin_remap_config(map, ENABLE);
}
void Port_Remap_Disable(GPIO_REMAP map) {
  gpio_pin_remap_config(map, DISABLE);
}
