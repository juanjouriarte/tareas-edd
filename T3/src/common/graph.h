#pragma once

#include <math.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	int id;
	int x;
	int y;
	int visited;
	struct node** neighbors;
	int neighbors_count;
	struct node* next;
	int priority;
	struct Lista_Neighbor* lista_neighbors;
} Node;

typedef struct edges {
	Node* node1;
	Node* node2;
	int weight;
} Edge;

Node* create_node(int id, int x, int y);

Node** connect_node_with_all_nodes(Node* node, Node** nodes, int nodes_count);

void destroy_node(Node* node);

void print_node(Node* node);

void print_all_node_neightbors(Node* node);

void free_neighbors(Node* node);

int recursive_dfs(Node* node, int* visited, Node* parent);

void check_if_graph_is_connected(Node** nodes, int nodes_count);

Edge** find_minimum_mst_with_kruskal(Node** nodes, int nodes_count, FILE* output);

void print_mst(Edge** mst, int mst_count, FILE* output);

void print_total_weight(Edge** mst, int mst_count, FILE* output);

void free_mst(Edge** mst, int mst_count);

int recursive_dfs_improved(Node* node);

void quicksort(Edge** edges, int left, int right);

int partition(Edge** edges, int left, int right);

void swap(Edge** edges, int i, int j);

void free_edges(Edge** edges, int edges_count);

void add_neighbor(Node* node, Node* neighbor);

void remove_neighbor(Node* node, Node* neighbor);

void count_neighbors_mst(Node** nodes, int nodes_count);

void minimize_edges_of_a_mst(Edge** mst, int mst_count, int nodes_count, Node** nodes, Edge** edges);

Edge** generate_edges(Node** nodes, int nodes_count, int edges_count);
