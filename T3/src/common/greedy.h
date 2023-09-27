#pragma once

#include <math.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Heap Struct

typedef struct node_greedy {
	int id;
	struct edge_greedy** edges;
	int edges_count;
} Node_greedy;

typedef struct edge_greedy {
	Node_greedy* node1;
	Node_greedy* node2;
	int weight;
} Edge_greedy;

typedef struct heap {
	Node_greedy** nodes;
	int size;
	int capacity;
} Heap;

Edge_greedy* create_edge_greedy(Node_greedy* node1, Node_greedy* node2, int weight);

Node_greedy* create_node_greedy(int id);

void add_edge_greedy(Node_greedy* node, Edge_greedy* edge);

void print_edges_greedy(Edge_greedy** edges, int edges_count);

void free_edges_greedy(Edge_greedy** edges, int edges_count);

void free_nodes_greedy(Node_greedy** nodes, int nodes_count);

void free_neighbors_greedy(Node_greedy* node);

int djikstra_algorithm(Node_greedy** nodes, int nodes_count, int start, int end);