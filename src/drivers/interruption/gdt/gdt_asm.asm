; gdt.asm
global gdt_flush

gdt_start:
    dq 0x0000000000000000
    dq 0x00AF9A000000FFFF
    dq 0x00AF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start

[BITS 64]
gdt_flush:
    lgdt [gdt_descriptor]
    ; flush the data segment registers
    mov ax, 0x10 
    mov ds, ax   
    mov es, ax   
    mov fs, ax   
    mov gs, ax   
    mov ss, ax   
    
    ; reload code segment
    push 0x08
    mov rax, gdt_flush_cs
    push rax
    retfq

gdt_flush_cs:
    ret

