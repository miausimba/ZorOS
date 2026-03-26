#ifndef VGA_H
#define VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((uint16_t *)VGA_ADDRESS)

#define VGA_CTRL 0x3D4
#define VGA_DATA 0x3D5

// function for write or print on vga
void vga_write(const char *str);

// function for clear vga
void vga_clear(void);

#endif /* VGA_H */