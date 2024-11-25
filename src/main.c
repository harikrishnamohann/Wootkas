// this program is unfinished.
#include <stdio.h>
#include <time.h>
#include "../include/strings.h"
#include "../include/utils.h"

static String create_triogon(Point origin);

static void sample_img() {
    String file = str_from(NULL);
    read_file_content("assets/sample_preset.svg", &file);

    size_t pos = 0;
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$color"), hsv_to_rgb(rand_range(0, 300), 0.2f, 0.8f)));
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$radius"), num_to_str(rand_range(35, 80))));
    for (pos = 0; pos != -1; pos = str_replace_next(&file, pos, str_from("$greeting"), get_quote()));

    write_to_file("out.svg", file);
    str_free(&file);
}

static void triogons() {
    String file = str_from(NULL);
    read_file_content("./assets/triogons.preset", &file);

    int number_of_triogons =  16;
    Point dimensions = {1920 - 80, 1080 - 80};
    Point tmp;

    String triogon = str_from(NULL);

    for (int i = 0; i < number_of_triogons; i++) {
        if (str_replace_all(&file, str_from("$TRIOGONS"), str_from("$TRIOGONS\n$TRIOGONS")) == 0) {
            tmp = rand_point((Point){80,80}, dimensions);;
            triogon = create_triogon(tmp);
            str_replace_next(&file, 0, str_from("$TRIOGONS"), triogon);
            str_free(&triogon);
        }
    }
    str_replace_next(&file, 0, str_from("$TRIOGONS"), str_from(""));
    
    write_to_file("out.svg", file);

    str_free(&file);
}

int main() {
    srand(time(NULL));
    triogons();
    return 0;
}

static String create_triogon(Point origin) {
    int C[3][6]; // to store the curve points.
    // I am using only 3 points it is called a triogon though.

    // Adjust these points to tweak the shape's character.
    // you can see some interesting behaviours.. just play with it
    // and find your harmony.
    Point position_range = {0, 5};
    Point anchor_range = {2, 6};

    // initializing the shape.
    C[2][4] = origin.x; C[2][5] = origin.y;
    C[0][4] = origin.x + 5 + rand_range(position_range.x, position_range.y);
    C[0][5] = origin.y + 10 + rand_range(position_range.x, position_range.y);
    C[1][4] = origin.x - 5 + rand_range(position_range.x, position_range.y);
    C[1][5] = origin.y + 10 + rand_range(position_range.x, position_range.y);

    C[0][0] = origin.x + rand_range(anchor_range.x, anchor_range.y);
    C[0][1] = origin.y - rand_range(anchor_range.x, anchor_range.y);
    C[0][2] = C[0][4] + rand_range(anchor_range.x, anchor_range.y);
    C[0][3] = C[0][5] - rand_range(anchor_range.x, anchor_range.y);

    C[1][0] = C[0][4] - rand_range(anchor_range.x, anchor_range.y);
    C[1][1] = C[0][5] + rand_range(anchor_range.x, anchor_range.y);
    C[1][2] = C[1][4] + rand_range(anchor_range.x, anchor_range.y);
    C[1][3] = C[1][5] + rand_range(anchor_range.x, anchor_range.y);

    C[2][0] = C[1][4] - rand_range(anchor_range.x, anchor_range.y);
    C[2][1] = C[1][5] - rand_range(anchor_range.x, anchor_range.y);
    C[2][2] = C[2][4] - rand_range(anchor_range.x, anchor_range.y);
    C[2][3] = C[2][5] + rand_range(anchor_range.x, anchor_range.y);

    // more entropy
    transform_rotate(3, 6, C, rand_range(0.0f, 360.0f));
    transform_scale(3, 6, C, rand_range(12.0f, 16.0f));

    // now we need to make an svg <path/> attribute with the data we have. 
    String style = str_from("\"fill:rgb(000,000,000);stroke:none;fill-opacity:1\"");
    str_replace_next(&style, 0, str_from("rgb(000,000,000)"), hsv_to_rgb(rand_range(0, 300), 0.2f, 0.76f));

    String d = str_from(NULL);
    if ((d.str = (char*)malloc(150)) == NULL) {
        DEBUG_PRINT("err: create_triogons(): failed to allocate memory for d.str.\n");
        exit(-1);
    }
    d.length = sprintf(d.str, "\"M %d,%d C %d,%d %d,%d %d,%d C %d,%d %d,%d %d,%d C %d,%d %d,%d %d,%d Z\"", 
            C[2][4], C[2][5],
            C[0][0], C[0][1], C[0][2], C[0][3], C[0][4], C[0][5],
            C[1][0], C[1][1], C[1][2], C[1][3], C[1][4], C[1][5],
            C[2][0], C[2][1], C[2][2], C[2][3], C[2][4], C[2][5]
        );
    d.length = strlen(d.str);

    String path = str_from(NULL);
    if ((path.str = (char *)malloc(style.length + d.length + 30)) == NULL) {
        DEBUG_PRINT("err: create_triogons(): failed to allocate memory for path.str.\n");
        exit(-1);
    }
    path.length = sprintf(path.str, "<path style=%s d=%s/>", style.str, d.str);

    str_free(&style);
    str_free(&d);

    return path;
}
