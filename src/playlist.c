#include "playlist.h"
#include "albums.h"
#include "songs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

PlaylistNode* g_playlist_head = NULL;
PlaylistNode* g_current_playing = NULL;

void init_playlist(void) {
    g_playlist_head = NULL;
    g_current_playing = NULL;
}

void add_song_to_playlist(int song_id) {
    SongNode* song = find_song_by_id(song_id);
    if (!song) {
        printf("Song with ID %d not found.\n", song_id);
        return;
    }
    
    PlaylistNode* node = (PlaylistNode*)malloc(sizeof(PlaylistNode));
    if (!node) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    node->song = song;
    node->play_count = 0;
    
    if (!g_playlist_head) {
        // First node - circular list points to itself
        node->next = node;
        g_playlist_head = node;
        g_current_playing = node;
    } else {
        // Find last node
        PlaylistNode* current = g_playlist_head;
        while (current->next != g_playlist_head) {
            current = current->next;
        }
        
        // Insert new node and maintain circular structure
        node->next = g_playlist_head;
        current->next = node;
    }
    
    printf("Song added to playlist.\n");
}

void add_album_to_playlist(int album_id) {
    AlbumNode* album = find_album_by_id(album_id);
    if (!album) {
        printf("Album with ID %d not found.\n", album_id);
        return;
    }
    
    if (!album->songs) {
        printf("Album has no songs.\n");
        return;
    }
    
    AlbumSongNode* current = album->songs;
    int count = 0;
    while (current) {
        add_song_to_playlist(current->song_id);
        count++;
        current = current->next;
    }
    
    printf("Added %d songs from album to playlist.\n", count);
}

void remove_song_from_playlist(int index) {
    if (!g_playlist_head) {
        printf("Playlist is empty.\n");
        return;
    }
    
    // Count playlist size
    int size = 1;
    PlaylistNode* current = g_playlist_head;
    while (current->next != g_playlist_head) {
        size++;
        current = current->next;
    }
    
    if (index < 1 || index > size) {
        printf("Invalid index. Playlist has %d songs.\n", size);
        return;
    }
    
    // Special case: single song
    if (size == 1) {
        free(g_playlist_head);
        g_playlist_head = NULL;
        g_current_playing = NULL;
        printf("Song removed from playlist.\n");
        return;
    }
    
    // Find node to remove
    PlaylistNode* prev = NULL;
    current = g_playlist_head;
    for (int i = 1; i < index; i++) {
        prev = current;
        current = current->next;
    }
    
    // Remove node and maintain circular structure
    if (current == g_playlist_head) {
        // Removing head
        PlaylistNode* last = g_playlist_head;
        while (last->next != g_playlist_head) {
            last = last->next;
        }
        last->next = g_playlist_head->next;
        
        if (g_current_playing == g_playlist_head) {
            g_current_playing = g_playlist_head->next;
        }
        
        PlaylistNode* old_head = g_playlist_head;
        g_playlist_head = g_playlist_head->next;
        free(old_head);
    } else {
        prev->next = current->next;
        
        if (g_current_playing == current) {
            g_current_playing = current->next;
        }
        
        free(current);
    }
    
    printf("Song removed from playlist.\n");
}

void play_next(void) {
    if (!g_current_playing) {
        printf("Playlist is empty.\n");
        return;
    }
    
    g_current_playing = g_current_playing->next;
    g_current_playing->play_count++;
    
    printf("Playing: %s by %s\n", 
           g_current_playing->song->title, 
           g_current_playing->song->artist);
}

void play_previous(void) {
    if (!g_current_playing) {
        printf("Playlist is empty.\n");
        return;
    }
    
    // Find previous node in circular list
    PlaylistNode* prev = g_playlist_head;
    while (prev->next != g_current_playing) {
        prev = prev->next;
    }
    
    g_current_playing = prev;
    g_current_playing->play_count++;
    
    printf("Playing: %s by %s\n", 
           g_current_playing->song->title, 
           g_current_playing->song->artist);
}

SongNode* get_current_song(void) {
    if (!g_current_playing) {
        return NULL;
    }
    return g_current_playing->song;
}

void display_playlist(void) {
    if (!g_playlist_head) {
        printf("Playlist is empty.\n");
        return;
    }
    
    printf("\n=== Current Playlist ===\n");
    printf("%-5s %-30s %-30s %-10s %-6s\n", "Index", "Title", "Artist", "Duration", "Plays");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    
    PlaylistNode* current = g_playlist_head;
    int index = 1;
    do {
        char duration_str[32];
        format_duration(current->song->duration, duration_str, sizeof(duration_str));
        
        char marker = (current == g_current_playing) ? '*' : ' ';
        printf("%c%-4d %-30s %-30s %-10s %-6d\n", 
               marker, index, current->song->title, 
               current->song->artist, duration_str, 
               current->play_count);
        
        current = current->next;
        index++;
    } while (current != g_playlist_head);
    
    printf("\n* = Currently playing\n\n");
}

void clear_playlist(void) {
    if (!g_playlist_head) {
        return;
    }
    
    PlaylistNode* current = g_playlist_head;
    PlaylistNode* start = g_playlist_head;
    
    do {
        PlaylistNode* next = current->next;
        free(current);
        current = next;
    } while (current != start);
    
    g_playlist_head = NULL;
    g_current_playing = NULL;
    
    printf("Playlist cleared.\n");
}

void free_playlist(void) {
    clear_playlist();
}
