#ifndef SONGS_H
#define SONGS_H

typedef struct SongNode {
    int song_id;
    char title[256];
    char artist[256];
    char album_name[256];
    int duration;
    char file_path[512];
    struct SongNode* next;
} SongNode;

// Global songs list
extern SongNode* g_songs_head;

// Song operations
int load_songs_from_file(const char* filename);
void list_all_songs(void);
SongNode* find_song_by_id(int song_id);
void free_songs_list(void);

#endif // SONGS_H
