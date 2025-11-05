#include "cli.h"
#include "songs.h"
#include "albums.h"
#include "playlist.h"
#include "logger.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_menu(void) {
    printf("\n=== C-Unplugged Music Application ===\n");
    printf("\nSong Management:\n");
    printf("  list songs                              - Display all songs\n");
    printf("  search song <id>                        - Find song by ID\n");
    printf("\nAlbum Management:\n");
    printf("  list albums                             - Display all albums\n");
    printf("  create album <name> <artist> <year>     - Create new album\n");
    printf("  delete album <id>                       - Remove album\n");
    printf("  view album <id>                         - Show songs in album\n");
    printf("  add song <song_id> to album <album_id>  - Add song to album\n");
    printf("  remove song <song_id> from album <album_id> - Remove song from album\n");
    printf("\nPlaylist Management:\n");
    printf("  add album <id> to playlist              - Add all album songs\n");
    printf("  add song <id> to playlist               - Add individual song\n");
    printf("  remove song <index> from playlist       - Remove from playlist\n");
    printf("  show playlist                           - Display current playlist\n");
    printf("  play next                               - Move to next song\n");
    printf("  play previous                           - Move to previous song\n");
    printf("  current song                            - Display currently playing\n");
    printf("  clear playlist                          - Empty playlist\n");
    printf("\nSystem Commands:\n");
    printf("  history                                 - Display command history\n");
    printf("  help                                    - Show this menu\n");
    printf("  exit                                    - Save and quit\n");
    printf("\n");
}

static void handle_list_command(const char* args) {
    if (strstr(args, "songs")) {
        list_all_songs();
    } else if (strstr(args, "albums")) {
        list_all_albums();
    } else {
        printf("Unknown list command. Use 'list songs' or 'list albums'.\n");
    }
}

static void handle_search_command(const char* args) {
    int song_id;
    if (sscanf(args, "song %d", &song_id) == 1) {
        SongNode* song = find_song_by_id(song_id);
        if (song) {
            char duration_str[32];
            format_duration(song->duration, duration_str, sizeof(duration_str));
            printf("\nSong ID: %d\n", song->song_id);
            printf("Title: %s\n", song->title);
            printf("Artist: %s\n", song->artist);
            printf("Album: %s\n", song->album_name);
            printf("Duration: %s\n", duration_str);
            printf("File: %s\n\n", song->file_path);
        } else {
            printf("Song with ID %d not found.\n", song_id);
        }
    } else {
        printf("Usage: search song <id>\n");
    }
}

static void handle_create_command(const char* args) {
    char name[256], artist[256];
    int year;
    
    if (sscanf(args, "album %255s %255s %d", name, artist, &year) == 3) {
        create_album(name, artist, year);
    } else {
        printf("Usage: create album <name> <artist> <year>\n");
    }
}

static void handle_delete_command(const char* args) {
    int album_id;
    
    if (sscanf(args, "album %d", &album_id) == 1) {
        delete_album(album_id);
    } else {
        printf("Usage: delete album <id>\n");
    }
}

static void handle_view_command(const char* args) {
    int album_id;
    
    if (sscanf(args, "album %d", &album_id) == 1) {
        view_album_songs(album_id);
    } else {
        printf("Usage: view album <id>\n");
    }
}

static void handle_add_command(const char* args) {
    int song_id, album_id;
    
    if (sscanf(args, "song %d to album %d", &song_id, &album_id) == 2) {
        add_song_to_album(song_id, album_id);
    } else if (sscanf(args, "album %d to playlist", &album_id) == 1) {
        add_album_to_playlist(album_id);
    } else if (sscanf(args, "song %d to playlist", &song_id) == 1) {
        add_song_to_playlist(song_id);
    } else {
        printf("Usage: add song <song_id> to album <album_id>\n");
        printf("       add album <album_id> to playlist\n");
        printf("       add song <song_id> to playlist\n");
    }
}

static void handle_remove_command(const char* args) {
    int song_id, album_id, index;
    
    if (sscanf(args, "song %d from album %d", &song_id, &album_id) == 2) {
        remove_song_from_album(song_id, album_id);
    } else if (sscanf(args, "song %d from playlist", &index) == 1) {
        remove_song_from_playlist(index);
    } else {
        printf("Usage: remove song <song_id> from album <album_id>\n");
        printf("       remove song <index> from playlist\n");
    }
}

static void handle_play_command(const char* args) {
    if (strstr(args, "next")) {
        play_next();
    } else if (strstr(args, "previous")) {
        play_previous();
    } else {
        printf("Usage: play next or play previous\n");
    }
}

static void handle_current_command(void) {
    SongNode* song = get_current_song();
    if (song) {
        char duration_str[32];
        format_duration(song->duration, duration_str, sizeof(duration_str));
        printf("\nCurrently Playing:\n");
        printf("Title: %s\n", song->title);
        printf("Artist: %s\n", song->artist);
        printf("Album: %s\n", song->album_name);
        printf("Duration: %s\n\n", duration_str);
    } else {
        printf("No song is currently playing.\n");
    }
}

void run_cli_loop(void) {
    char input[512];
    int running = 1;
    
    display_menu();
    
    while (running) {
        printf("c-unplugged> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Skip empty commands
        if (strlen(trim_whitespace(input)) == 0) {
            continue;
        }
        
        // Log command
        log_command(input);
        
        // Parse command
        char command[64];
        char* args = input;
        
        // Extract first word
        int i = 0;
        while (*args && *args != ' ' && i < (int)sizeof(command) - 1) {
            command[i++] = *args++;
        }
        command[i] = '\0';
        
        // Skip whitespace after command
        while (*args == ' ') args++;
        
        // Execute command
        if (strcmp(command, "list") == 0) {
            handle_list_command(args);
        } else if (strcmp(command, "search") == 0) {
            handle_search_command(args);
        } else if (strcmp(command, "create") == 0) {
            handle_create_command(args);
        } else if (strcmp(command, "delete") == 0) {
            handle_delete_command(args);
        } else if (strcmp(command, "view") == 0) {
            handle_view_command(args);
        } else if (strcmp(command, "add") == 0) {
            handle_add_command(args);
        } else if (strcmp(command, "remove") == 0) {
            handle_remove_command(args);
        } else if (strcmp(command, "show") == 0) {
            if (strstr(args, "playlist")) {
                display_playlist();
            } else {
                printf("Unknown show command.\n");
            }
        } else if (strcmp(command, "play") == 0) {
            handle_play_command(args);
        } else if (strcmp(command, "current") == 0) {
            handle_current_command();
        } else if (strcmp(command, "clear") == 0) {
            if (strstr(args, "playlist")) {
                clear_playlist();
            } else {
                printf("Unknown clear command.\n");
            }
        } else if (strcmp(command, "history") == 0) {
            display_command_history();
        } else if (strcmp(command, "help") == 0) {
            display_menu();
        } else if (strcmp(command, "exit") == 0) {
            running = 0;
            printf("Saving and exiting...\n");
        } else {
            printf("Unknown command: %s\n", command);
            printf("Type 'help' for available commands.\n");
        }
    }
}
