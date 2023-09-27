#include "bst.h"

bst* bst_new(int value) {
	bst* tree = (bst*)calloc(1, sizeof(bst));
	tree->value = value;
	tree->depth = 0;
	return tree;
}

void bst_insert_node(bst* tree, int value) {
	if (value < tree->value) {
		if (tree->left == NULL) {
			tree->left = bst_new(value);
			tree->left->depth = tree->depth + 1;
		}
		else {
			bst_insert_node(tree->left, value);
		}
	}
	else {
		if (tree->right == NULL) {
			tree->right = bst_new(value);
			tree->right->depth = tree->depth + 1;
		}
		else {
			bst_insert_node(tree->right, value);
		}
	}
}

void print_inorder(bst* tree, FILE* output_file) {
	if (tree->left != NULL) {
		print_inorder(tree->left, output_file);
	}
	fprintf(output_file, "%d ", tree->value);
	if (tree->right != NULL) {
		print_inorder(tree->right, output_file);
	}
}

void delete_tree(bst* tree) {
	if (tree->left != NULL) {
		delete_tree(tree->left);
	}
	if (tree->right != NULL) {
		delete_tree(tree->right);
	}
	free(tree);
}

void print_bst_value_path(bst* tree, int value, FILE* output_file) {
	if (tree->value == value) {
		fprintf(output_file, "%d \n", tree->value);
		return;
	}
	else if (value < tree->value) {
		fprintf(output_file, "%d ", tree->value);
		if (tree->left != NULL) {
			return print_bst_value_path(tree->left, value, output_file);
		}
		else {
			fprintf(output_file, "X\n");
		}
	}
	else {
		fprintf(output_file, "%d ", tree->value);
		if (tree->right != NULL) {
			return print_bst_value_path(tree->right, value, output_file);
		}
		else {
			fprintf(output_file, "X\n");
		}
	}
}

bst* print_value_bst_depth(bst* tree, int value) {
	if (tree == NULL || tree->value == value) {
		printf("%d\n", tree->depth);
		return tree;
	}
	if (value < tree->value) {
		return print_value_bst_depth(tree->left, value);
	}
	else if (value >= tree->value) {
		return print_value_bst_depth(tree->right, value);
	}
}