#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "songs.h"

typedef struct PlaylistNode {
    SongNode* song;
    int play_count;
    struct PlaylistNode* next;
} PlaylistNode;

// Global playlist
extern PlaylistNode* g_playlist_head;
extern PlaylistNode* g_current_playing;

// Playlist operations
void init_playlist(void);
void add_song_to_playlist(int song_id);
void add_album_to_playlist(int album_id);
void remove_song_from_playlist(int index);
void play_next(void);
void play_previous(void);
SongNode* get_current_song(void);
void display_playlist(void);
void clear_playlist(void);
void free_playlist(void);

#endif // PLAYLIST_H
