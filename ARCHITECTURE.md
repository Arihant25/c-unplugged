# C-Unplugged Music Application

## Project Overview
C-Unplugged is a CLI-based music library and playlist management application written in C. It provides comprehensive music organization through songs, albums, and playlists with persistent storage and command history logging.

## System Architecture

### Core Components

#### 1. Data Structures (LinkedList-based)
- **SongNode**: Represents individual songs in a linked list
  - Fields: song_id, title, artist, album_name, duration, file_path
  - Pointer to next node
  
- **AlbumNode**: Represents albums in a linked list
  - Fields: album_id, album_name, artist, year
  - Pointer to songs linked list (songs in this album)
  - Pointer to next album node
  
- **PlaylistNode**: Represents current playlist as circular linked list
  - Fields: pointer to song reference, play_count
  - Pointer to next node (circular - last node points to first)
  - Current playing position pointer

- **CommandHistoryNode**: Stores command history
  - Fields: timestamp, command_string, session_id
  - Pointer to next node

### 2. Module Structure

#### songs.h / songs.c
- `load_songs_from_file()` - Parse song library file and populate linked list
- `list_all_songs()` - Display all songs in library
- `find_song_by_id()` - Search song by ID
- `get_songs_by_album()` - Filter songs belonging to specific album
- `free_songs_list()` - Memory cleanup

#### albums.h / albums.c
- `load_albums_from_file()` - Load saved albums from persistent storage
- `save_albums_to_file()` - Persist albums to disk
- `create_album()` - Create new album
- `delete_album()` - Remove album
- `list_all_albums()` - Display all albums
- `add_song_to_album()` - Associate song with album
- `remove_song_from_album()` - Dissociate song from album
- `view_album_songs()` - Display songs in specific album
- `free_albums_list()` - Memory cleanup

#### playlist.h / playlist.c
- `init_playlist()` - Initialize circular linked list
- `add_album_to_playlist()` - Add all songs from album to playlist
- `add_song_to_playlist()` - Add individual song to playlist
- `remove_song_from_playlist()` - Remove song maintaining circular structure
- `play_next()` - Move to next song in circular list
- `play_previous()` - Move to previous song in circular list
- `get_current_song()` - Return currently playing song
- `display_playlist()` - Show all songs in playlist order
- `clear_playlist()` - Remove all songs
- `free_playlist()` - Memory cleanup

#### logger.h / logger.c
- `init_logger()` - Open log file in append mode
- `log_command()` - Write command with timestamp and session ID
- `display_command_history()` - Read and display all logged commands
- `close_logger()` - Close log file handle
- Session ID generated at program start (timestamp-based)

#### cli.h / cli.c
- `display_menu()` - Show available commands
- `parse_command()` - Parse user input into command and arguments
- `execute_command()` - Route command to appropriate module function
- `run_cli_loop()` - Main interactive loop
- Command tokenization and validation

#### utils.h / utils.c
- `trim_whitespace()` - String processing utility
- `parse_duration()` - Convert duration string to seconds
- `format_duration()` - Convert seconds to MM:SS format
- `generate_id()` - Create unique IDs for songs/albums
- `get_current_timestamp()` - Return formatted timestamp string

## File Structure

```
c-unplugged/
├── Makefile
├── ARCHITECTURE.md
├── README.md
├── src/
│   ├── main.c
│   ├── songs.c
│   ├── albums.c
│   ├── playlist.c
│   ├── logger.c
│   ├── cli.c
│   └── utils.c
├── include/
│   ├── songs.h
│   ├── albums.h
│   ├── playlist.h
│   ├── logger.h
│   ├── cli.h
│   └── utils.h
└── data/
    ├── songs_library.txt
    ├── albums.dat
    └── command_history.log
```

## Persistent Storage Format

### songs_library.txt
Plain text format, pipe-separated values:
```
song_id|title|artist|album_name|duration|file_path
```

### albums.dat
Binary or structured text format:
```
album_id|album_name|artist|year|song_count
song_id1,song_id2,song_id3...
```

### command_history.log
Append-only text file:
```
[YYYY-MM-DD HH:MM:SS][SESSION_ID] command_string
```

## CLI Commands

### Song Management
- `list songs` - Display all songs in library
- `search song <id>` - Find song by ID

### Album Management
- `list albums` - Display all albums
- `create album <name> <artist> <year>` - Create new album
- `delete album <id>` - Remove album
- `view album <id>` - Show songs in album
- `add song <song_id> to album <album_id>` - Add song to album
- `remove song <song_id> from album <album_id>` - Remove song from album

### Playlist Management
- `add album <album_id> to playlist` - Add all album songs to playlist
- `add song <song_id> to playlist` - Add individual song
- `remove song <index> from playlist` - Remove from playlist
- `show playlist` - Display current playlist
- `play next` - Move to next song
- `play previous` - Move to previous song
- `current song` - Display currently playing song
- `clear playlist` - Empty playlist

### System Commands
- `history` - Display command history
- `help` - Show available commands
- `exit` - Save and quit application

## Workflow

### Startup Sequence
1. Initialize logger with new session ID
2. Load songs library from file into linked list
3. Load saved albums from persistent storage
4. Initialize empty circular playlist
5. Display welcome message and prompt

### Runtime Loop
1. Display prompt
2. Read user command
3. Log command to history file
4. Parse and validate command
5. Execute corresponding function
6. Display result/feedback
7. Repeat until exit command

### Shutdown Sequence
1. Save all albums to persistent storage
2. Free all linked lists (songs, albums, playlist, history)
3. Close logger file handle
4. Exit gracefully

## Memory Management

### Allocation Strategy
- Dynamic allocation for all nodes using malloc
- Each module responsible for its own memory
- Global cleanup function calls module-specific free functions

### Optimization Considerations
- Circular linked list for playlist eliminates array reallocation
- Single-pass search operations with early termination
- Lazy loading - parse files only on startup
- Minimal data duplication - playlist stores song pointers, not copies

## Error Handling
- Validate file existence on startup
- Check malloc return values
- Validate command arguments before execution
- Handle missing album/song IDs gracefully
- Provide clear error messages to user

## Makefile Structure

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -I./include
SRC = src/*.c
OBJ = $(SRC:.c=.o)
TARGET = c-unplugged

run: $(TARGET)
    ./$(TARGET)

$(TARGET): $(OBJ)
    $(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
    rm -f $(TARGET) $(OBJ)
```

## Testing Considerations
- Test circular playlist wraparound (last -> first, first -> last)
- Test album persistence across sessions
- Test command history accumulation across multiple runs
- Test edge cases: empty playlist, single-song playlist
- Test memory cleanup with valgrind

## Scalability Notes
- Linked lists allow unlimited songs/albums (memory permitting)
- Log file may grow large - consider rotation strategy for production
- File I/O occurs only on startup/shutdown for performance
- O(n) search operations acceptable for typical library sizes