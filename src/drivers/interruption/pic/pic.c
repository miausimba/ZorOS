#include <drivers/hardware/ports.h>
#include <drivers/interruption/pic/pic.h>

static inline void io_wait(void) { outb(0x80, 0); }

void pic_remap(uint8_t offset1, uint8_t offset2) {
  uint8_t a1, a2;

  a1 = inb(PIC1_DATA);
  a2 = inb(PIC2_DATA);

  outb(PIC1_CMD, 0x11);
  io_wait();
  outb(PIC2_CMD, 0x11);
  io_wait();
  outb(PIC1_DATA, offset1);
  io_wait();
  outb(PIC2_DATA, offset2);
  io_wait();
  outb(PIC1_DATA, 0x04);
  io_wait();
  outb(PIC2_DATA, 0x02);
  io_wait();
  outb(PIC1_DATA, 0x01);
  io_wait();
  outb(PIC2_DATA, 0x01);
  io_wait();

  outb(PIC1_DATA, a1);
  outb(PIC2_DATA, a2);
}

void pic_send_eoi(uint8_t irq) {
  if (irq >= 8) {
    outb(PIC2_CMD, PIC_EOI);
  }
  outb(PIC1_CMD, PIC_EOI);
}

void pic_wait(void) { __asm__ __volatile__("outb %%al, $0x80" : : "a"(0)); }
