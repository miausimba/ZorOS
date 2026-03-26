// tools
#include <app/critical/internal/shell.h>
#include <drivers/hardware/keyboard.h>
#include <drivers/hardware/ports.h>
#include <drivers/vga.h>
#include <stddef.h>

// aplication commands
#include <app/critical/commands/help.h>
#include <app/critical/commands/info.h>

// aplication criticals
#include <app/critical/internal/poweroff.h>

// complements, controllers and tools
#include <app/critical/controllers/timer.h>

// system
#include <app/system/programs/notepad.h>

// Minimal strcmp implementation for our freestanding kernel
static int my_strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

static inline void io_wait(void) { outb(0x80, 0); }

void shell_init(void) {
  vga_clear();
  vga_write("Welcome to ZorOS\n");
}

void shell_prompt(void) {
  char line[256];
  size_t line_pos = 0;

  vga_write("user@ZorOS:/$ ");

  while (1) {
    char c = get_key();
    if (c == '\0') {
      // Nothing pressed right now, wait
      __asm__ __volatile__("hlt");
      continue;
    }

    if (c == '\n') {
      vga_write("\n");
      line[line_pos] = '\0'; // Null-terminate the string

      // Process command
      if (line_pos > 0) {
        if (my_strcmp(line, "help") == 0) {
          help();
        } else if (my_strcmp(line, "clear") == 0) {
          vga_clear();
        } else if (my_strcmp(line, "info") == 0) {
          info();
        } else if (my_strcmp(line, "poweroff") == 0) {
          vga_write("\nShutting down ZorOS, se you later :D");
          for (int i = 0; i < 20; i++) {
            vga_write(".");
            timer_wait(150);
          }
          poweroff();
        } else if (my_strcmp(line, "notepad") == 0) {
          notepad_run();
        } else {
          vga_write("Unknown command: ");
          vga_write(line);
          vga_write("\n");
        }
      }

      // Reset prompt
      line_pos = 0;
      vga_write("user@ZorOS:/$ ");
    } else if (c == '\b') {
      // Backspace handling
      if (line_pos > 0) {
        line_pos--;
        vga_write("\b");
      }
    } else {
      if (line_pos < sizeof(line) - 1) {
        line[line_pos++] = c;
        char str[2] = {c, '\0'};
        vga_write(str);
      }
    }
  }
}