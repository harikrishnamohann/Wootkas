#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "arena.h"

// to toggle error messages.
#define DEBUG 1

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
  do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)
#endif

typedef struct String String;

// Reads given character sequence file int *read_content.
// Returns 0 on success and -1 on failures.
int8_t read_file_content(const char* filename, String* read_content);

// Writes given content to filename.
// returns 0 on success and -1 on failure.
int8_t write_to_file(const char* filename, const String content);



// ### project specific definitions ###

String hsv_to_rgb(float h, float s, float v);
String greet();

// returns a random number between x and y;
float rand_range(float x, float y);
String get_quote();
String num_to_str(int num);

typedef struct {int x; int y;} Point;

// generates random point between begin and end.
Point rand_point(Point begin, Point end);

// Function to compute the centroid of the shape
Point get_centroid(int row, int col, int C[][col]);

// Function to rotate the shape around its center
void transform_rotate(int row, int col, int C[][col], double angle_degrees);

// Scale the given matrix arount its center.
void transform_scale(int row, int col, int mat[][col], double scale_factor);

#endif
