#include "../include/strings.h"
#include "../include/utils.h"

void sample() {
    String file = str_from(NULL);
    read_file_content("assets/sample_preset.svg", &file);

    size_t pos = 0;
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$color"), hsv_to_rgb(rand_range(0, 300), 0.2f, 0.8f)));
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$radius"), num_to_str(rand_range(35, 80))));
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$greeting"), get_quote()));

    write_to_file("out.svg", file);
    str_free(&file);
}
