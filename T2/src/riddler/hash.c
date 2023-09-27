#include "hash.h"

// Tree* create_tree(int index, int height, Tree* left, Tree* right) {
// 	Tree* tree = (Tree*)malloc(sizeof(Tree));
// 	tree->index = index;
// 	tree->height = height;
// 	tree->left = left;
// 	tree->right = right;
// 	return tree;
// }

List* new_list(int index, int height) {
	List* list = (List*)malloc(sizeof(List));
	list->index = index;
	list->height = height;
	list->next = NULL;
	return list;
}

int bit_to_integer(int number, int height, int max_height, int last_output) {
	int integer = last_output;
	integer += number * pow(2, max_height - height - 1);
	return integer;
}

void hash_tree(int* tree, List** hash_table, int* initialized_hash, int size, int max_height) {
	for (int i = 0; i < size; i++) {
		int last_output = 0;
		int height = log2(i + 1);
		if (max_height - height > 1) {
			for (int j = height; j < max_height; j++) {
				int suma = 0;
				int elements = pow(2, j - height);
				for (int s = 0; s < elements; s++) {
					int indice = (i + 1) * pow(2, j - height) - 1 + s;
					suma += tree[indice];
					if (tree[indice] == 1 && j == height) {
						last_output = bit_to_integer(tree[indice], j - height, max_height, last_output);
					}
					else if (tree[indice] == 1 && j != height) {
						last_output = bit_to_integer(tree[indice], j - height, max_height, last_output);
						create_subtree(initialized_hash, last_output, hash_table, i, j + 1 - height);
						break;
					}
				}
				if (suma == 0 && elements > 1) {
					create_subtree(initialized_hash, last_output, hash_table, i, j + 1 - height);
				}
			}
		}
	}
}

void create_subtree(int* initialized_hash, int last_output, List** hash_table, int i, int height) {
	if (initialized_hash[last_output] == 0) {
		initialized_hash[last_output] = 1;
		List* list = new_list(i, height);
		hash_table[last_output] = list;
	}
	else {
		List* list = new_list(i, height);
		List* aux = hash_table[last_output];
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = list;
	}
}

void hash_function(int* arbol, int size, int* initialized_hash, List** hash_table, FILE* file, int max_height, int* arbol_original) {
	int last_output = 0;
	int height = log2(size + 1);
	for (int i = 0; i < height; i++) {
		int elements = pow(2, i);
		for (int j = 0; j < elements; j++) {
			int indice = pow(2, i) - 1 + j;
			if (arbol[indice] == 1) {
				last_output = bit_to_integer(arbol[indice], i, max_height, last_output);
				break;
			}
		}
	}
	if (initialized_hash[last_output] == 0) {
		fprintf(file, "-1\n");
	}
	else if (initialized_hash[last_output] == 1) {
		int contador_true = 0;
		List* aux = hash_table[last_output];
		while (aux->next != NULL) {
			int indice = aux->index;
			int altura = aux->height;
			int check = check_if_is_same_tree(arbol, arbol_original, size, indice, altura, height);
			if (check == 1) {
				if (aux->next != NULL && contador_true == 0) {
					fprintf(file, "%d", indice + 1);
					aux = aux->next;
					contador_true++;
				}
				else if (aux->next != NULL && contador_true > 0) {
					fprintf(file, " %d", indice + 1);
					aux = aux->next;
					contador_true++;
				}
				else {
					fprintf(file, " %d\n", indice + 1);
					break;
				}
			}
			else {
				aux = aux->next;
			}
		}

		if (aux->next == NULL && contador_true == 0) {
			int indice = aux->index;
			int altura = aux->height;
			int check = check_if_is_same_tree(arbol, arbol_original, size, indice, altura, height);
			if (check == 0) {
				fprintf(file, "-1\n");
			}
			else if (check == 1) {
				fprintf(file, "%d\n", indice + 1);
			}
		}
		else if (aux->next == NULL && contador_true > 0) {
			int indice = aux->index;
			int altura = aux->height;
			int check = check_if_is_same_tree(arbol, arbol_original, size, indice, altura, height);
			if (check == 1) {
				fprintf(file, " %d\n", indice + 1);
			}
			else {
				fprintf(file, "\n");
			}
		}
	}
}

int check_if_is_same_tree(int* tree, int* tree2, int size, int indice, int height_tree1, int height_tree2) {
	if (height_tree1 == height_tree2) {
		for (int i = 0; i < height_tree1; i++) {
			int elements = pow(2, i);
			for (int j = 0; j < elements; j++) {
				int indice_tree1 = pow(2, i) - 1 + j;
				int indice_tree2 = (indice + 1) * pow(2, i) - 1 + j;
				if (tree[indice_tree1] != tree2[indice_tree2]) {
					return 0;
				}
			}
		}
	}
	else if (height_tree1 != height_tree2) {
		return 0;
	}
	return 1;
}

int bit_to_integer_array(int* bit, int size) {
	int integer = 0;
	for (int i = 0; i < size; i++) {
		integer += bit[i] * pow(2, size - i - 1);
	}
	return integer;
}

void free_hash_table(List* hash_table) {
	List* aux = hash_table;
	if (aux != NULL) {
		if (aux->next != NULL) {
			free_hash_table(aux->next);
		}
		free(aux);
	}
}
