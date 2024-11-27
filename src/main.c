// this program is unfinished.
#include <time.h>
#include <stdlib.h>

typedef enum {lumos, noir} Theme;

extern void sample();
extern void triogons(Theme);

int main(int argc, char** argv) {
    srand(time(NULL));
    triogons(lumos);
    return 0;
}

