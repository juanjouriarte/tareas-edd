#pragma once

#include <math.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct binary_search_tree {
	int value;
	int depth;
	struct binary_search_tree* left;
	struct binary_search_tree* right;
} bst;

bst* bst_new(int value);

void bst_insert_node(bst* tree, int value);

void print_inorder(bst* tree, FILE* output_file);

void delete_tree(bst* tree);

void print_bst_value_path(bst* tree, int value, FILE* output_file);

bst* print_value_bst_depth(bst* tree, int value);
