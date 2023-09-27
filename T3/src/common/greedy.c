#include "greedy.h"

Edge_greedy* create_edge_greedy(Node_greedy* node1, Node_greedy* node2, int weight) {
	Edge_greedy* edge = (Edge_greedy*)malloc(sizeof(Edge_greedy));
	edge->node1 = node1;
	edge->node2 = node2;
	edge->weight = weight;
	return edge;
}

void print_edges_greedy(Edge_greedy** edges, int edges_count) {
	for (int i = 0; i < edges_count; i++) {
		printf("Edge %d: %d - %d ; Costo %d\n", i, edges[i]->node1->id, edges[i]->node2->id, edges[i]->weight);
	}
}

// Djikstra Algorithm to find the shortes path
void free_edges_greedy(Edge_greedy** edges, int edges_count) {
	for (int i = 0; i < edges_count; i++) {
		free(edges[i]);
	}
}

void free_nodes_greedy(Node_greedy** nodes, int nodes_count) {
	for (int i = 0; i < nodes_count; i++) {
		free(nodes[i]);
	}
}

void free_neighbors_greedy(Node_greedy* node) {
	free(node->edges);
}

Node_greedy* create_node_greedy(int id) {
	// printf("Creando nodo %d\n", id);
	Node_greedy* node = (Node_greedy*)malloc(sizeof(Node_greedy));
	node->id = id;
	node->edges = NULL;
	node->edges_count = 0;
	return node;
}

void add_edge_greedy(Node_greedy* node, Edge_greedy* edge) {
	node->edges_count++;
	node->edges = (Edge_greedy**)realloc(node->edges, node->edges_count * sizeof(Edge_greedy*));
	node->edges[node->edges_count - 1] = edge;
	// printf("AGREGADO Edge A NODO ID: %d ;  %d: %d - %d ; Costo %d\n", node->id, node->edges_count, edge->node1->id, edge->node2->id, edge->weight);
}

// Djikstra Algorithm to find the shortes path

int djikstra_algorithm(Node_greedy** nodes, int nodes_count, int start, int end) {
	int* distances = (int*)malloc(nodes_count * sizeof(int));
	int* visited = (int*)malloc(nodes_count * sizeof(int));
	int* previous = (int*)malloc(nodes_count * sizeof(int));
	for (int i = 0; i < nodes_count; i++) {
		distances[i] = INT_MAX;
		visited[i] = 0;
		previous[i] = -1;
	}
	distances[start] = 0;
	for (int i = 0; i < nodes_count; i++) {
		int min = INT_MAX;
		int min_index = -1;
		for (int j = 0; j < nodes_count; j++) {
			if (visited[j] == 0 && distances[j] < min) {
				min = distances[j];
				min_index = j;
			}
		}
		// printf("Minimo Index %d\n", min_index);
		if (min_index == -1) {
			break;
		}
		visited[min_index] = 1;
		for (int j = 0; j < nodes[min_index]->edges_count; j++) {
			int alt = distances[min_index] + nodes[min_index]->edges[j]->weight;
			if (nodes[min_index]->edges[j]->node2->id == min_index) {
				if (alt < distances[nodes[min_index]->edges[j]->node1->id]) {
					distances[nodes[min_index]->edges[j]->node1->id] = alt;
					previous[nodes[min_index]->edges[j]->node1->id] = min_index;
				}
			}
			else {
				if (alt < distances[nodes[min_index]->edges[j]->node2->id]) {
					distances[nodes[min_index]->edges[j]->node2->id] = alt;
					previous[nodes[min_index]->edges[j]->node2->id] = min_index;
				}
			}
		}
	}
	int costo = distances[end];
	// // Print distances
	// for (int i = 0; i < nodes_count; i++) {
	// 	printf("Distancia %d: %d\n", i, distances[i]);
	// }
	// // Print previous
	// for (int i = 0; i < nodes_count; i++) {
	// 	printf("Previous %d: %d\n", i, previous[i]);
	// }
	free(distances);
	free(visited);
	free(previous);
	printf("El camino mas corto es: %d \n", costo);
	return costo;
}