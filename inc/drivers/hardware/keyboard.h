#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <drivers/interruption/idt/idt.h>
#include <drivers/interruption/irq/irq.h>
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init(void); // init the keyboard
void keyboard_handler(uint64_t irq_vector, stack_frame_t *stack);
char get_key(void);

#endif /* KEYBOARD_H */