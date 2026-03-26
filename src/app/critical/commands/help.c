/*

*/

#include <app/critical/commands/help.h>
#include <drivers/vga.h>

void help(void) {
  vga_write("Available commands:\n");
  vga_write("help - Show this message\n");
  vga_write("clear - Clear screen\n");
  vga_write("info - Show system info\n");
  vga_write("notepad - Open notepad\n");
  vga_write("poweroff - Shutdown ZorOS\n");
}