#include <app/critical/internal/shell.h>
#include <boot/multiboot2.h>
#include <drivers/interruption/gdt/gdt.h>
#include <drivers/interruption/idt/idt.h>
#include <drivers/interruption/irq/irq.h>
#include <drivers/hardware/keyboard.h>
#include <drivers/vga.h>
#include <kernel/kernel.h>

void kernel_main(uint64_t multiboot_addr) {
  gdt_init();
  idt_init();
  irq_init();
  keyboard_init();

  multiboot2_process(multiboot_addr);
  vga_clear();
  shell_init();
  shell_prompt();
}
