[BITS 64]
global idt_flush
extern idt_pointer

idt_flush:
    lidt [idt_pointer]
    ret