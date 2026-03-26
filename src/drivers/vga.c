#include <drivers/hardware/ports.h>
#include <drivers/vga.h>
#include <stdint.h>

static size_t cursor = 0;
static size_t pointer = 0;
static uint16_t *vga_buffer = (uint16_t *)VGA_ADDRESS;

#define VGA_COLOR_WHITE 0x0F
#define VGA_ENTRY(ch, color) ((uint16_t)(ch) | (uint16_t)((color) << 8))

static void update_cursor(void) {
  uint16_t pos = cursor;
  outb(VGA_CTRL, 0x0F);
  outb(VGA_DATA, (uint8_t)(pos & 0xFF));
  outb(VGA_CTRL, 0x0E);
  outb(VGA_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

static void scroll(void) {
  for (size_t i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
    vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
  }
  for (size_t i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT;
       i++) {
    vga_buffer[i] = VGA_ENTRY(' ', VGA_COLOR_WHITE);
  }
  cursor -= VGA_WIDTH;
  pointer -= VGA_WIDTH;
  update_cursor();
}

void vga_clear(void) {
  for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga_buffer[i] = VGA_ENTRY(' ', VGA_COLOR_WHITE);
  }
  cursor = 0;
  pointer = 0;
  update_cursor();
}

void vga_write(const char *str) {
  while (*str) {
    if (*str == '\n') {
      cursor += VGA_WIDTH - (cursor % VGA_WIDTH);
      pointer = cursor;
      update_cursor();
      str++;
      continue;
    } else if (*str == '\b') {
      if (cursor > 0) {
        cursor--;
        pointer--;
        vga_buffer[cursor] = VGA_ENTRY(' ', VGA_COLOR_WHITE);
        update_cursor();
      }
      str++;
      continue;
    }
    vga_buffer[cursor] = VGA_ENTRY(*str, VGA_COLOR_WHITE);
    cursor++;
    pointer++;
    if (cursor >= VGA_WIDTH * VGA_HEIGHT) {
      scroll();
    }
    update_cursor();
    str++;
  }
}
