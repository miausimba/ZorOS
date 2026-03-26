#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KERNEL_NAME "ZorOS"
#define KERNEL_VERSION "0.0.1"
#define KERNEL_BUILD "01AA"
#define KERNEL_CODENAME "fish"
#define KERNEL_NAME_VERSION                                                    \
  KERNEL_NAME " " KERNEL_VERSION " (" KERNEL_CODENAME ")"
#define KERNEL_COMPILE_DATE __DATE__
#define KERNEL_COMPILE_TIME __TIME__
#define TEAM_NAME "ZorOS Team"

#define KERNEL_ARCH "x86_64"
#define KERNEL_MODE "64-bit"
#define KERNEL_TYPE "Monolithic"

#endif /* KERNEL_H */