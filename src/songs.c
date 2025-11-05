#include "songs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SongNode* g_songs_head = NULL;

int load_songs_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Warning: Could not open songs library file: %s\n", filename);
        return -1;
    }
    
    char line[1024];
    SongNode* tail = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        // Skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#') continue;
        
        SongNode* node = (SongNode*)malloc(sizeof(SongNode));
        if (!node) {
            fclose(file);
            return -1;
        }
        
        char duration_str[32];
        if (sscanf(line, "%d|%255[^|]|%255[^|]|%255[^|]|%31[^|]|%511[^\n]",
                   &node->song_id, node->title, node->artist, 
                   node->album_name, duration_str, node->file_path) == 6) {
            
            node->duration = parse_duration(duration_str);
            node->next = NULL;
            
            if (!g_songs_head) {
                g_songs_head = node;
                tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
        } else {
            free(node);
        }
    }
    
    fclose(file);
    return 0;
}

void list_all_songs(void) {
    if (!g_songs_head) {
        printf("No songs in library.\n");
        return;
    }
    
    printf("\n=== Song Library ===\n");
    printf("%-5s %-30s %-30s %-30s %-10s\n", "ID", "Title", "Artist", "Album", "Duration");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    
    SongNode* current = g_songs_head;
    while (current) {
        char duration_str[32];
        format_duration(current->duration, duration_str, sizeof(duration_str));
        printf("%-5d %-30s %-30s %-30s %-10s\n", 
               current->song_id, current->title, current->artist, 
               current->album_name, duration_str);
        current = current->next;
    }
    printf("\n");
}

SongNode* find_song_by_id(int song_id) {
    SongNode* current = g_songs_head;
    while (current) {
        if (current->song_id == song_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_songs_list(void) {
    SongNode* current = g_songs_head;
    while (current) {
        SongNode* next = current->next;
        free(current);
        current = next;
    }
    g_songs_head = NULL;
}
