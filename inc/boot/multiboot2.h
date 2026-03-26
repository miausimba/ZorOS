#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MULTIBOOT2_HEADER_MAGIC 0xe85250d6
#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

#define MULTIBOOT2_TAG_TYPE_END 0
#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 6
#define MULTIBOOT2_TAG_TYPE_MEMORY_MAP 9
#define MULTIBOOT2_TAG_TYPE_MODULE 11

struct multiboot2_header {
  uint32_t magic;
  uint32_t flags;
  uint32_t checksum;
};

struct multiboot2_tag {
  uint32_t type;
  uint32_t size;
};

struct multiboot2_tag_memory_map_entry {
  uint64_t addr;
  uint64_t len;
  uint32_t type;
  uint32_t reserved;
};

struct multiboot2_tag_memory_map {
  uint32_t type;
  uint32_t size;
  uint32_t entry_size;
  uint32_t entry_version;
  struct multiboot2_tag_memory_map_entry entries[];
};

void multiboot2_process(uint64_t addr);

#endif