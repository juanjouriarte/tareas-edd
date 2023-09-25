#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "batiplaylist.h"
#include "canciones.h"
#include "discos.h"
/* Retorna true si ambos string so iguales */
bool string_equals(char *string1, char *string2) {
	return !strcmp(string1, string2);
}

/* Función encargada de chequear los argumentos ingresados */
bool check_arguments(int argc, char **argv) {
	if (argc != 3) {
		printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
		printf("Donde:\n");
		printf("\tINPUT es la ruta del archivo de input\n");
		printf("\tOUTPUT es la ruta del archivo de output\n");
		return false;
	}
	return true;
}

int main(int argc, char **argv) {
	/////////////////////////
	//        Input        //
	/////////////////////////
	/* Si los parámetros del programa son inválidos */
	if (!check_arguments(argc, argv)) {
		/* Salimos del programa indicando que no terminó correctamente */
		return 1;
	}

	/* Abrimos el archivo de input */
	FILE *input_file = fopen(argv[1], "r");

	/* Abrimos el archivo de output */
	FILE *output_file = fopen(argv[2], "w");

	/* String que usaremos para guardar la instrucción actual*/
	char command[32];

	// Lectura del número de batiplaylists
	int N_BATIPLAYLISTS;
	fscanf(input_file, "%d", &N_BATIPLAYLISTS);

	// Lectura del número de discos
	int N_DISCS;
	fscanf(input_file, "%d", &N_DISCS);

	/* Leemos la primera instrucción */
	fscanf(input_file, "%s", command);

	int id_disco = 0;

	// Creamos las clases
	Disco *discos[N_DISCS];
	Batiplaylist *batiplaylists[N_BATIPLAYLISTS];
	int *batiplaylist_index = calloc(N_BATIPLAYLISTS, sizeof(int *));
	Batiplaylist *ultima_batiplaylist[N_BATIPLAYLISTS];
	/* Mientras la instrucción sea distinta a FIN */
	while (!string_equals(command, "FIN")) {
		/////////////////////////
		//       Parte A       //
		/////////////////////////

		if (string_equals(command, "CREAR-DISCO")) {
			/* Obtenemos la información del disco */
			int capacity, length, rating;
			fscanf(input_file, "%d", &capacity);
			int id_cancion = 0;
			int i_max = 0;
			int i_min = 10000;
			int song_max = 0;
			int song_min = 0;
			int length_total = 0;
			/* COMPLETAR */
			discos[id_disco] = disco_init(id_disco, capacity);
			while (id_cancion < capacity) {
				fscanf(input_file, "%d", &length);
				fscanf(input_file, "%d", &rating);
				discos[id_disco]->canciones[id_cancion] = cancion_init(&discos[id_disco]->canciones[id_cancion], id_cancion, id_disco, length, rating, output_file);
				if (rating > i_max) {
					i_max = rating;
					song_max = id_cancion;
				}
				if (rating < i_min) {
					i_min = rating;
					song_min = id_cancion;
				}
				length_total += length;
				id_cancion++;
			}
			actualizar_info_disco(discos[id_disco], song_max, song_min, i_max, i_min, length_total);
			id_disco++;
		}

		else if (string_equals(command, "ELIMINAR-CANCION")) {
			/* Obtenemos la información de la canción */
			int disc_id, song_id;
			fscanf(input_file, "%d %d", &disc_id, &song_id);

			/* COMPLETAR */
		}

		else if (string_equals(command, "IMPRIMIR-DISCO")) {
			/* Obtenemos la información correspondiente */
			int disc_id;
			fscanf(input_file, "%d", &disc_id);

			/* COMPLETAR */
			print_disco(*discos[disc_id], output_file);
		}
		else if (string_equals(command, "IMPRIMIR-CANCION")) {
			/* Obtenemos la información correspondiente */
			int disc_id, song_id;
			fscanf(input_file, "%d %d", &disc_id, &song_id);

			/* COMPLETAR */
			print_song(*discos[disc_id], song_id, output_file);
		}

		/////////////////////////
		//       Parte B       //
		/////////////////////////

		else if (string_equals(command, "CREAR-BATIPLAYLIST")) {
			/* Obtenemos la información correspondiente */
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);
			//
			///* COMPLETAR */
			fprintf(output_file, "BATIPLAYLIST CREATED %d\n", playlist_id);
			batiplaylist_index[playlist_id] = 1;
			batiplaylists[playlist_id] = batiplaylist_init(playlist_id);
		}

		else if (string_equals(command, "AGREGAR-CANCION-BATIPLAYLIST")) {
			int playlist_id, disc_id, song_id;
			fscanf(input_file, "%d %d %d", &playlist_id, &disc_id, &song_id);

			/* COMPLETAR */
			if (discos[disc_id]->capacity > song_id) {
				batiplaylist_add_song(&discos[disc_id]->canciones[song_id], batiplaylists[playlist_id], ultima_batiplaylist, playlist_id);
				fprintf(output_file, "NEW SONG ADDED %d %d %d\n", song_id, disc_id, playlist_id);
			}
			else {
				fprintf(output_file, "SONG NOT FOUND\n");
			}
		}

		else if (string_equals(command, "ELIMINAR-CANCION-BATIPLAYLIST")) {
			int playlist_id, disk_id, song_id;
			fscanf(input_file, "%d %d %d", &playlist_id, &disk_id, &song_id);
			/* COMPLETAR */
			bool check = check_song_in_batiplaylist(batiplaylists[playlist_id], disk_id, song_id);
			if (check) {
				batiplaylist_remove_song(batiplaylists[playlist_id], disk_id, song_id, ultima_batiplaylist);
				fprintf(output_file, "ELIMINADO %d %d %d\n", song_id, disk_id, playlist_id);
			}
			else {
				fprintf(output_file, "SONG NOT FOUND ON PLAYLIST\n");
			}
		}

		else if (string_equals(command, "AGREGAR-DISCO-BATIPLAYLIST")) {
			int playlist_id, disc_id;
			fscanf(input_file, "%d %d", &playlist_id, &disc_id);

			/* COMPLETAR */
			batiplaylist_add_disc(batiplaylists[playlist_id], ultima_batiplaylist, discos[disc_id], output_file);
		}

		else if (string_equals(command, "PLAY-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
			print_batiplaylist(batiplaylists[playlist_id], output_file);
		}

		else if (string_equals(command, "RATE-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
			print_batiplaylist_mean_rating(batiplaylists[playlist_id], output_file);
		}

		/////////////////////////
		//       Parte C       //
		/////////////////////////

		else if (string_equals(command, "ELIMINAR-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
			int contador_songs = batiplaylist_song_count(batiplaylists[playlist_id]);
			batiplaylist_destroy(batiplaylists[playlist_id]);
			batiplaylist_index[playlist_id] = 0;
			fprintf(output_file, "BATIPLAYLIST DELETED %d %d\n", playlist_id, contador_songs);
		}

		else if (string_equals(command, "UNIR-BATIPLAYLIST")) {
			int playlist_id1, playlist_id2;
			fscanf(input_file, "%d %d", &playlist_id1, &playlist_id2);

			/* COMPLETAR */
			batiplaylist_union(batiplaylists[playlist_id1], ultima_batiplaylist, batiplaylists[playlist_id2], playlist_id1);
			fprintf(output_file, "JOINED %d AND %d\n", playlist_id1, playlist_id2);
			batiplaylist_index[playlist_id2] = 0;
		}

		else if (string_equals(command, "SPLIT-BATIPLAYLIST")) {
			int playlist_id, new_playlist_id, position;
			fscanf(input_file, "%d %d %d", &playlist_id, &new_playlist_id, &position);

			/* COMPLETAR */
			batiplaylist_index[new_playlist_id] = 1;
			batiplaylists[new_playlist_id] = batiplaylist_split_in_two_from_index(batiplaylists[playlist_id], ultima_batiplaylist, new_playlist_id, position);
			new_last_batiplaylist(batiplaylists[new_playlist_id], ultima_batiplaylist, new_playlist_id);
		}

		else if (string_equals(command, "ORDENAR-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
		}

		else if (string_equals(command, "PURGAR-BATIPLAYLIST")) {
			int playlist_id, rating;
			fscanf(input_file, "%d %d", &playlist_id, &rating);

			/* COMPLETAR */
			int contador = batiplaylist_delete_all_song_with_rating_less_than(batiplaylists[playlist_id], ultima_batiplaylist, rating);
			fprintf(output_file, "BATIPLAYLIST PURGED %d %d\n", playlist_id, contador);
		}

		/* Leemos la siguiente instrucción */
		fscanf(input_file, "%s", command);
	}

	/////////////////////////////////////
	//        Cerramos archivos        //
	////////////////////////////////////

	fclose(input_file);
	fclose(output_file);

	///////////////////////////////////
	//     Liberamos memoria         //
	///////////////////////////////////

	/* COMPLETAR */
	for (int i = 0; i < N_DISCS; i++) {
		free(discos[i]->canciones);
		free(discos[i]);
	}

	for (int i = 0; i < N_BATIPLAYLISTS; i++) {
		if (batiplaylist_index[i] == 1) {
			batiplaylist_destroy(batiplaylists[i]);
		}
	}

	free(batiplaylist_index);

	return 0;
}
