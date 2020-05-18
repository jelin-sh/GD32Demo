#ifndef __PORT__H
#define __PORT__H

#include "main.h"

typedef uint32_t GPIO_Pin;
typedef uint32_t GPIO_Mode;
typedef uint32_t GPIO_Speed;
typedef uint32_t GPIO_Port;
typedef uint32_t GPIO_REMAP;

struct Port {
  GPIO_Pin pin;
  GPIO_Mode mode;
  GPIO_Speed speed;
  GPIO_Port port;
  void (*Write)(struct Port*, bool);
  bool (*Read)(struct Port*);
  void (*Logout)(struct Port*);
};

struct Port* Port_Register(GPIO_Port port,
                           GPIO_Pin pin,
                           GPIO_Mode mode,
                           GPIO_Speed speed);
void Port_Remap_Enable(GPIO_REMAP map);

#endif
