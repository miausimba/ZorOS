#ifndef POWEROFF_H
#define POWEROFF_H

// information for shutdown, in pc real for x86_64
#define PM1a_CNT 0x604
#define SLP_TYPa_S5 0x1
#define SLP_EN 0x2000

void poweroff(void);

#endif /* POWEROFF_H */