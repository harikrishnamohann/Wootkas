// this program is unfinished.
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {Lumo, Noir} Theme;

extern void sample();
extern void triogons(uint16_t width, uint16_t height, Theme theme);

int main(int argc, char** argv) {
    srand(time(NULL));
    triogons(1080, 2400, Noir);
    return 0;
}

