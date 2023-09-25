#include "batiplaylist.h"

Batiplaylist* batiplaylist_init(int id_playlist) {
	Batiplaylist* playlist = malloc(sizeof(Batiplaylist));
	*playlist = (Batiplaylist){
	    .id_playlist = id_playlist,
	    .cancion = NULL,
	    .next = NULL,
	};
	return playlist;
}

void batiplaylist_add_song(Cancion* cancion, Batiplaylist* playlist, Batiplaylist* ultima_playlist[], int id_playlist) {
	// ADD ELEMENT TO LIST FROM ANY POSITION
	if (playlist->cancion == NULL) {
		playlist->cancion = cancion;
		playlist->next = NULL;
	}
	else if (playlist->next == NULL && playlist->cancion != NULL) {
		playlist->next = batiplaylist_init(id_playlist);
		playlist->next->cancion = cancion;
		ultima_playlist[id_playlist] = playlist->next;
	}
	else if (playlist->next != NULL && playlist->cancion != NULL) {
		ultima_playlist[id_playlist]->next = batiplaylist_init(id_playlist);
		ultima_playlist[id_playlist]->next->cancion = cancion;
		ultima_playlist[id_playlist] = ultima_playlist[id_playlist]->next;
	}
}

void batiplaylist_remove_song(Batiplaylist* playlist, int disk_id, int song_id, Batiplaylist* ultima_batiplaylist[]) {
	Batiplaylist* current = playlist;
	Batiplaylist* prev = NULL;
	int contador = 0;
	while (current != NULL) {
		if (current->cancion->id_cancion == song_id && current->cancion->id_disco == disk_id) {
			if (prev == NULL) {
				Batiplaylist* aux = current;
				aux->cancion = NULL;
				// current = current->next;
				// free(aux);
			}
			else {
				if (current == ultima_batiplaylist[playlist->id_playlist]) {
					ultima_batiplaylist[playlist->id_playlist] = prev;
					ultima_batiplaylist[playlist->id_playlist]->next = NULL;
				}
				else {
					prev->next = current->next;
					current->next = NULL;
				}
				free(current);
				current = prev->next;
			}
			contador++;
			break;
		}
		prev = current;
		current = current->next;
	}
	if (contador == 0) {
		printf("SONG NOT FOUND ON PLAYLIST\n");
	}
}

void batiplaylist_destroy(Batiplaylist* playlist) {
	Batiplaylist* current = playlist;
	if (current != NULL) {
		while (current->next != NULL) {
			Batiplaylist* aux = current;
			current = current->next;
			free(aux);
		}
		if (current->next == NULL) {
			free(current);
		}
	}
	else {
		printf("THERE IS NO PLAYLIST\n");
	}
}

void batiplaylist_add_disc(Batiplaylist* playlist, Batiplaylist* ultima_playlist[], Disco* disco, FILE* file) {
	int contador = 0;
	for (int i = 0; i < disco->capacity; i++) {
		Cancion* cancion = &disco->canciones[i];
		bool check = check_song_in_batiplaylist(playlist, cancion->id_disco, cancion->id_cancion);
		if (!check) {
			batiplaylist_add_song(cancion, playlist, ultima_playlist, playlist->id_playlist);
			contador++;
		}
	}
	fprintf(file, "AGREGADO %d %d %d\n", contador, disco->id_disco, playlist->id_playlist);
}

bool check_song_in_batiplaylist(Batiplaylist* playlist, int disk_id, int song_id) {
	Batiplaylist* current = playlist;
	while (current->next != NULL) {
		if (current->cancion->id_cancion == song_id && current->cancion->id_disco == disk_id) {
			return true;
		}
		current = current->next;
	}
	if (current->cancion != NULL && current->next == NULL) {
		if (current->cancion->id_cancion == song_id && current->cancion->id_disco == disk_id) {
			return true;
		}
	}
	return false;
}

void print_batiplaylist(Batiplaylist* playlist, FILE* file) {
	Batiplaylist* current = playlist;
	fprintf(file, "ESTADO BATIPLAYLIST %d\n", playlist->id_playlist);
	int contador_canciones = batiplaylist_song_count(current);
	fprintf(file, "        %d\n", contador_canciones);
	fprintf(file, "        CANCIONES\n");
	while (current->next != NULL) {
		fprintf(file, "                %d %d\n", current->cancion->id_cancion, current->cancion->id_disco);
		current = current->next;
	}
	if (current->next == NULL && current->cancion != NULL) {
		fprintf(file, "                %d %d\n", current->cancion->id_cancion, current->cancion->id_disco);
	}
	fprintf(file, "FIN ESTADO\n");
}

void print_batiplaylist_mean_rating(Batiplaylist* playlist, FILE* file) {
	Batiplaylist* current = playlist;
	float suma = 0;
	int contador = 0;
	while (current->next != NULL) {
		suma += current->cancion->rating;
		contador++;
		current = current->next;
	}
	if (current->next == NULL && current->cancion != NULL) {
		suma += current->cancion->rating;
		contador++;
	}
	fprintf(file, "BATIPLAYLIST %d: %.2f\n", current->id_playlist, suma / contador);
}

int batiplaylist_song_count(Batiplaylist* playlist) {
	Batiplaylist* current = playlist;
	int contador = 0;
	while (current->next != NULL) {
		contador++;
		current = current->next;
	}
	if (current->next == NULL && current->cancion != NULL) {
		contador++;
	}
	return contador;
}

Batiplaylist* batiplaylist_split_in_two_from_index(Batiplaylist* playlist, Batiplaylist* ultima_batiplaylist_1[], int new_playlist_id, int index) {
	Batiplaylist* current = playlist;
	Batiplaylist* prev = NULL;
	int contador = 0;
	while (current->next != NULL) {
		if (contador == index) {
			if (prev == NULL) {
				Batiplaylist* aux = current;
				ultima_batiplaylist_1[playlist->id_playlist] = prev;
				aux->id_playlist = new_playlist_id;
				return aux;
			}
			else {
				Batiplaylist* aux = current;
				ultima_batiplaylist_1[playlist->id_playlist] = prev;
				aux->id_playlist = new_playlist_id;
				prev->next = NULL;
				ultima_batiplaylist_1[new_playlist_id] = aux;
				return aux;
			}
		}
		prev = current;
		current = current->next;
		contador++;
	}
	if (current->next == NULL && current->cancion != NULL) {
		if (contador == index) {
			if (prev == NULL) {
				Batiplaylist* aux = current;
				ultima_batiplaylist_1[playlist->id_playlist] = prev;
				aux->id_playlist = new_playlist_id;
				return aux;
			}
			else {
				Batiplaylist* aux = current;
				ultima_batiplaylist_1[playlist->id_playlist] = prev;
				aux->id_playlist = new_playlist_id;
				prev->next = NULL;
				return aux;
			}
		}
	}
	return NULL;
}

void new_last_batiplaylist(Batiplaylist* playlist, Batiplaylist* ultima_batiplaylist_1[], int new_playlist_id) {
	Batiplaylist* current = playlist;
	while (current->next != NULL) {
		current->next->id_playlist = new_playlist_id;
		current = current->next;
	}
	if (current->next == NULL && current->cancion != NULL) {
		current->id_playlist = new_playlist_id;
	}
	ultima_batiplaylist_1[new_playlist_id] = current;
}

int batiplaylist_delete_all_song_with_rating_less_than(Batiplaylist* playlist, Batiplaylist* ultima_batiplaylist[], int rating) {
	Batiplaylist* current = playlist;
	Batiplaylist* prev = NULL;
	int contador = 0;
	while (current->next != NULL) {
		if (current->cancion->rating < rating) {
			if (prev == NULL) {
				Batiplaylist* aux = current;
				current = current->next;
				aux->cancion = current->cancion;
				aux->next = current->next;
				free(current);
				current = aux;
				prev = NULL;
				contador++;
				continue;
			}
			else {
				prev->next = current->next;
				free(current);
				current = prev->next;
				contador++;
				continue;
			}
		}
		if (current != NULL) {
			prev = current;
			current = current->next;
		}
		else {
			break;
		}
	}
	if (current->next == NULL && current->cancion != NULL) {
		if (current->cancion->rating < rating) {
			ultima_batiplaylist[playlist->id_playlist] = prev;
			prev->next = NULL;
			free(current);
			contador++;
		}
	}
	return contador;
}

void batiplaylist_union(Batiplaylist* playlist1, Batiplaylist* ultima_playlist_1[], Batiplaylist* playlist2, int id_playlist) {
	Batiplaylist* current = playlist1;
	Batiplaylist* segunda = playlist2;
	while (segunda->next != NULL) {
		bool check = check_song_in_batiplaylist(current, segunda->cancion->id_disco, segunda->cancion->id_cancion);
		if (!check) {
			batiplaylist_add_song(segunda->cancion, playlist1, ultima_playlist_1, id_playlist);
		}
		segunda = segunda->next;
	}
	if (segunda->next == NULL && segunda->cancion != NULL) {
		bool check = check_song_in_batiplaylist(current, segunda->cancion->id_disco, segunda->cancion->id_cancion);
		if (!check) {
			batiplaylist_add_song(segunda->cancion, playlist1, ultima_playlist_1, id_playlist);
		}
	}
	batiplaylist_destroy(playlist2);
}