// this program is unfinished.
#include <time.h>
#include <stdlib.h>

typedef enum {Lumo, Noir} Theme;

extern void sample();
extern void triogons(Theme);

int main(int argc, char** argv) {
    srand(time(NULL));
    triogons(Lumo);
    return 0;
}

