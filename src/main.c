// this program is unfinished.
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {Lumos, Noir} Theme;

extern void sample();
extern void triogons(uint16_t width, uint16_t height, Theme theme);

int main(int argc, char** argv) {
    srand(time(NULL));
    triogons(1600, 900, Lumos);
    return 0;
}

