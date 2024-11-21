#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/strings.h"
#include <time.h>

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

char* join_int_str(char* str, int num) {
    String buf = str_init(NULL);
    buf.length = strlen(str) + 2;
    buf.str = malloc(buf.length);
    sprintf(buf.str, "%s%d", str, num);
    return buf.str;
}
