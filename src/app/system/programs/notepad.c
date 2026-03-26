#include <app/critical/controllers/timer.h>
#include <app/system/programs/notepad.h>
#include <drivers/hardware/keyboard.h>
#include <drivers/vga.h>

void notepad_app(void);

void notepad_run(void) {
  vga_clear();
  vga_write("Welcome to ZorOS Notepad (tester version)\n");
  vga_write("Press ESC to exit\n");
  vga_write("\n");
  notepad_app();
}

void notepad_app(void) {
  while (1) {
    char c = get_key();
    if (c == '\0') {
      __asm__ __volatile__("hlt");
      continue;
    }
    if (c == '\n') {
      vga_write("\n");
    } else if (c == '\b') {
      vga_write("\b");
    } else if (c == 27) {
      vga_clear();
      break;
    } else {
      char str[2] = {c, '\0'};
      vga_write(str);
    }
  }
}

void notepad_exit(void) {
  vga_clear();

  __asm__ __volatile__("hlt");
}