#ifndef INC_DINO_H_
#define INC_DINO_H_

#include "main.h"
#include "oled.h"
#include "function.h"

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

void play(void);
void introMessage(void);
void showLine(void);

#endif /* INC_DINO_H_ */
