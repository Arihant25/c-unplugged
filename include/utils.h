#ifndef UTILS_H
#define UTILS_H

#include <time.h>

// String processing
char* trim_whitespace(char* str);

// Time utilities
void get_current_timestamp(char* buffer, size_t size);

// Duration utilities
int parse_duration(const char* duration_str);
void format_duration(int seconds, char* buffer, size_t size);

// ID generation
int generate_id(void);

#endif // UTILS_H
