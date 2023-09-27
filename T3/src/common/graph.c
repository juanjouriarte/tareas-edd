#include "graph.h"

Node* create_node(int id, int x, int y) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->id = id;
	node->x = x;
	node->y = y;
	node->visited = 0;
	node->neighbors_count = 0;
	node->next = NULL;
	return node;
}

void destroy_node(Node* node) {
	free(node);
}

void print_node(Node* node) {
	printf("Node id: %d, x: %d, y: %d, visited: %d, neighbors_count: %d\n", node->id, node->x, node->y, node->visited, node->neighbors_count);
}

Node** connect_node_with_all_nodes(Node* node, Node** nodes, int nodes_count) {
	node->neighbors = (Node**)malloc(sizeof(Node*) * nodes_count - 1);
	for (int i = 0; i < nodes_count; i++) {
		if (node->id != nodes[i]->id) {
			node->neighbors[node->neighbors_count] = nodes[i];
			node->neighbors_count++;
		}
	}
	return node->neighbors;
}

void print_all_node_neightbors(Node* node) {
	print_node(node);
	for (int i = 0; i < node->neighbors_count; i++) {
		printf("Neighbor: %d \n", node->neighbors[i]->id);
	}
}

void free_neighbors(Node* node) {
	free(node->neighbors);
}

int recursive_dfs(Node* node, int* visited, Node* parent) {
	visited[node->id] = 1;
	// printf("Revisando Vecinos Nodo id %d, Pos (%d, %d) \n", node->id, node->x, node->y);
	for (int i = 0; i < node->neighbors_count; i++) {
		if (visited[node->neighbors[i]->id] == 0) {
			recursive_dfs(node->neighbors[i], visited, node);
		}
		else if (node->neighbors[i]->id == parent->id) {
			// printf("Es el padre, por lo que no pasa nada, CONTINUAMOS\n");
		}
		else if (visited[node->neighbors[i]->id] == 1 && node->neighbors[i]->id != parent->id) {
			// printf("El nodo id %d Pos (%d, %d) ya ha sido visitado, por lo que hay un ciclo\n", node->neighbors[i]->id, node->neighbors[i]->x, node->neighbors[i]->y);
			return 1;
		}
	}
	return 0;
}

void check_if_graph_is_connected(Node** nodes, int nodes_count) {
	int connected = 1;
	for (int i = 0; i < nodes_count; i++) {
		if (nodes[i]->visited == 0) {
			connected = 0;
			break;
		}
	}
	if (connected) {
		printf("Graph is connected\n");
	}
	else {
		printf("Graph is not connected\n");
	}
}

Edge** find_minimum_mst_with_kruskal(Node** nodes, int nodes_count, FILE* output) {
	Edge** mst = (Edge**)malloc(sizeof(Edge*) * nodes_count - 1);
	int mst_count = 0;
	int edges_count = (nodes_count * (nodes_count - 1)) / 2;
	Edge** edges = generate_edges(nodes, nodes_count, edges_count);
	// Free all neighbors
	for (int i = 0; i < nodes_count; i++) {
		free_neighbors(nodes[i]);
		nodes[i]->neighbors_count = 0;
		nodes[i]->neighbors = NULL;
	}
	// printf("Edges count: %d\n", edges_count);
	// Sort edges by weight using quiksort
	quicksort(edges, 0, edges_count - 1);
	// printf("Edges sorted\n");

	// Add edges to mst
	for (int i = 0; i < edges_count; i++) {
		// printf("Index: %d; MST COUNT: %d\n", i, mst_count);
		if (mst_count == nodes_count - 1) {
			break;
		}
		mst[mst_count] = edges[i];
		mst_count++;
		Node* root = edges[i]->node1;
		add_neighbor(edges[i]->node1, edges[i]->node2);
		add_neighbor(edges[i]->node2, edges[i]->node1);
		// Check visted nodes
		int visited[nodes_count];
		for (int i = 0; i < nodes_count; i++) {
			visited[i] = 0;
		}
		int cylce = recursive_dfs(root, visited, root);
		if (cylce) {
			mst_count--;
			remove_neighbor(edges[i]->node1, edges[i]->node2);
			remove_neighbor(edges[i]->node2, edges[i]->node1);
			// printf("Cyclo encontrado, se elimina el edge\n");
		}
		else {
			// printf("Edge: (%d, %d) - (%d, %d), weight: %d; AGREGADO \n", edges[i]->node1->x, edges[i]->node1->y, edges[i]->node2->x, edges[i]->node2->y, edges[i]->weight);
		}
	}

	// minimize_edges_of_a_mst(mst, mst_count, nodes_count, nodes, edges);
	print_total_weight(mst, mst_count, output);
	print_mst(mst, mst_count, output);
	free_edges(edges, edges_count);
	free(edges);

	return mst;
}

void print_mst(Edge** mst, int mst_count, FILE* output) {
	for (int i = 0; i < mst_count; i++) {
		// printf("Edge: (%d, %d) - (%d, %d), weight: %d\n", mst[i]->node1->x, mst[i]->node1->y, mst[i]->node2->x, mst[i]->node2->y, mst[i]->weight);
		// printf("%d %d %d %d\n", mst[i]->node1->x, mst[i]->node1->y, mst[i]->node2->x, mst[i]->node2->y);
		fprintf(output, "%d %d %d %d\n", mst[i]->node1->x, mst[i]->node1->y, mst[i]->node2->x, mst[i]->node2->y);
	}
}

void print_total_weight(Edge** mst, int mst_count, FILE* output) {
	int total_weight = 0;
	for (int i = 0; i < mst_count; i++) {
		total_weight += mst[i]->weight;
	}
	fprintf(output, "%d\n", total_weight);
	// printf("%d\n", total_weight);
}

void free_mst(Edge** mst, int mst_count) {
	for (int i = 0; i < mst_count; i++) {
		free(mst[i]);
	}
	free(mst);
}

int recursive_dfs_improved(Node* root) {
	if (root->visited == 1) {
		return 1;
	}
	root->visited = 1;
	if (root->next != NULL) {
		return recursive_dfs_improved(root->next);
	}
	return 0;
}

void quicksort(Edge** edges, int left, int right) {
	if (left < right) {
		int pivot = partition(edges, left, right);
		quicksort(edges, left, pivot - 1);
		quicksort(edges, pivot + 1, right);
	}
}

int partition(Edge** edges, int left, int right) {
	int pivot = edges[right]->weight;
	int i = left - 1;
	for (int j = left; j < right; j++) {
		if (edges[j]->weight <= pivot) {
			i++;
			swap(edges, i, j);
		}
	}
	swap(edges, i + 1, right);
	return i + 1;
}

void swap(Edge** edges, int i, int j) {
	Edge* temp = edges[i];
	edges[i] = edges[j];
	edges[j] = temp;
}

void free_edges(Edge** edges, int edges_count) {
	for (int i = 0; i < edges_count; i++) {
		// printf("Freeing edge: (%d, %d) - (%d, %d), weight: %d\n", edges[i]->node1->x, edges[i]->node1->y, edges[i]->node2->x, edges[i]->node2->y, edges[i]->weight);
		free(edges[i]);
	}
}

void add_neighbor(Node* node, Node* neighbor) {
	if (node->neighbors_count == 0) {
		node->neighbors = (Node**)malloc(sizeof(Node*));
		node->neighbors[0] = neighbor;
		node->neighbors_count++;
	}
	else {
		node->neighbors = (Node**)realloc(node->neighbors, sizeof(Node*) * (node->neighbors_count + 1));
		node->neighbors[node->neighbors_count] = neighbor;
		node->neighbors_count++;
	}
}

void remove_neighbor(Node* node, Node* neighbor) {
	int index = -1;
	for (int i = 0; i < node->neighbors_count; i++) {
		if (node->neighbors[i] == neighbor) {
			index = i;
			break;
		}
	}
	if (index != -1) {
		for (int i = index; i < node->neighbors_count - 1; i++) {
			node->neighbors[i] = node->neighbors[i + 1];
		}
		node->neighbors_count--;
		node->neighbors = (Node**)realloc(node->neighbors, sizeof(Node*) * node->neighbors_count);
	}
}

void count_neighbors_mst(Node** nodes, int nodes_count) {
	int cero_neighbors = 0, uno_neighbors = 0, dos_neighbors = 0, tres_neighbors = 0, cuatro_neighbors = 0, cinco_neighbors = 0, seis_neighbors = 0;
	for (int i = 0; i < nodes_count; i++) {
		if (nodes[i]->neighbors_count == 0) {
			cero_neighbors++;
		}
		else if (nodes[i]->neighbors_count == 1) {
			uno_neighbors++;
		}
		else if (nodes[i]->neighbors_count == 2) {
			dos_neighbors++;
		}
		else if (nodes[i]->neighbors_count == 3) {
			tres_neighbors++;
		}
		else if (nodes[i]->neighbors_count == 4) {
			cuatro_neighbors++;
		}
		else if (nodes[i]->neighbors_count == 5) {
			cinco_neighbors++;
		}
		else if (nodes[i]->neighbors_count == 6) {
			seis_neighbors++;
		}
	}
	printf("0 neighbors: %d\n", cero_neighbors);
	printf("1 neighbors: %d\n", uno_neighbors);
	printf("2 neighbors: %d\n", dos_neighbors);
	printf("3 neighbors: %d\n", tres_neighbors);
	printf("4 neighbors: %d\n", cuatro_neighbors);
	printf("5 neighbors: %d\n", cinco_neighbors);
	printf("6 neighbors: %d\n", seis_neighbors);
}

void minimize_edges_of_a_mst(Edge** mst, int mst_count, int nodes_count, Node** nodes, Edge** edges) {
	int total_weight = 0;
	int edges_count = (nodes_count * (nodes_count - 1)) / 2;
	for (int i = 0; i < mst_count; i++) {
		total_weight += mst[i]->weight;
	}
	int Array_grados[nodes_count];
	int Array_ids_grados[nodes_count];
	for (int j = 0; j < nodes_count; j++) {
		Array_grados[j] = nodes[j]->neighbors_count;
	}
	for (int j = 0; j < nodes_count; j++) {
		Array_ids_grados[j] = j;
	}
	// Ordenar el array de grados de mayor a menor
	for (int j = 0; j < nodes_count; j++) {
		for (int k = 0; k < nodes_count - 1; k++) {
			if (Array_grados[k] < Array_grados[k + 1]) {
				int temp = Array_grados[k];
				Array_grados[k] = Array_grados[k + 1];
				Array_grados[k + 1] = temp;
				int temp2 = Array_ids_grados[k];
				Array_ids_grados[k] = Array_ids_grados[k + 1];
				Array_ids_grados[k + 1] = temp2;
			}
		}
	}
	for (int j = 0; j < nodes_count; j++) {
		int mst_count_optimized = 0;
		if (Array_grados[j] >= 3) {
			int id = Array_ids_grados[j];
			for (int s = 0; s < nodes[id]->neighbors_count; s++) {
				remove_neighbor(nodes[id], nodes[id]->neighbors[s]);
				remove_neighbor(nodes[id]->neighbors[s], nodes[id]);
			}
			for (int k = 0; k < edges_count; k++) {
				printf("Index: %d; MST COUNT: %d\n", k, mst_count_optimized);
				if (mst_count_optimized == nodes_count - 2) {
					break;
				}
				if (edges[k]->node1->id == id || edges[k]->node2->id == id) {
					continue;
				}
				mst[mst_count_optimized] = edges[k];
				mst_count_optimized++;
				Node* root = edges[k]->node1;
				add_neighbor(edges[k]->node1, edges[k]->node2);
				add_neighbor(edges[k]->node2, edges[k]->node1);
				// Check visted nodes
				int visited[nodes_count];
				for (int i = 0; i < nodes_count; i++) {
					visited[i] = 0;
				}
				int cylce = recursive_dfs(root, visited, root);
				if (cylce) {
					mst_count_optimized--;
					remove_neighbor(edges[k]->node1, edges[k]->node2);
					remove_neighbor(edges[k]->node2, edges[k]->node1);
					printf("Cyclo encontrado, se elimina el edge\n");
				}
				else {
					printf("Edge: (%d, %d) - (%d, %d), weight: %d; AGREGADO \n", edges[k]->node1->x, edges[k]->node1->y, edges[k]->node2->x, edges[k]->node2->y, edges[k]->weight);
				}
			}
			for (int k = 0; k < edges_count; k++) {
				if (edges[k]->node1->id == id || edges[k]->node2->id == id) {
					mst[mst_count_optimized] = edges[k];
					mst_count_optimized++;
					Node* root = edges[k]->node1;
					add_neighbor(edges[k]->node1, edges[k]->node2);
					add_neighbor(edges[k]->node2, edges[k]->node1);
					// Check visted nodes
					int visited[nodes_count];
					for (int i = 0; i < nodes_count; i++) {
						visited[i] = 0;
					}
					int cylce = recursive_dfs(root, visited, root);
					if (cylce) {
						mst_count_optimized--;
						remove_neighbor(edges[k]->node1, edges[k]->node2);
						remove_neighbor(edges[k]->node2, edges[k]->node1);
						printf("Cyclo encontrado, se elimina el edge\n");
					}
					else {
						printf("OPTIMIZAMOS EL NODO %d\n", id);
						printf("Edge: (%d, %d) - (%d, %d), weight: %d; AGREGADO \n", edges[k]->node1->x, edges[k]->node1->y, edges[k]->node2->x, edges[k]->node2->y, edges[k]->weight);
					}
				}
			}
		}
		else {
			printf("No se puede optimizar el nodo %d\n", Array_ids_grados[j]);
		}
		// Print arrays
		printf("Array grados: ");
		for (int j = 0; j < nodes_count; j++) {
			printf("%d ", Array_grados[j]);
		}
		printf("\n");
		printf("Array ids grados: ");
		for (int j = 0; j < nodes_count; j++) {
			printf("%d ", Array_ids_grados[j]);
		}
		printf("\n");
	}
}

Edge** generate_edges(Node** nodes, int nodes_count, int edges_count) {
	Edge** edges = (Edge**)malloc(sizeof(Edge*) * edges_count);
	int edges_index = 0;
	for (int i = 0; i < nodes_count; i++) {
		for (int j = i; j < nodes[i]->neighbors_count; j++) {
			edges[edges_index] = (Edge*)malloc(sizeof(Edge));
			edges[edges_index]->node1 = nodes[i];
			edges[edges_index]->node2 = nodes[i]->neighbors[j];
			edges[edges_index]->weight = abs(nodes[i]->x - nodes[i]->neighbors[j]->x) + abs(nodes[i]->y - nodes[i]->neighbors[j]->y);
			edges_index++;
		}
	}
	return edges;
}
