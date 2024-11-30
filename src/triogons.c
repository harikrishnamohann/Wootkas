/*
This file defines functions to dynamically create a collection of triogons (abstract shapes) 
based on Bézier curves—and write them into an SVG file. The shapes are generated using 
random transformations (positioning, scaling, rotation) to ensure visual diversity.

Functions:
    * `void create_triogon(origin)`: Generates a single triogon with randomized attributes.
        - origin: Starting point of a triogon

    * `void triogons(width, height, theme)`: generate multiple triogons and writes the SVG file.
        - width: width of the canvas
        - height: height of the canvas
        - theme: (Lumos or Noir) choose whether to use light(Lumos) or dark(Noir) style.
*/
#include "../include/strings.h"
#include "../include/utils.h"

// some customization options.
#define SCALE_FACTOR 1.3
#define DENSITY rand_range(12, 23)
// HSLA color:
#define HUE rand_range(0, 360)
#define SATURATION (int)rand_range(30, 65)
#define LIGHTNESS(theme) (int)((theme == Lumos) ? rand_range(46, 78) : rand_range(65, 95))
#define ALPHA(theme) (theme == Lumos) ? 0.72 : 0.8

// light and dark theme
typedef enum {Lumos, Noir} Theme;
#define LUMO str_from("#E5E5E5")
#define NOIR str_from("#121212")

static uint16_t CANVAS_WIDTH = 1920;
static uint16_t CANVAS_HEIGHT = 1080;
// common divisor is used to maintain relative scaling for other resolutions.
// 120 is the common divisor of 1920 x 1080.
// !It is not necessary to change it for different resolutions.
#define COMMON_DIVISOR 120

/**
 * @brief Creates a triogon SVG path based on the given origin point.
 *
 * A triogon is created using three control points. This function initializes
 * the control points, applies random transformations, and constructs an SVG path tag.
 *
 * @param origin: The origin point where the triogon starts.
 * @param theme: whether to use light or dark theme
 * @return A string containing the SVG path for the triogon.
 */
static String create_triogon(Point origin, Theme theme) {
    int C[3][6]; // to store the control points of beziere curve.

    // Range for positioning and control point adjustments.
    // The shape's fundamental character can be modified by changing these values.
    // play with it to find a sweet spot.
    const Point pos_range = {5, 8};
    const Point anchor_range = {2, 8};
    const Point shift = {6.5, 10};

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

    // more entropy using transformations.
    const float scale = rand_range(CANVAS_HEIGHT, CANVAS_WIDTH) / COMMON_DIVISOR * SCALE_FACTOR;
    transform_rotate(3, 6, C, rand_range(0, 360));
    transform_scale(3, 6, C, scale);

    // now we need to make an svg <path/> attribute with the data we have. 
    String color = str_compose("hsla(%f,%d\%,%d\%,%f)", HUE, SATURATION, LIGHTNESS(theme), ALPHA(theme));
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
 * @param width Define the width of the image.
 * @param height Define the height of the image.
 * @param theme The visual theme (Lumos or Noir).
 */
void triogons(uint16_t width, uint16_t height, Theme theme) {
    String file = str_from(NULL);
    if (read_file_content("./assets/triogons.preset", &file) != 0) {
        DEBUG_PRINT("Err: triogons(): error while accessing file\n");
        return;
    }

    if (height != 0 && width != 0) {
        CANVAS_HEIGHT = height;
        CANVAS_WIDTH = width;
    }
    const uint8_t padding_fac = 5; // this is obtained through trial and error.
    Point padding = {
        (float)CANVAS_WIDTH / COMMON_DIVISOR * padding_fac,
        (float)CANVAS_HEIGHT / COMMON_DIVISOR * padding_fac
    };
    Point origin;
    String tmp = str_compose("%d", CANVAS_HEIGHT);
    str_replace_all(&file, str_from("$CANVAS_HEIGHT"), tmp);
    str_free(&tmp);
    tmp = str_compose("%d", CANVAS_WIDTH);
    str_replace_all(&file, str_from("$CANVAS_WIDTH"), tmp);
    str_free(&tmp);

    size_t pos = str_replace_next(&file, 0, str_from("$THEME"), (theme == Noir) ? NOIR : LUMO);
    for (uint16_t i = 0; i < DENSITY; i++) {
        origin = rand_point(padding, (Point){CANVAS_WIDTH - padding.x, CANVAS_HEIGHT - padding.y});
        tmp = create_triogon(origin, theme);
        str_replace_next(&file, pos, str_from("$TRIOGONS"), str_from("$TRIOGONS\n$TRIOGONS"));
        pos = str_replace_next(&file, pos, str_from("$TRIOGONS"), tmp);
        str_free(&tmp);
    }
    str_replace_next(&file, pos, str_from("$TRIOGONS"), str_from(""));

    write_to_file("out.svg", file);
    str_free(&file);
}
