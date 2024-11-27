// this program is unfinished.
#include <time.h>
#include <stdlib.h>

typedef enum {Lumos, Noir} Theme;

extern void sample();
extern void triogons(Theme);

int main(int argc, char** argv) {
    srand(time(NULL));
    triogons(Lumos);
    return 0;
}

