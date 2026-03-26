#include <app/critical/controllers/timer.h>
#include <drivers/hardware/ports.h>
#include <drivers/interruption/pic/pic.h>
#include <stdint.h>

void timer_wait(int seconds) {
  uint64_t ticks = seconds * 1000;
  while (ticks > 0) {
    pic_wait();
    ticks--;
  }
}
