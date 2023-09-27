#pragma once

#include <math.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct liged_list {
	int index;
	int height;
	struct liged_list* next;
} List;

// typedef struct tree {
// 	int index;
// 	int height;
// 	struct tree* left;
// 	struct tree* right;
// } Tree;

// Tree* create_tree(int index, int height, Tree* left, Tree* right);

int bit_to_integer(int number, int height, int max_height, int last_output);

void hash_tree(int* tree, List** hash_table, int* initialized_hash, int size, int max_height);

void create_subtree(int* initialized_hash, int last_output, List** hash_table, int i, int height);

int bit_to_integer_array(int* bit, int size);

List* new_list(int index, int height);

void free_hash_table(List* hash_table);

void hash_function(int* arbol, int size, int* initialized_hash, List** hash_table, FILE* file, int max_height, int* arbol_original);

int check_if_is_same_tree(int* tree, int* tree2, int size, int indice, int height_tree1, int height_tree2);
