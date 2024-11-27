/*
This file defines functions to dynamically create a collection of triogons (abstract shapes) 
based on Bézier curves—and write them into an SVG file. The shapes are generated using 
random transformations (positioning, scaling, rotation) to ensure visual variability.

Functions:
    * `create_triogon(Point origin)`: Generates a single triogon with randomized attributes.
        - origin: Starting point of a triogon

    * `triogons(Theme theme)`: generate multiple triogons and writes the SVG file.
        - theme: (Lumo or Noir) choose whether to use light(Lumo) or dark(Noir) style.
*/
#include "../include/strings.h"
#include "../include/utils.h"

#define CANVAS_WIDTH 1920
#define CANVAS_HEIGHT 1080
#define PADDING_X 100
#define PADDING_Y 100
#define SCALE 1.0
// common divisor is used to maintain relative scaling on other resolutions.
// 120 is the common divisor of 1920 x 1080
#define COMMON_DIVISOR 120

// DENSITY determines the number of triogons to create.
#define DENSITY rand_range(18, 30)

// light and dark theme
typedef enum {Lumo, Noir} Theme;
#define LUMO str_from("#E5E5E5")
#define NOIR str_from("#212121")

/**
 * @brief Creates a triogon SVG path based on the given origin point.
 *
 * A triogon is an orgainc shape created using three control points. This function initializes
 * the control points, applies random transformations, and constructs an SVG path tag.
 *
 * @param origin: The origin point where the triogon starts.
 * @return A string containing the SVG path for the triogon.
 */
static String create_triogon(Point origin) {
    int C[3][6]; // to store the control points of beziere curve.

    // Adjust the values below to tweak the shape's character.
    // you can see some interesting behaviours.. just play with it
    // and find your harmony.
    
    // Range for positioning and control point adjustments.
    const Point pos_range = {5, 8};
    const Point anchor_range = {2, 8};
    const Point shift = {6.5, 10};

    // transformation values
    const float rotation = rand_range(0.0f, 360.0f);
    const float scale = rand_range(CANVAS_HEIGHT, CANVAS_WIDTH) / COMMON_DIVISOR * SCALE;

    // Random color generation using HLSA format.
    const float hue = rand_range(0, 360);
    const int saturation = (int)rand_range(30, 65);
    const int lightness = (int)rand_range(55, 85);
    const float alpha = 0.75; // For some transparancy.

    // creating the shape with entropy.
    C[2][4] = origin.x;
    C[2][5] = origin.y;
    C[0][4] = origin.x + shift.x + rand_range(pos_range.x, pos_range.y);
    C[0][5] = origin.y + shift.y + rand_range(pos_range.x, pos_range.y);
    C[1][4] = origin.x - shift.x + rand_range(pos_range.x, pos_range.y);
    C[1][5] = origin.y + shift.y + rand_range(pos_range.x, pos_range.y);

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
    transform_rotate(3, 6, C, rotation);
    transform_scale(3, 6, C, scale);

    // now we need to make an svg <path/> attribute with the data we have. 
    String color = str_compose("hsla(%f,%d\%,%d\%,%f)", hue, saturation, lightness, alpha);
    String d = str_compose("\"M %d,%d C %d,%d %d,%d %d,%d C %d,%d %d,%d %d,%d C %d,%d %d,%d %d,%d Z\"", 
            C[2][4], C[2][5],
            C[0][0], C[0][1], C[0][2], C[0][3], C[0][4], C[0][5],
            C[1][0], C[1][1], C[1][2], C[1][3], C[1][4], C[1][5],
            C[2][0], C[2][1], C[2][2], C[2][3], C[2][4], C[2][5]
        );
    String path = str_compose("<path style=\"fill:%s;stroke:none;fill-opacity:1\" d=%s/>", color.str, d.str);

    str_free(&color);
    str_free(&d);

    return path;
}

/**
 * @brief Generates a complete SVG file with multiple triogons based on a preset template.
 *
 * This function:
 * - Reads a base SVG template from `triogons.preset`.
 * - Replaces placeholders like `$CANVAS_WIDTH`, `$CANVAS_HEIGHT`, and `$TRIOGONS` dynamically.
 * - Populates the SVG with randomized triogons.
 *
 * @param theme The visual theme (Lumo or Noir).
 */
void triogons(Theme theme) {
    String file = str_from(NULL);
    if (read_file_content("./assets/triogons.preset", &file) != 0) {
        DEBUG_PRINT("Err: triogons(): error while accessing file\n");
        return;
    }
    const Point padding = {PADDING_X, PADDING_Y};
    const Point padded_dimensions = {CANVAS_WIDTH - padding.x, CANVAS_HEIGHT - padding.y};

    String tmp = str_compose("%d", CANVAS_HEIGHT);
    str_replace_all(&file, str_from("$CANVAS_HEIGHT"), tmp);
    str_free(&tmp);
    tmp = str_compose("%d", CANVAS_WIDTH);
    str_replace_all(&file, str_from("$CANVAS_WIDTH"), tmp);
    str_free(&tmp);

    size_t pos = str_replace_next(&file, 0, str_from("$THEME"), (theme == Noir) ? NOIR : LUMO);
    for (uint16_t i = 0; i < DENSITY; i++) {
        tmp = create_triogon(rand_point(padding, padded_dimensions));
        str_replace_next(&file, pos, str_from("$TRIOGONS"), str_from("$TRIOGONS\n$TRIOGONS"));
        pos = str_replace_next(&file, pos, str_from("$TRIOGONS"), tmp);
        str_free(&tmp);
    }
    str_replace_next(&file, pos, str_from("$TRIOGONS"), str_from(""));

    write_to_file("out.svg", file);
    str_free(&file);
}
