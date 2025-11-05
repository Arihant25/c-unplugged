#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static int id_counter = 1;

char* trim_whitespace(char* str) {
    char* end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0) return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    return str;
}

void get_current_timestamp(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

int parse_duration(const char* duration_str) {
    int minutes = 0, seconds = 0;
    if (sscanf(duration_str, "%d:%d", &minutes, &seconds) == 2) {
        return minutes * 60 + seconds;
    }
    return 0;
}

void format_duration(int seconds, char* buffer, size_t size) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    snprintf(buffer, size, "%d:%02d", minutes, secs);
}

int generate_id(void) {
    return id_counter++;
}
