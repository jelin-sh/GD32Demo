#ifndef __MAIN_H
#define __MAIN_H

#include "gd32f30x_libopt.h"
#include "stdbool.h"
#include "systick.h"

int main(void);

__weak void OnSystemStartup(void);
__weak void Loop(void);

#endif
