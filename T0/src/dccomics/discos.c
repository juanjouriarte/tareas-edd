
#include "discos.h"

Disco* disco_init(int id_disco, int capacity) {
	Disco* disco = malloc(sizeof(Disco));
	*disco = (Disco){
	    .id_disco = id_disco,
	    .capacity = capacity,
	    .id_song_max_raiting = 0,
	    .id_song_min_raiting = 0,
	    .max_raiting = 0,
	    .min_rating = 10000,
	    .total_length = 0,
	    .canciones = calloc(capacity, sizeof(Cancion)),
	};
	return disco;
}

void actualizar_info_disco(Disco* disco, int song_max, int song_min, int max, int min, int length) {
	disco->id_song_max_raiting = song_max;
	disco->id_song_min_raiting = song_min;
	disco->max_raiting = max;
	disco->min_rating = min;
	disco->total_length = length;
}

void print_disco(Disco disco, FILE* file) {
	fprintf(file, "ESTADO DISCO %d\n", disco.id_disco);
	fprintf(file, "        %d\n", disco.capacity);
	fprintf(file, "        %d %d\n", disco.max_raiting, disco.id_song_max_raiting);
	fprintf(file, "        %d %d\n", disco.min_rating, disco.id_song_min_raiting);
	fprintf(file, "        %d\n", disco.total_length);
	fprintf(file, "        CANCIONES\n");
	for (int i = 0; i < disco.capacity; i++) {
		fprintf(file, "                %d\n", disco.canciones[i].id_cancion);
	}
	fprintf(file, "FIN ESTADO\n");
}

void print_song(Disco disco, int song_id, FILE* file) {
	fprintf(file, "ESTADO CANCION %d\n", disco.canciones[song_id].id_cancion);
	fprintf(file, "        %d\n", disco.canciones[song_id].length);
	fprintf(file, "        %d\n", disco.canciones[song_id].rating);
	fprintf(file, "FIN ESTADO\n");
}