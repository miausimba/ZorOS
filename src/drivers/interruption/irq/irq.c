#include <drivers/hardware/keyboard.h>
#include <drivers/hardware/ports.h>
#include <drivers/interruption/idt/idt.h>
#include <drivers/interruption/irq/irq.h>
#include <drivers/interruption/pic/pic.h>
#include <stdint.h>

void pic_unmask_all(void) {
  outb(PIC1_DATA, 0x00);
  outb(PIC2_DATA, 0x00);
}

void irq_init(void) {
  pic_remap(PIC1_OFFSET, PIC2_OFFSET);
  pic_unmask_all();

  // Set IDT gates for IRQs 32-47
  idt_set_gate(32, (uint64_t)irq0, 0x08, 0x8E);
  idt_set_gate(33, (uint64_t)irq1, 0x08, 0x8E);
  idt_set_gate(34, (uint64_t)irq2, 0x08, 0x8E);
  idt_set_gate(35, (uint64_t)irq3, 0x08, 0x8E);
  idt_set_gate(36, (uint64_t)irq4, 0x08, 0x8E);
  idt_set_gate(37, (uint64_t)irq5, 0x08, 0x8E);
  idt_set_gate(38, (uint64_t)irq6, 0x08, 0x8E);
  idt_set_gate(39, (uint64_t)irq7, 0x08, 0x8E);
  idt_set_gate(40, (uint64_t)irq8, 0x08, 0x8E);
  idt_set_gate(41, (uint64_t)irq9, 0x08, 0x8E);
  idt_set_gate(42, (uint64_t)irq10, 0x08, 0x8E);
  idt_set_gate(43, (uint64_t)irq11, 0x08, 0x8E);
  idt_set_gate(44, (uint64_t)irq12, 0x08, 0x8E);
  idt_set_gate(45, (uint64_t)irq13, 0x08, 0x8E);
  idt_set_gate(46, (uint64_t)irq14, 0x08, 0x8E);
  idt_set_gate(47, (uint64_t)irq15, 0x08, 0x8E);

  // Re-enable interrupts
  __asm__ __volatile__("sti");
}

void irq_handler(uint64_t irq_vector, stack_frame_t *stack) {
  (void)stack;

  // Handle some specific IRQs
  if (irq_vector == 32) {
    // Timer (PIT) - Just silent for now
  } else if (irq_vector == 33) {
    // Keyboard
    keyboard_handler(irq_vector, stack);
  }

  pic_send_eoi((uint8_t)(irq_vector - 32));
}
