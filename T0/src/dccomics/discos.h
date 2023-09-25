#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "canciones.h"

typedef struct disco {
	int id_disco;
	int capacity;
	int id_song_max_raiting;
	int id_song_min_raiting;
	int max_raiting;
	int min_rating;
	int total_length;
	Cancion* canciones;
} Disco;

Disco* disco_init(int id_disco, int capacity);

void actualizar_info_disco(Disco* disco, int song_max, int song_min, int max, int min, int length);

void print_disco(Disco disco, FILE* file);

void print_song(Disco disco, int song_id, FILE* file);
