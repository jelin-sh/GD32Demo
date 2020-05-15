#ifndef __DEMO1_GPIO_H
#define __DEMO1_GPIO_H

#include "App.h"
#include "main.h"

typedef uint32_t GPIO_Pin;
typedef uint32_t GPIO_Mode;
typedef uint32_t GPIO_Speed;
typedef uint32_t GPIO_Port;

struct Port {
  GPIO_Pin pin;
  GPIO_Mode mode;
  GPIO_Speed speed;
  GPIO_Port port;
  void (*Write)(struct Port*, bool);
  void (*Logout)(struct Port*);
};

void OnSystemStartup(void);
void Loop(void);

struct Port* Port_Register(GPIO_Port port,
                           GPIO_Pin pin,
                           GPIO_Mode mode,
                           GPIO_Speed speed);
#endif
