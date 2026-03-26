; boot.asm - ZorOS Boot Stub (32-bit entry to 64-bit transition)
[BITS 32]

section .text.boot
align 8
multiboot_header:
    dd 0xe85250d6                ; Magic (multiboot 2)
    dd 0                         ; Architecture 0 (protected mode i386)
    dd multiboot_header_end - multiboot_header ; Header length
    dd 0x100000000 - (0xe85250d6 + 0 + (multiboot_header_end - multiboot_header)) ; Checksum

    ; Tags could go here
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
multiboot_header_end:

global efi_main
extern kernel_main

efi_main:
    cli
    mov esp, stack_top          ; Set up temporary stack
    
    ; Save Multiboot info
    mov edi, eax                ; Magic in EDI
    mov esi, ebx                ; Info struct pointer in ESI

    ; 1. Check if Long Mode is available
    ; (Simplified check for brevity, assuming modern x86_64)
    
    ; 2. Set up Paging (identity map first 2MB)
    ; Clear page tables
    mov edi, p4_table
    mov ecx, 3072               ; 3 tables * 1024 dwords
    xor eax, eax
    rep stosd
    
    ; Link tables
    mov eax, p3_table
    or eax, 0b11                ; present + writable
    mov [p4_table], eax
    
    mov eax, p2_table
    or eax, 0b11
    mov [p3_table], eax
    
    ; Map first 2MB as huge page
    mov eax, 0x000000 | 0b10000011 ; present + writable + huge
    mov [p2_table], eax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set P4 table in CR3
    mov eax, p4_table
    mov cr3, eax

    ; Enable Long Mode in EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable Paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Load 64-bit GDT
    lgdt [gdt64.pointer]

    ; Jump to 64-bit mode
    jmp gdt64.code:long_mode_start

[BITS 64]
long_mode_start:
    ; Clean up segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call kernel_main(multiboot_ptr)
    ; Prototypes usually say efi_main(addr, header), 
    ; but kernel_main expects multiboot_ptr. SysV ABI passes in RDI.
    mov rdi, rbx                ; rbx still holds info struct pointer from ESI
    call kernel_main

    ; Halt if returns
    cli
.hlt:
    hlt
    jmp .hlt

section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .bss
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096
stack_bottom:
    resb 16384                  ; 16 KB stack
stack_top:

section .rodata
mensaje:
    db "ZorOS is booting...", 0
