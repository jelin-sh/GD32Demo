#include "main.h"

int main(void) {
  systick_config();
  OnSystemStartup();

  for (;;) {
    Loop();
  }
}

__weak void OnSystemStartup(void) {
  __NOP();
}
__weak void Loop(void) {
  __NOP();
}
