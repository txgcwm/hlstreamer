#ifndef _M3U8_INCLUDED
#define _M3U8_INCLUDED

#include <stdio.h>
#include <stdint.h>

/**
 *	This library has the following objectives:
 *	 1. Read m3u8 files [both extended and standard]
 *	 2. Write m3u8 files, according to parameters, in a
 *		sequential manner
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *	Defines the structure of a single #EXTINF
 *	record
 */
typedef struct _inf_record {
	float length;
	char *name;
	char *path;
} InfRecord;

/**
 *	Plain initialization of InfRecord
 */
extern int8_t inf_init(InfRecord **record);

/**
 *	Sets the length of the media-segment this record defines
 */
extern int8_t inf_set_length(InfRecord **record, const float length);

/**
 *	Sets the name of the media segment
 */
extern int8_t inf_set_name(InfRecord **record, const char *name);

/**
 *	Sets the path of the media-segment
 */
extern int8_t inf_set_path(InfRecord **record, const char *path);

/**
 *	Safely destroy an InfRecord
 */
extern int8_t inf_destroy(InfRecord **record);

/**
 *	Holds the list of playlist records
 */
typedef struct _playlist_records {
	InfRecord *record;

	struct _playlist_records *next;
	struct _playlist_records *prev;
} PlaylistRecords;

/**
 *	Initializes the playlist record list
 */
extern int8_t record_list_init(PlaylistRecords **head);

/**
 *	Adds a new InfRecord to the playlist records list
 *	** This is slightly more expensive than the other signatures of the same
 */
extern int8_t record_list_add(PlaylistRecords **cur, InfRecord *data);

/**
 *	Adds a new InfRecord to the playlist records list
 */
extern int8_t record_list_add_more(PlaylistRecords **cur, const float length, const char *name, const char *path);

/**
 *	Removes record number [index]
 */
extern int8_t record_list_remove_at(PlaylistRecords **head, uint16_t index);

/**
 *	Removes the first [n] records from the playlist
 */
extern int8_t record_list_remove_first_n(PlaylistRecords **head, uint16_t n);

/**
 *	Destroys all records in the playlist
 */
extern int8_t record_list_destroy(PlaylistRecords **head);

/**
 *	Defines the types of playlists available
 */
typedef enum _x_playlist_type {
	DEFAULT = 0x00, VOD, EVENT, LIVE, VARIANT
} XPlaylistType;

/**
 *	Convert string to playlist type
 */
extern XPlaylistType playlist_type_from(const char *str);

/**
 *	Convert playlist type to string
 */
extern const char *playlist_type_str(const XPlaylistType type);

/**
 *	Defines a playlist
 */
typedef struct _playlist {
	XPlaylistType x_type;		// #EXT-X-PLAYLIST-TYPE
	uint8_t x_target_duration;	// #EXT-X-TARGETDURATION
	uint8_t x_version;			// #EXT-X-VERSION
	uint16_t x_media_sequence;	// #EXT-X-MEDIA-SEQUENCE

	PlaylistRecords *head;		// List of records in file
	PlaylistRecords *cur;		// Current record
} Playlist;

/**
 *	Write playlist to file
 */
extern int8_t playlist_write(Playlist *playlist, const char *filename);

/**
 *	Write playlist to provided file
 */
extern int8_t playlist_write_to(Playlist *playlist, FILE *out);

/**
 *	Read playlist from file
 *	Parse-Codes:
 *		7 - START tag not found/incorrectly placed2 [#EXTM3U]
 */
extern int8_t playlist_load(Playlist *playlist, const char *filename);

/**
 *	Read playlist from provided location
 */
extern int8_t playlist_load_from(Playlist *playlist, FILE *in);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _M3U8_INCLUDED */
