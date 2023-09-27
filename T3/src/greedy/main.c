#include <stdio.h>
#include <stdlib.h>

#include "../common/greedy.h"

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
	int inicio_ruta, fin_ruta;
	int numero_vertices;

	fscanf(input_stream, "%d", &numero_nodos);
	printf("numero_nodos: %d \n", numero_nodos);

	Node_greedy* nodos[numero_nodos];

	for (int i = 0; i < numero_nodos; i++) {
		nodos[i] = create_node_greedy(i);
	}

	fscanf(input_stream, "%d %d", &inicio_ruta, &fin_ruta);
	printf("inicio_ruta: %d \n", inicio_ruta);
	printf("fin_ruta: %d \n", fin_ruta);

	fscanf(input_stream, "%d", &numero_vertices);
	printf("numero_vertices: %d \n", numero_vertices);

	Edge_greedy* edges[numero_vertices];

	for (int i = 0; i < numero_vertices; i++) {
		int nodo1, nodo2, peso;
		fscanf(input_stream, "%d %d %d", &nodo1, &nodo2, &peso);
		// add_neighbor(nodos[nodo1], nodos[nodo2]);
		// add_neighbor(nodos[nodo2], nodos[nodo1]);
		edges[i] = create_edge_greedy(nodos[nodo1], nodos[nodo2], peso);
		add_edge_greedy(nodos[nodo1], edges[i]);
		add_edge_greedy(nodos[nodo2], edges[i]);
	}

	// print_edges_greedy(edges, numero_vertices);
	// Djikstra

	int costo = djikstra_algorithm(nodos, numero_nodos, inicio_ruta, fin_ruta);
	fprintf(output_stream, "%d", costo);

	for (int i = 0; i < numero_nodos; i++) {
		free_neighbors_greedy(nodos[i]);
	}

	free_edges_greedy(edges, numero_vertices);

	free_nodes_greedy(nodos, numero_nodos);

	// Cerrar archivo de input
	fclose(input_stream);

	// Cerrar archivo de output
	fclose(output_stream);

	return 0;
}
