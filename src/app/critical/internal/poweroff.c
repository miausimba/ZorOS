#include <app/critical/internal/poweroff.h>
#include <drivers/hardware/ports.h>
#include <drivers/vga.h>

static inline void io_wait(void) { outb(0x80, 0); }

void poweroff(void) {
  vga_clear();
  __asm__ __volatile__("cli");

  io_wait();
  outb(PM1a_CNT, SLP_TYPa_S5 | SLP_EN); // acpi shutdown (PC REAL x86_64)

  /*
  // this is to shutdown the old pc
  outb(0x70, 0x05); // this is to shutdown the old pc
  outb(0x71, 0x00); // this is to shutdown the old pc
  */
  // QEMU shutdown
  outw(0x604, 0x2000);
  // VirtualBox shutdown
  outw(0x4004, 0x3400);
  io_wait();

  for (;;) {
    __asm__ __volatile__("hlt");
  }
}