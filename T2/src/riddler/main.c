#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Modo de uso: %s input output\nDonde:\n", argv[0]);
		printf("\t\"input\" es la ruta al archivo de input\n");
		printf("\t\"output\" es la ruta al archivo de output\n");
		return 1;
	}

	// Abrimos el archivo de input
	FILE* input_stream = fopen(argv[1], "r");

	// Abrimos el archivo de output
	FILE* output_stream = fopen(argv[2], "w");

	// Si alguno de los dos archivos no se pudo abrir
	if (!input_stream) {
		printf("El archivo %s no existe o no se puede leer\n", argv[1]);
		return 2;
	}
	if (!output_stream) {
		printf("El archivo %s no se pudo crear o no se puede escribir\n", argv[2]);
		printf("Recuerda que \"fopen\" no puede crear carpetas\n");
		fclose(input_stream);
		return 3;
	}

	// [Aqui va tu tarea]
	int nodos_arbol;
	int nodo;
	int altura_arbol;

	fscanf(input_stream, "%d", &nodos_arbol);

	altura_arbol = log2(nodos_arbol + 1);

	int A[nodos_arbol];

	int Largo_hash[altura_arbol];

	for (int i = 0; i < altura_arbol; i++) {
		Largo_hash[i] = 1;
	}

	int len_hash = bit_to_integer_array(Largo_hash, altura_arbol);

	List* hash_table[len_hash + 1];

	int* initilized_hash = (int*)calloc(len_hash + 1, sizeof(int));

	for (int i = 0; i < nodos_arbol; i++) {
		fscanf(input_stream, "%d", &nodo);
		A[i] = nodo;
	}

	// Hasheamos el arbol
	hash_tree(A, hash_table, initilized_hash, nodos_arbol, altura_arbol);

	int numero_consultas;
	int nodos_arbol_consulta;

	fscanf(input_stream, "%d", &numero_consultas);

	for (int i = 0; i < numero_consultas; i++) {
		fscanf(input_stream, "%d", &nodos_arbol_consulta);
		int arbol_consulta[nodos_arbol_consulta];
		for (int j = 0; j < nodos_arbol_consulta; j++) {
			fscanf(input_stream, "%d", &nodo);
			arbol_consulta[j] = nodo;
		}
		hash_function(arbol_consulta, nodos_arbol_consulta, initilized_hash, hash_table, output_stream, altura_arbol, A);
	}

	// Free all hash tree
	for (int i = 0; i < len_hash; i++) {
		if (initilized_hash[i] == 1) {
			free_hash_table(hash_table[i]);
		}
	}

	// Free initiliazed hash
	free(initilized_hash);

	// Cerrar archivo de input
	fclose(input_stream);

	// Cerrar archivo de output
	fclose(output_stream);

	return 0;
}
