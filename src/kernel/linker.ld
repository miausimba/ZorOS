/* ============================================================
 * ZorOS Linker Script
 * Target:  x86_64 (64-bit ELF)
 * Format:  ELF64 little-endian
 * Load at: 1 MiB (0x100000) – classic higher-half stub region
 * ============================================================ */

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

/* Kernel entry point (defined in boot.asm) */
ENTRY(efi_main)

SECTIONS
{
    /* --------------------------------------------------------
     * Place the kernel at 1 MiB physical / virtual.
     * Change this to your UEFI load address if needed.
     * -------------------------------------------------------- */
    . = 0x100000;

    /* ---- Text (code) section ---- */
    .text ALIGN(4K) :
    {
        KEEP(*(.text.boot))   /* boot stub must come first        */
        *(.text)
        *(.text.*)
    }

    /* ---- Read-only data ---- */
    .rodata ALIGN(4K) :
    {
        *(.rodata)
        *(.rodata.*)
    }

    /* ---- Initialized data ---- */
    .data ALIGN(4K) :
    {
        *(.data)
        *(.data.*)
    }

    /* ---- BSS (zero-initialized) ---- */
    .bss ALIGN(4K) :
    {
        _bss_start = .;
        *(COMMON)
        *(.bss)
        *(.bss.*)
        _bss_end = .;
    }

    /* ---- End-of-kernel symbol ---- */
    _kernel_end = .;

    /* ---- Discard DWARF / debug / comments ---- */
    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.note*)
    }
}
