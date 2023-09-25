#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// struct cancion

typedef struct cancion {
	int id_cancion;
	int id_disco;
	int length;
	int rating;
} Cancion;

Cancion cancion_init(Cancion* cancion, int id_cancion, int id_disco, int length, int rating, FILE* file);