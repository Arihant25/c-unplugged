#include "albums.h"
#include "songs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AlbumNode* g_albums_head = NULL;

int load_albums_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0; // File might not exist yet, which is fine
    }
    
    char line[1024];
    AlbumNode* tail = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        AlbumNode* node = (AlbumNode*)malloc(sizeof(AlbumNode));
        if (!node) {
            fclose(file);
            return -1;
        }
        
        char song_ids[512] = {0};
        if (sscanf(line, "%d|%255[^|]|%255[^|]|%d|%511[^\n]",
                   &node->album_id, node->album_name, node->artist, 
                   &node->year, song_ids) >= 4) {
            
            node->songs = NULL;
            node->next = NULL;
            
            // Parse song IDs
            if (strlen(song_ids) > 0) {
                char* token = strtok(song_ids, ",");
                AlbumSongNode* song_tail = NULL;
                
                while (token) {
                    AlbumSongNode* song_node = (AlbumSongNode*)malloc(sizeof(AlbumSongNode));
                    if (song_node) {
                        song_node->song_id = atoi(token);
                        song_node->next = NULL;
                        
                        if (!node->songs) {
                            node->songs = song_node;
                            song_tail = song_node;
                        } else {
                            song_tail->next = song_node;
                            song_tail = song_node;
                        }
                    }
                    token = strtok(NULL, ",");
                }
            }
            
            if (!g_albums_head) {
                g_albums_head = node;
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

int save_albums_to_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        return -1;
    }
    
    AlbumNode* current = g_albums_head;
    while (current) {
        fprintf(file, "%d|%s|%s|%d|", 
                current->album_id, current->album_name, 
                current->artist, current->year);
        
        AlbumSongNode* song = current->songs;
        int first = 1;
        while (song) {
            if (!first) fprintf(file, ",");
            fprintf(file, "%d", song->song_id);
            first = 0;
            song = song->next;
        }
        fprintf(file, "\n");
        
        current = current->next;
    }
    
    fclose(file);
    return 0;
}

void create_album(const char* name, const char* artist, int year) {
    AlbumNode* node = (AlbumNode*)malloc(sizeof(AlbumNode));
    if (!node) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    node->album_id = generate_id();
    strncpy(node->album_name, name, sizeof(node->album_name) - 1);
    node->album_name[sizeof(node->album_name) - 1] = '\0';
    strncpy(node->artist, artist, sizeof(node->artist) - 1);
    node->artist[sizeof(node->artist) - 1] = '\0';
    node->year = year;
    node->songs = NULL;
    node->next = NULL;
    
    if (!g_albums_head) {
        g_albums_head = node;
    } else {
        AlbumNode* current = g_albums_head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
    
    printf("Album created successfully with ID: %d\n", node->album_id);
}

void delete_album(int album_id) {
    AlbumNode* current = g_albums_head;
    AlbumNode* prev = NULL;
    
    while (current) {
        if (current->album_id == album_id) {
            if (prev) {
                prev->next = current->next;
            } else {
                g_albums_head = current->next;
            }
            
            // Free song list
            AlbumSongNode* song = current->songs;
            while (song) {
                AlbumSongNode* next = song->next;
                free(song);
                song = next;
            }
            
            free(current);
            printf("Album deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    
    printf("Album with ID %d not found.\n", album_id);
}

void list_all_albums(void) {
    if (!g_albums_head) {
        printf("No albums found.\n");
        return;
    }
    
    printf("\n=== Albums ===\n");
    printf("%-5s %-30s %-30s %-6s %-10s\n", "ID", "Album Name", "Artist", "Year", "Songs");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    
    AlbumNode* current = g_albums_head;
    while (current) {
        int song_count = 0;
        AlbumSongNode* song = current->songs;
        while (song) {
            song_count++;
            song = song->next;
        }
        
        printf("%-5d %-30s %-30s %-6d %-10d\n", 
               current->album_id, current->album_name, 
               current->artist, current->year, song_count);
        current = current->next;
    }
    printf("\n");
}

void add_song_to_album(int song_id, int album_id) {
    AlbumNode* album = find_album_by_id(album_id);
    if (!album) {
        printf("Album with ID %d not found.\n", album_id);
        return;
    }
    
    SongNode* song = find_song_by_id(song_id);
    if (!song) {
        printf("Song with ID %d not found.\n", song_id);
        return;
    }
    
    // Check if song already in album
    AlbumSongNode* current = album->songs;
    while (current) {
        if (current->song_id == song_id) {
            printf("Song already in this album.\n");
            return;
        }
        current = current->next;
    }
    
    AlbumSongNode* node = (AlbumSongNode*)malloc(sizeof(AlbumSongNode));
    if (!node) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    node->song_id = song_id;
    node->next = album->songs;
    album->songs = node;
    
    printf("Song added to album successfully.\n");
}

void remove_song_from_album(int song_id, int album_id) {
    AlbumNode* album = find_album_by_id(album_id);
    if (!album) {
        printf("Album with ID %d not found.\n", album_id);
        return;
    }
    
    AlbumSongNode* current = album->songs;
    AlbumSongNode* prev = NULL;
    
    while (current) {
        if (current->song_id == song_id) {
            if (prev) {
                prev->next = current->next;
            } else {
                album->songs = current->next;
            }
            free(current);
            printf("Song removed from album successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    
    printf("Song with ID %d not found in album.\n", song_id);
}

void view_album_songs(int album_id) {
    AlbumNode* album = find_album_by_id(album_id);
    if (!album) {
        printf("Album with ID %d not found.\n", album_id);
        return;
    }
    
    printf("\n=== Album: %s by %s (%d) ===\n", 
           album->album_name, album->artist, album->year);
    
    if (!album->songs) {
        printf("No songs in this album.\n\n");
        return;
    }
    
    printf("%-5s %-30s %-30s %-10s\n", "ID", "Title", "Artist", "Duration");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    
    AlbumSongNode* current = album->songs;
    while (current) {
        SongNode* song = find_song_by_id(current->song_id);
        if (song) {
            char duration_str[32];
            format_duration(song->duration, duration_str, sizeof(duration_str));
            printf("%-5d %-30s %-30s %-10s\n", 
                   song->song_id, song->title, song->artist, duration_str);
        }
        current = current->next;
    }
    printf("\n");
}

AlbumNode* find_album_by_id(int album_id) {
    AlbumNode* current = g_albums_head;
    while (current) {
        if (current->album_id == album_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_albums_list(void) {
    AlbumNode* current = g_albums_head;
    while (current) {
        AlbumNode* next = current->next;
        
        // Free song list
        AlbumSongNode* song = current->songs;
        while (song) {
            AlbumSongNode* song_next = song->next;
            free(song);
            song = song_next;
        }
        
        free(current);
        current = next;
    }
    g_albums_head = NULL;
}
