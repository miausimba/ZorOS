/*
    COPYRIGHT
*/

#include <app/critical/commands/info.h>
#include <drivers/vga.h>
#include <kernel/kernel.h>

void info(void) {
  vga_write("hello, this is zorOS\n");
  vga_write("this operating system is created with love <3\n");
  vga_write("--------------------------------\n");
  vga_write("information of the zorOS\n");
  vga_write("\n");
  vga_write("Name: " KERNEL_NAME "\n");
  vga_write("Version: " KERNEL_VERSION "\n");
  vga_write("Build: " KERNEL_BUILD "\n");
  vga_write("Codename: " KERNEL_CODENAME "\n");
  vga_write("Compile Date: " KERNEL_COMPILE_DATE "\n");
  vga_write("Compile Time: " KERNEL_COMPILE_TIME "\n");
  vga_write("Team: " TEAM_NAME "\n");
  vga_write("Architecture: " KERNEL_ARCH "\n");
  vga_write("--------------------------------\n");
}