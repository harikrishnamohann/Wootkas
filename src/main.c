// this program is unfinished.
#include <stdio.h>
#include <time.h>
#include "../include/strings.h"
#include "../include/utils.h"

static void sample_img() {
    String file = str_from(NULL);
    read_file_content("assets/sample_preset.svg", &file);

    size_t pos = 0;
    while ((pos = str_replace_next(&file, pos, str_from("$color"), hsv_to_rgb(get_random_range(0, 300), 0.2f, 0.8f))) != -1);
    pos = 0;
    while ((pos = str_replace_next(&file, pos, str_from("$radius"), num_to_str(get_random_range(35, 80)))) != -1);
    pos = 0;
    while ((pos = str_replace_next(&file, pos, str_from("$greeting"), get_quote())) != -1);

    write_to_file("out.svg", file);
    str_free(&file);
}

int main() {
    srand(time(NULL));
    sample_img();
    return 0;
}

