#include "../include/strings.h"
#include "../include/utils.h"

void sample() {
    String file = str_from(NULL);
    read_file_content("assets/sample.preset", &file);

    size_t pos = 0;

    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$color"), str_compose("hsl(%f,%d\%,%d\%)", rand_range(0, 360), (int)rand_range(15, 50), (int)rand_range(68, 86))));
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$radius"), str_compose("%d", (int)rand_range(35, 80))));
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$greeting"), get_quote()));

    write_to_file("out.svg", file);
    str_free(&file);
}
