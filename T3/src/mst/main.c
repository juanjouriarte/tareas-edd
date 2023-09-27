#include <stdio.h>
#include <stdlib.h>

#include "../common/graph.h"

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
	int numero_nodos;
	int x, y;

	fscanf(input_stream, "%d", &numero_nodos);
	// printf("numero_nodos: %d \n", numero_nodos);

	// Lista de nodos
	Node* nodos[numero_nodos];

	for (int i = 0; i < numero_nodos; i++) {
		fscanf(input_stream, "%d %d", &x, &y);
		// printf("Nodo_id = %d, X = %d Y= %d \n", i, x, y);
		nodos[i] = create_node(i, x, y);
	}

	for (int i = 0; i < numero_nodos; i++) {
		connect_node_with_all_nodes(nodos[i], nodos, numero_nodos);
		// print_all_node_neightbors(nodos[i]);
	}

	// Visitamos los nodos
	// recursive_dfs(nodos[0]);

	// Encontramos el MST
	Edge** mst = find_minimum_mst_with_kruskal(nodos, numero_nodos, output_stream);

	// int edges_count = (numero_nodos * (numero_nodos - 1)) / 2;

	// Printeamos el peso total del MST
	// print_total_weight(mst, numero_nodos - 1);

	// Printeamos el MST
	// print_mst(mst, numero_nodos - 1);

	// Destruimos el MST
	// free_mst(mst, numero_nodos - 1); // Borrado antes
	// Print count vecinos
	// count_neighbors_mst(nodos, numero_nodos);
	free(mst);

	// Destruir Edges
	// free_edges(mst, edges_count);

	// Destruir nodos y vecinos
	for (int i = 0; i < numero_nodos; i++) {
		free_neighbors(nodos[i]);
		destroy_node(nodos[i]);
	}

	// Cerrar archivo de input
	fclose(input_stream);

	// Cerrar archivo de output
	fclose(output_stream);

	return 0;
}
