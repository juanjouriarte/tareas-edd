#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "discos.h"

// struct Batiplaylist
// Lista anidada de canciones

typedef struct batiplaylist {
	int id_playlist;
	Cancion* cancion;
	struct batiplaylist* next;
} Batiplaylist;

Batiplaylist* batiplaylist_init(int id_playlist);

void batiplaylist_add_song(Cancion* cancion, Batiplaylist* playlist, Batiplaylist* ultima_batiplaylist[], int id_playlist);

void batiplaylist_remove_song(Batiplaylist* playlist, int disk_id, int id_cancion, Batiplaylist* ultima_batiplaylist[]);

void batiplaylist_destroy(Batiplaylist* playlist);

void batiplaylist_add_disc(Batiplaylist* playlist, Batiplaylist* ultima_playlist[], Disco* disco, FILE* file);

bool check_song_in_batiplaylist(Batiplaylist* playlist, int disk_id, int song_id);

void print_batiplaylist(Batiplaylist* playlist, FILE* file);

void print_batiplaylist_mean_rating(Batiplaylist* playlist, FILE* file);

int batiplaylist_song_count(Batiplaylist* playlist);

void batiplaylist_union(Batiplaylist* playlist1, Batiplaylist* ultima_playlist_1[], Batiplaylist* playlist2, int id_playlist);

Batiplaylist* batiplaylist_split_in_two_from_index(Batiplaylist* playlist, Batiplaylist* ultima_batiplaylist_1[], int new_playlist_id, int index);

int batiplaylist_delete_all_song_with_rating_less_than(Batiplaylist* playlist, Batiplaylist* ultima_batiplaylist[], int rating);

Cancion* save_all_song_from_batiplaylist_in_array(Batiplaylist* playlist, Cancion* array_canciones[]);

void new_last_batiplaylist(Batiplaylist* playlist, Batiplaylist* ultima_batiplaylist_1[], int new_playlist_id);