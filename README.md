# C-Unplugged Music Application

A command-line music library and playlist management application written in C. Features comprehensive music organization through songs, albums, and playlists with persistent storage and command history logging.

## Features

- **Song Management**: Load and browse a library of songs with detailed metadata
- **Album Management**: Create, modify, and delete custom albums
- **Playlist Management**: Build circular playlists from songs and albums
- **Persistent Storage**: Albums and command history are saved between sessions
- **Memory Safe**: No memory leaks - verified with Valgrind

## Building

Requires GCC and Make:

```bash
make
```

Clean build artifacts:

```bash
make clean
```

## Running

```bash
./c-unplugged
```

Or build and run in one step:

```bash
make run
```

## Usage

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
- `play next` - Move to next song (circular)
- `play previous` - Move to previous song (circular)
- `current song` - Display currently playing song
- `clear playlist` - Empty playlist

### System Commands
- `history` - Display command history from all sessions
- `help` - Show available commands
- `exit` - Save and quit application

## Project Structure

```
c-unplugged/
├── Makefile              # Build configuration
├── README.md             # This file
├── ARCHITECTURE.md       # Detailed architecture documentation
├── src/                  # Source files
│   ├── main.c
│   ├── songs.c
│   ├── albums.c
│   ├── playlist.c
│   ├── logger.c
│   ├── cli.c
│   └── utils.c
├── include/              # Header files
│   ├── songs.h
│   ├── albums.h
│   ├── playlist.h
│   ├── logger.h
│   ├── cli.h
│   └── utils.h
└── data/                 # Data files
    ├── songs_library.txt
    ├── albums.dat
    └── command_history.log
```

## Data Files

### songs_library.txt
Plain text format with pipe-separated values:
```
song_id|title|artist|album_name|duration|file_path
```

### albums.dat
Structured text format automatically managed by the application:
```
album_id|album_name|artist|year|song_id1,song_id2,...
```

### command_history.log
Append-only command log with timestamps and session IDs:
```
[YYYY-MM-DD HH:MM:SS][SESSION_ID] command_string
```

## Example Session

```
c-unplugged> list songs
[displays all songs]

c-unplugged> create album BestOf Various 2024
Album created successfully with ID: 1

c-unplugged> add song 1 to album 1
Song added to album successfully.

c-unplugged> add album 1 to playlist
Added 1 songs from album to playlist.

c-unplugged> show playlist
[displays playlist with current song marked with *]

c-unplugged> play next
Playing: Bohemian Rhapsody by Queen

c-unplugged> exit
Saving and exiting...
```

## Testing

The application has been tested with:
- GCC with `-Wall -Wextra -Werror` flags
- Valgrind memory leak detection (no leaks found)
- Manual testing of all features

## Architecture

See [ARCHITECTURE.md](ARCHITECTURE.md) for detailed architecture documentation including:
- Data structures (linked lists)
- Module breakdown
- Memory management strategy
- File formats
- Command parsing logic

## License

This is an educational project.
