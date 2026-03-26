#include <drivers/hardware/keyboard.h>
#include <drivers/hardware/ports.h>
#include <drivers/interruption/irq/irq.h>
#include <drivers/vga.h>
#include <stdbool.h>
#include <stdint.h>

#define KEYBOARD_MAP_SIZE 128
static bool keyboard_shift = false;
static bool keyboard_capslock = false;

const char keyboard_map[KEYBOARD_MAP_SIZE] = {
    '\0', 27, '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8', '9', '0',
    '-',  '=',  '\b', '\0', 'q',  'w',  'e',  'r',  't',  'y', 'u', 'i',
    'o',  'p',  '[',  ']',  '\n', '\0', 'a',  's',  'd',  'f', 'g', 'h',
    'j',  'k',  'l',  ';',  '\'', '`',  '\0', '\\', 'z',  'x', 'c', 'v',
    'b',  'n',  'm',  ',',  '.',  '/',  '\0', '\0', '\0', ' '};

const char keyboard_map_upper[KEYBOARD_MAP_SIZE] = {
    '\0', 27, '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*', '(', ')',
    '_',  '+',  '\b', '\0', 'Q',  'W',  'E',  'R',  'T',  'Y', 'U', 'I',
    'O',  'P',  '{',  '}',  '\n', '\0', 'A',  'S',  'D',  'F', 'G', 'H',
    'J',  'K',  'L',  ':',  '"',  '~',  '\0', '|',  'Z',  'X', 'C', 'V',
    'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0', '\0', ' '};

#define KEYBOARD_BUFFER_SIZE 1024
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_index = 0;

void keyboard_init(void) {
  for (int i = 0; i < KEYBOARD_BUFFER_SIZE; i++) {
    keyboard_buffer[i] = '\0';
  }
}

char get_key(void) {
  if (keyboard_buffer_index > 0) {
    char c = keyboard_buffer[0];
    for (int i = 0; i < KEYBOARD_BUFFER_SIZE - 1; i++) {
      keyboard_buffer[i] = keyboard_buffer[i + 1];
    }
    keyboard_buffer_index--;
    return c;
  }
  return '\0';
}

void keyboard_handler(uint64_t irq_vector, stack_frame_t *stack) {
  (void)irq_vector;
  (void)stack;
  uint8_t scancode = inb(0x60);

  if (scancode == 0x2A || scancode == 0x36) {
    keyboard_shift = true;
  } else if (scancode == 0xAA || scancode == 0xB6) {
    keyboard_shift = false;
  } else if (scancode == 0x3A) {
    keyboard_capslock = !keyboard_capslock;
  }



  if (scancode >= 128) {
    // Key release (or extended keys), ignore for characters
    return;
  }

  char c = keyboard_shift || keyboard_capslock ? keyboard_map_upper[scancode]
                                               : keyboard_map[scancode];

  if (keyboard_capslock) {
    if (c >= 'a' && c <= 'z') {
      c = c - 'a' + 'A';
    } else if (c >= 'A' && c <= 'Z') {
      c = c - 'A' + 'a';
    }
  }

  if (c != '\0') {
    // Write character to buffer
    int next = (keyboard_buffer_index + 1) % KEYBOARD_BUFFER_SIZE;
    if (next != keyboard_buffer_index) {
      keyboard_buffer[keyboard_buffer_index] = c;
      keyboard_buffer_index = next;
    }
  }
}
