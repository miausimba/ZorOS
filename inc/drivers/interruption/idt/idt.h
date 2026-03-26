#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t ist;
  uint8_t flags;
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t zero;
} __attribute__((packed));

struct idt_pointer {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

void idt_init(void);
void idt_set_gate(int num, uint64_t base, uint16_t sel, uint8_t flags);
void idt_flush(void);

#endif /* IDT_H */