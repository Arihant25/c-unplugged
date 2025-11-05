#include "logger.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* log_file = NULL;
static char session_id[64];

int init_logger(const char* sid) {
    strncpy(session_id, sid, sizeof(session_id) - 1);
    session_id[sizeof(session_id) - 1] = '\0';
    
    log_file = fopen("data/command_history.log", "a");
    if (!log_file) {
        return -1;
    }
    return 0;
}

void close_logger(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_command(const char* command) {
    if (!log_file) return;
    
    char timestamp[64];
    get_current_timestamp(timestamp, sizeof(timestamp));
    
    fprintf(log_file, "[%s][%s] %s\n", timestamp, session_id, command);
    fflush(log_file);
}

void display_command_history(void) {
    FILE* file = fopen("data/command_history.log", "r");
    if (!file) {
        printf("No command history found.\n");
        return;
    }
    
    char line[512];
    printf("\n=== Command History ===\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("\n");
    
    fclose(file);
}
