#include "songs.h"
#include "albums.h"
#include "playlist.h"
#include "logger.h"
#include "cli.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    // Generate session ID
    char session_id[64];
    snprintf(session_id, sizeof(session_id), "%ld", (long)time(NULL));
    
    // Initialize logger
    if (init_logger(session_id) != 0) {
        printf("Warning: Could not initialize logger.\n");
    }
    
    printf("C-Unplugged Music Application\n");
    printf("Session ID: %s\n", session_id);
    
    // Load songs library
    printf("Loading songs library...\n");
    if (load_songs_from_file("data/songs_library.txt") == 0) {
        printf("Songs library loaded successfully.\n");
    }
    
    // Load albums
    printf("Loading albums...\n");
    load_albums_from_file("data/albums.dat");
    printf("Albums loaded.\n");
    
    // Initialize playlist
    init_playlist();
    
    // Run CLI loop
    run_cli_loop();
    
    // Save albums before exit
    printf("Saving albums...\n");
    if (save_albums_to_file("data/albums.dat") == 0) {
        printf("Albums saved successfully.\n");
    }
    
    // Cleanup
    free_songs_list();
    free_albums_list();
    free_playlist();
    close_logger();
    
    printf("Goodbye!\n");
    return 0;
}
