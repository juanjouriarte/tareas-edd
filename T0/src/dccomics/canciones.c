
#include "canciones.h"

Cancion cancion_init(Cancion* cancion, int id_cancion, int id_disco, int length, int rating, FILE* file) {
	*cancion = (Cancion){
	    .id_cancion = id_cancion,
	    .id_disco = id_disco,
	    .length = length,
	    .rating = rating,
	};
	fprintf(file, "CANCION AGREGADA %d %d\n", cancion->id_cancion, cancion->id_disco);
	return *cancion;
}
