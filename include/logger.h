#ifndef LOGGER_H
#define LOGGER_H

// Logger initialization and cleanup
int init_logger(const char* session_id);
void close_logger(void);

// Logging operations
void log_command(const char* command);
void display_command_history(void);

#endif // LOGGER_H
