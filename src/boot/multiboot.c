#include <boot/multiboot2.h>
#include <drivers/vga.h>

#define MULTIBOOT2_HEADER_SIZE sizeof(struct multiboot2_header)

#define MULTIBOOT2_TAG_SIZE sizeof(struct multiboot2_tag)

#define MULTIBOOT2_TAG_TYPE_END 0

void multiboot2_process(uint64_t addr) {
  // esto es para parsear el multiboot2
  struct multiboot2_tag *tag = (struct multiboot2_tag *)(addr + 8);
  while (tag->type != MULTIBOOT2_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT2_TAG_TYPE_FRAMEBUFFER: {
      // read the framebuffer
      struct multiboot2_tag_framebuffer *framebuffer =
          (struct multiboot2_tag_framebuffer *)tag;
      vga_write("Framebuffer found\n");
      break;
    }
    case MULTIBOOT2_TAG_TYPE_MEMORY_MAP: {
      // read the memory map
      struct multiboot2_tag_memory_map *memory_map =
          (struct multiboot2_tag_memory_map *)tag;
      struct multiboot2_tag_memory_map_entry *entry = memory_map->entries;
      for (; (uint8_t *)entry < (uint8_t *)memory_map + memory_map->size;
           entry = (struct multiboot2_tag_memory_map_entry
                        *)((uint8_t *)entry + memory_map->entry_size)) {
        vga_write("Memory map entry found\n");
      }

      break;
    }
    case MULTIBOOT2_TAG_TYPE_MODULE: {
      // read the module
      struct multiboot2_tag_module *module =
          (struct multiboot2_tag_module *)tag;
      vga_write("Module found\n");
      break;
    }
    default: {
      vga_write("Unknown tag\n");
      break;
    }
    }
    tag = (struct multiboot2_tag *)(((uintptr_t)((uint8_t *)tag + tag->size +
                                                 7)) &
                                    ~((uintptr_t)7));
  }
}