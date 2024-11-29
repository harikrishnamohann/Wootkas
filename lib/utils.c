#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../include/strings.h"
#include <math.h>

// Reads given character sequence file int *read_content.
// Returns 0 on success and -1 on failures.
int8_t read_file_content(const char* filename, String* read_content) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
      return -1;
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
      fclose(fp);
      return -1;
    }
    long file_size = ftell(fp);
    if (file_size == -1) {
        fclose(fp);
        return -1;
    }

    rewind(fp);

    read_content->length = 0;
    read_content->str = (char*)malloc(file_size + 1);
    if (read_content->str == NULL) {
        fclose(fp);
        return -1;
    }

    size_t read_size = fread(read_content->str, 1, file_size, fp);
    if (read_size != (size_t)file_size) {
        fclose(fp);
        return -1;
    }

    read_content->str[read_size] = '\0';
    read_content->length = read_size;

    fclose(fp);
    return 0;
}

// Writes given content to filename.
// returns 0 on success and -1 on failure.
int8_t write_to_file(const char* filename, const String content) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;    
    }
    
    size_t written = fwrite(content.str, sizeof(char), content.length, fp);
    fclose(fp);
    
    return (written == str_len(content)) ? 0 : -1;   
}

String greet() {
    time_t now;
    now = time(NULL);
    struct tm *local = localtime(&now);
    
    int hour = local->tm_hour;

    if (hour >= 5 && hour < 12) {
        return str_from("Good morning!");
    } else if (hour >= 12 && hour < 17) {
        return str_from("Good afternoon!");
    } else if (hour >= 17 && hour < 21) {
        return str_from("Good evening!");
    } else {
        return str_from("Good night!");
    }
    return str_from("oops! something happened!");
}

String get_quote() {
    char* quotes[] = {
        "Focus on progress, not perfection.",
        "Small steps every day lead to big results.",
        "Success is the sum of small efforts, repeated daily.",
        "Start where you are. Use what you have. Do what you can.",
        "Discipline is the bridge between goals and accomplishment.",
        "Don’t wish for it. Work for it.",
        "You don’t have to be perfect to be amazing.",
        "Dream big. Start small. Act now.",
        "Your future depends on what you do today.",
        "Push yourself, because no one else is going to do it for you."
    };
    
    int num_quotes = sizeof(quotes) / sizeof(quotes[0]);
    int index = rand() % num_quotes;
    return str_from(quotes[index]);
}

// returns a random number between x and y.
float rand_range(float x, float y) {
    if (x > y) {
        float temp = x;
        x = y;
        y = temp;
    }
    return x + ((float)rand() / RAND_MAX) * (y - x);
}

// matrix transformation functions.
typedef struct {float x; float y;} Point;

Point rand_point(Point begin, Point end) {
    return (Point) {rand_range(begin.x, end.x), rand_range(begin.y, end.y)};
}

// Function to compute the centroid of the shape
Point get_centroid(int row, int col, int C[][col]) {
    float sum_x = 0, sum_y = 0, count = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j += 2) { // Only x, y pairs
            sum_x += C[i][j];
            sum_y += C[i][j + 1];
            count++;
        }
    }
    return (Point) { sum_x / count, sum_y / count};
}

// Function to rotate a single point around a given origin
static void rotate_point(int *x, int *y, Point centroid, double angle_degrees) {
    double angle_radians = angle_degrees * M_PI / 180.0;
    double sin_angle = sin(angle_radians);
    double cos_angle = cos(angle_radians);

    int translated_x = *x - centroid.x;
    int translated_y = *y - centroid.y;

    // x′=x⋅cos(θ)−y⋅sin(θ)
    int rotated_x = translated_x * cos_angle - translated_y * sin_angle;
    // y′=x⋅sin(θ)+y⋅cos(θ)
    int rotated_y = translated_x * sin_angle + translated_y * cos_angle;

    *x = rotated_x + centroid.x;
    *y = rotated_y + centroid.y;
}

// Function to rotate the matrix around its center
void transform_rotate(int row, int col, int C[][col], double angle_degrees) {
    Point centroid = get_centroid(row, col, C);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j += 2) { // Only x, y pairs
            rotate_point(&C[i][j], &C[i][j + 1], centroid, angle_degrees);
        }
    }
}

// Scale the given matrix arount its center.
void transform_scale(int row, int col, int mat[][col], double scale_factor) {
    Point origin = get_centroid(row, col, mat);
    Point translated;

    // Scale each point relative to the centroid
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j += 2) { // Only x, y pairs
            // Translate point to origin and scale it.
            translated.x = (mat[i][j] - origin.x) * scale_factor;
            translated.y = (mat[i][j + 1] - origin.y) * scale_factor;

            // Translate the point back to its original position
            mat[i][j] = origin.x + translated.x;
            mat[i][j + 1] = origin.y + translated.y;
        }
    }
}
