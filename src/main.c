// this program is unfinished.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/strings.h"
#include "../include/utils.h"

static void sample_img() {
    String file = str_init(NULL);
    read_file_content("assets/sample_preset.svg", &file);
    while (str_replace_next(&file, str_init("$color"), str_init(hsvToRgb(get_random_range(0, 300), 0.2f, 0.8f))) == 0);
    while (str_replace_next(&file, str_init("$radius"), str_init(num_to_str(get_random_range(35, 80)))) == 0);
    while (str_replace_next(&file, str_init("$greeting"), str_init(get_quote())) == 0);
    write_to_file("out.svg", file);
    str_free(&file);
}

int main() {
    srand(time(NULL));
    sample_img();
    return 0;
}

