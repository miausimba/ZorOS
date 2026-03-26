#include <drivers/interruption/idt/idt.h>

struct idt_entry idt[IDT_MAX_DESCRIPTORS];
struct idt_pointer idt_pointer;

void idt_init(void) {
  idt_pointer.limit = sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;
  idt_pointer.base = (uint64_t)idt;
  for (int i = 0; i < IDT_MAX_DESCRIPTORS; i++) {
    idt[i].offset_low = 0;
    idt[i].selector = 0;
    idt[i].ist = 0;
    idt[i].flags = 0;
    idt[i].offset_mid = 0;
    idt[i].offset_high = 0;
    idt[i].zero = 0;
  }
  idt_flush();
}

void idt_set_gate(int num, uint64_t base, uint16_t sel, uint8_t flags) {
  idt[num].offset_low = base & 0xFFFF;
  idt[num].selector = sel;
  idt[num].ist = 0;
  idt[num].flags = flags;
  idt[num].offset_mid = (base >> 16) & 0xFFFF;
  idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
  idt[num].zero = 0;
}
