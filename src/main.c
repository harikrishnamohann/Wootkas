// this program is unfinished.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/strings.h"
#include "../include/utils.h"

char* hsvToRgb(float h, float s, float v);
char* greet(); // returns greeting message based on time of the day.
int get_random_range(int x, int y);
char* get_quote(); // returns random motivational quotes
char* num_to_str(int num);
static char* join_int_str(char* str, int num);

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

char* hsvToRgb(float h, float s, float v) {
    float red, green, blue;

    if (s == 0.0f) {
        red = green = blue = v;
    } else {
        float f, p, q, t;
        int i;

        h /= 60.0f;
        i = (int)h;
        f = h - i; 
        p = v * (1.0f - s);
        q = v * (1.0f - s * f);
        t = v * (1.0f - s * (1.0f - f));

        switch (i) {
            case 0:
                red = v; green = t; blue = p;
                break;
            case 1:
                red = q; green = v; blue = p;
                break;
            case 2:
                red = p; green = v; blue = t;
                break;
            case 3:
                red = p; green = q; blue = v;
                break;
            case 4:
                red = t; green = p; blue = v;
                break;
            default: 
                red = v; green = p; blue = q;
                break;
        }
    }
    int r = (int)(red * 255);
    int g = (int)(green * 255);
    int b = (int)(blue * 255);

    char* result = (char*)malloc(20);
    if (result != NULL) {
        sprintf(result, "rgb(%d,%d,%d)", r, g, b);
    }
    return result;
}
char* greet() {
    time_t now;
    now = time(NULL);
    struct tm *local = localtime(&now);
    
    int hour = local->tm_hour;

    if (hour >= 5 && hour < 12) {
        return "Good morning!";
    } else if (hour >= 12 && hour < 17) {
        return "Good afternoon!";
    } else if (hour >= 17 && hour < 21) {
        return "Good evening!";
    } else {
        return "Good night!";
    }
    return "oops! something happened!";
}

int get_random_range(int x, int y) {
    if (x > y) {
        int temp = x;
        x = y;
        y = temp;
    }
    return x + rand() % (y - x + 1);
}

char* get_quote() {
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
    srand(time(0));
    int index = rand() % num_quotes;
    return quotes[index];
}



void clear_buffer(char* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = '\0';
    }
}
char* num_to_str(int num) {
    char buff[20];
    sprintf(buff, "%d", num);
    return strdup(buff);
}

static char* join_int_str(char* str, int num) {
    String buf = str_init(NULL);
    buf.length = strlen(str) + 2;
    buf.str = malloc(buf.length);
    sprintf(buf.str, "%s%d", str, num);
    return buf.str;
}
