#ifndef ALBUMS_H
#define ALBUMS_H

typedef struct AlbumSongNode {
    int song_id;
    struct AlbumSongNode* next;
} AlbumSongNode;

typedef struct AlbumNode {
    int album_id;
    char album_name[256];
    char artist[256];
    int year;
    AlbumSongNode* songs;
    struct AlbumNode* next;
} AlbumNode;

// Global albums list
extern AlbumNode* g_albums_head;

// Album operations
int load_albums_from_file(const char* filename);
int save_albums_to_file(const char* filename);
void create_album(const char* name, const char* artist, int year);
void delete_album(int album_id);
void list_all_albums(void);
void add_song_to_album(int song_id, int album_id);
void remove_song_from_album(int song_id, int album_id);
void view_album_songs(int album_id);
AlbumNode* find_album_by_id(int album_id);
void free_albums_list(void);

#endif // ALBUMS_H
