# =============================================================
#  ZorOS – Professional Build System
#  Architecture: x86_64
#  Outputs:      build/zoros.elf, bin/zoros.iso
# =============================================================

# ──────────────────────────────────────────────────────────────
# 1. Configuration & Toolchain
# ──────────────────────────────────────────────────────────────
TARGET_ARCH := x86_64

# Toolchain prefixes (change to x86_64-elf- if using a cross-compiler)
PREFIX      := x86_64-linux-gnu-
CC          := $(PREFIX)gcc
LD          := $(PREFIX)ld
AS          := nasm
OBJCOPY     := $(PREFIX)objcopy
SIZE        := $(PREFIX)size
QEMU        := qemu-system-$(TARGET_ARCH)

# ── Directories ──
SRC_DIR     := src
INC_DIR     := inc
BUILD_DIR   := build
BIN_DIR     := bin
ISO_DIR     := iso

BOOT_DIR    := $(SRC_DIR)/boot
KERNEL_DIR  := $(SRC_DIR)/kernel
DRIVER_DIR  := $(SRC_DIR)/drivers
APP_DIR     := $(SRC_DIR)/app
APP_CRITICAL_DIR := $(APP_DIR)/critical
APP_INTERNAL_DIR := $(APP_CRITICAL_DIR)/internal
APP_COMMANDS_DIR := $(APP_CRITICAL_DIR)/commands
APP_SYSTEM_DIR := $(APP_DIR)/system
APP_SYSTEM_PROGRAMS_DIR := $(APP_SYSTEM_DIR)/programs
APP_GAMES_DIR := $(APP_DIR)/games

# ──────────────────────────────────────────────────────────────
# 2. Compilation Flags
# ──────────────────────────────────────────────────────────────

# Warnings & Standards
WARN_FLAGS  := -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wshadow
STD_FLAGS   := -std=c11 -ffreestanding

# Optimization & Debug
OPT_FLAGS   := -O2 -g

# Kernel Specific (No standard libraries, no red zone, no PIE)
KERNEL_FLAGS := -I$(abspath $(INC_DIR)) -MMD -MP \
                -mno-red-zone \
                -fno-stack-protector \
                -fno-builtin \
                -fno-pie \
                -fno-pic \
                -mcmodel=kernel \
                -mno-sse \
                -mno-mmx \
                -mno-80387

# Include Paths & Dependency Generation
# -MMD -MP generates .d files for header tracking
CPPFLAGS    := -I$(INC_DIR) -MMD -MP

CFLAGS      := $(STD_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(KERNEL_FLAGS)
ASFLAGS     := -f elf64
LDFLAGS     := -T $(KERNEL_DIR)/linker.ld -nostdlib -z max-page-size=0x1000 -no-pie

# ── QEMU Flags ──
QEMU_FLAGS  := -cdrom $(BIN_DIR)/zoros.iso \
               -m 512M \
               -serial stdio \
               -vga std \
               -no-reboot \
               -no-shutdown

# ──────────────────────────────────────────────────────────────
# 3. Source Discovery & Object Mapping
# ──────────────────────────────────────────────────────────────

# Discovery
# Discovery using recursive search
ALL_C_SRCS  := $(shell find $(SRC_DIR) -name "*.c")
ALL_ASM_SRCS := $(shell find $(SRC_DIR) -name "*.asm")

# Map to build directory objects
ALL_C_OBJS   := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(ALL_C_SRCS))
ALL_ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ALL_ASM_SRCS))

ALL_OBJS     := $(ALL_C_OBJS) $(ALL_ASM_OBJS)

# Dependency files
DEPS        := $(ALL_OBJS:.o=.d)

KERNEL_ELF  := $(BUILD_DIR)/zoros.elf
KERNEL_ISO  := $(BIN_DIR)/zoros.iso

# ──────────────────────────────────────────────────────────────
# 4. Build Rules (The Output Control)
# ──────────────────────────────────────────────────────────────

# Quiet mode support (use V=1 for verbose)
ifeq ($(V),1)
  Q :=
else
  Q := @
endif

.PHONY: all
all: $(KERNEL_ISO)

# Link the kernel
$(KERNEL_ELF): $(ALL_OBJS)
	@echo " [LD]   $@"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(LD) $(LDFLAGS) -o $@ $^

# Build the ISO
$(KERNEL_ISO): $(KERNEL_ELF)
	@echo " [ISO]  $@"
	$(Q)mkdir -p $(ISO_DIR)/boot/grub
	$(Q)cp $(KERNEL_ELF) $(ISO_DIR)/boot/zoros.elf
	$(Q)printf 'set timeout=0\nset default=0\n\nmenuentry "ZorOS" {\n\tmultiboot2 /boot/zoros.elf\n\tboot\n}\n' > $(ISO_DIR)/boot/grub/grub.cfg
	$(Q)mkdir -p $(BIN_DIR)
	$(Q)grub-mkrescue -o $@ $(ISO_DIR) 2>/dev/null || grub2-mkrescue -o $@ $(ISO_DIR)

# Compile C files (generic rule)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo " [CC]   $<"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

# Assemble ASM files (generic rule)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@echo " [ASM]  $@"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(AS) $(ASFLAGS) -o $@ $<

# ──────────────────────────────────────────────────────────────
# 5. Utility Targets
# ──────────────────────────────────────────────────────────────

.PHONY: run
run: $(KERNEL_ISO)
	@echo " [RUN]  Booting ZorOS..."
	$(Q)$(QEMU) $(QEMU_FLAGS)

.PHONY: debug
debug: $(KERNEL_ISO)
	@echo " [DEBUG] Waiting for GDB connection..."
	$(Q)$(QEMU) $(QEMU_FLAGS) -s -S

.PHONY: size
size: $(KERNEL_ELF)
	$(Q)$(SIZE) $<

.PHONY: clean
clean:
	@echo " [CLEAN] Removing artifacts..."
	$(Q)rm -rf $(BUILD_DIR) $(BIN_DIR) $(ISO_DIR)

# Include dependency files if they exist
-include $(DEPS)

.PHONY: help
help:
	@echo "ZorOS Makefile Help"
	@echo "--------------------"
	@echo "make        - Build the bootable ISO"
	@echo "make run    - Run the ISO in QEMU"
	@echo "make debug  - Run in QEMU with GDB stub"
	@echo "make clean  - Remove all build files"
	@echo "make size   - Show kernel section sizes"
	@echo ""
	@echo "Use V=1 for verbose output."
