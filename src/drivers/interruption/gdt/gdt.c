#include <drivers/hardware/ports.h>
#include <drivers/interruption/gdt/gdt.h>
#include <stddef.h>

static inline void io_wait(void) { outb(0x80, 0); }
#define GDT_PTR 0x0000

struct gdt_entry gdt[5];
struct gdt_pointer gdt_ptr;

// gdt_flush is implemented in assembly
extern void gdt_flush(void);

void gdt_init() {
  gdt_ptr.limit = (sizeof(gdt) - 1);
  gdt_ptr.base = (uint64_t)gdt;

  gdt[0] = (struct gdt_entry){0, 0, 0, 0, 0, 0};

  gdt[1] = (struct gdt_entry){
      .limit_low = 0xFFFF,
      .base_low = 0x0000,
      .base_mid = 0x00,
      .access = 0x9A,
      .flags_limit_high = 0xAF,
      .base_high = 0x00,
  };

  gdt[2] = (struct gdt_entry){
      .limit_low = 0xFFFF,
      .base_low = 0x0000,
      .base_mid = 0x00,
      .access = 0x92,
      .flags_limit_high = 0xCF,
      .base_high = 0x00,
  };

  gdt[3] = (struct gdt_entry){
      .limit_low = 0xFFFF,
      .base_low = 0x0000,
      .base_mid = 0x00,
      .access = 0x9A,
      .flags_limit_high = 0xCF,
      .base_high = 0x00,
  };

  gdt[4] = (struct gdt_entry){
      .limit_low = 0xFFFF,
      .base_low = 0x0000,
      .base_mid = 0x00,
      .access = 0x92,
      .flags_limit_high = 0xCF,
      .base_high = 0x00,
  };

  gdt_flush();
}

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t gran) {
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_mid = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].flags_limit_high = ((limit >> 16) & 0x0F) | (gran & 0xF0);
  gdt[num].access = access;
}