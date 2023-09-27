#include "quadtree.h"

// Crea un nuevo nodo Quadtree
Quadtree* quadtree_new() {
	Quadtree* node = malloc(sizeof(Quadtree));
	node->average_L = 0;
	node->average_a = 0;
	node->average_b = 0;
	node->average = 0;
	node->stddev_L = 0;
	node->stddev_a = 0;
	node->stddev_b = 0;
	node->total_L2 = 0;
	node->total_b2 = 0;
	node->total_a2 = 0;
	node->stddev = 0;
	node->depth = 0;
	node->is_leaf = 0;
	node->width = 0;
	node->height = 0;
	node->nw = NULL;
	node->ne = NULL;
	node->sw = NULL;
	node->se = NULL;
	return node;
}

/** Libera la memoria de un nodo de QuadTree */
void quadtree_node_free(Quadtree* node) {
	if (node->nw != NULL) {
		quadtree_node_free(node->nw);
	}
	if (node->ne != NULL) {
		quadtree_node_free(node->ne);
	}
	if (node->sw != NULL) {
		quadtree_node_free(node->sw);
	}
	if (node->se != NULL) {
		quadtree_node_free(node->se);
	}
	free(node);
}

/** Libera la memoria de un QuadTree */
void quadtree_free(Quadtree* tree) {
	quadtree_node_free(tree);
}

/** Crea un nuevo QuadTree a partir de una imagen */
Quadtree* quadtree_from_image(Image* img) {
	Quadtree* tree = quadtree_new();
	int img_size = img->width;
	quadtree_from_image_recursive(tree, img, 0, 0, img_size);
	return tree;
}

/** Crea un nuevo QuadTree a partir de una imagen */
void quadtree_from_image_recursive(Quadtree* tree, Image* img, int row, int col, int size) {
	// Caso base: el nodo es una hoja
	if (size == 1) {
		tree->is_leaf = 1;
		tree->average_L = img->pixels[row][col].L;
		tree->average_a = img->pixels[row][col].a;
		tree->average_b = img->pixels[row][col].b;
		tree->total_L = img->pixels[row][col].L;
		tree->total_a = img->pixels[row][col].a;
		tree->total_b = img->pixels[row][col].b;
		tree->stddev_L = 0;
		tree->stddev_a = 0;
		tree->stddev_b = 0;
		tree->total_a2 = tree->average_a * tree->average_a;
		tree->total_b2 = tree->average_b * tree->average_b;
		tree->total_L2 = tree->average_L * tree->average_L;
		tree->average = 0;
		tree->depth = 1;
		tree->width = size;
		tree->height = size;
		tree->stddev = 0;
		return;
	}
	// Caso recursivo: el nodo no es una hoja
	tree->is_leaf = 0;
	tree->nw = quadtree_new();
	tree->ne = quadtree_new();
	tree->sw = quadtree_new();
	tree->se = quadtree_new();
	int half_size = size / 2;
	quadtree_from_image_recursive(tree->nw, img, row, col, half_size);
	quadtree_from_image_recursive(tree->ne, img, row, col + half_size, half_size);
	quadtree_from_image_recursive(tree->sw, img, row + half_size, col, half_size);
	quadtree_from_image_recursive(tree->se, img, row + half_size, col + half_size, half_size);
	// depth
	tree->depth = tree->nw->depth * 4;
	// Calcula el promedio y la desviación estándar de los colores
	// Promedio
	tree->average_L = calculate_average_color(tree->nw->average_L, tree->ne->average_L, tree->sw->average_L, tree->se->average_L);
	tree->average_a = calculate_average_color(tree->nw->average_a, tree->ne->average_a, tree->sw->average_a, tree->se->average_a);
	tree->average_b = calculate_average_color(tree->nw->average_b, tree->ne->average_b, tree->sw->average_b, tree->se->average_b);
	// Promedio Total
	tree->average = (tree->average_L + tree->average_a + tree->average_b) / 3;
	// Acumulado total
	tree->total_L = tree->nw->total_L + tree->ne->total_L + tree->sw->total_L + tree->se->total_L;
	tree->total_a = tree->nw->total_a + tree->ne->total_a + tree->sw->total_a + tree->se->total_a;
	tree->total_b = tree->nw->total_b + tree->ne->total_b + tree->sw->total_b + tree->se->total_b;
	// Acumulado total al cuadrado
	tree->total_L2 = tree->nw->total_L2 + tree->ne->total_L2 + tree->sw->total_L2 + tree->se->total_L2;
	tree->total_a2 = tree->nw->total_a2 + tree->ne->total_a2 + tree->sw->total_a2 + tree->se->total_a2;
	tree->total_b2 = tree->nw->total_b2 + tree->ne->total_b2 + tree->sw->total_b2 + tree->se->total_b2;
	// Desviacion estandar Incremental Bien
	tree->stddev_L = sqrt((1 / tree->depth) * tree->total_L2 - pow((1 / tree->depth) * tree->total_L, 2));
	tree->stddev_a = sqrt((1 / tree->depth) * tree->total_a2 - pow((1 / tree->depth) * tree->total_a, 2));
	tree->stddev_b = sqrt((1 / tree->depth) * tree->total_b2 - pow((1 / tree->depth) * tree->total_b, 2));
	// Desviacion estandar Total
	tree->stddev = (tree->stddev_L + tree->stddev_a + tree->stddev_b) / 3;
	// Tamaño
	tree->width = size;
	tree->height = size;
}

// Image New
Image* image_new(int width, int height) {
	Image* img = (Image*)malloc(sizeof(Image));
	img->width = width;
	img->height = height;
	img->pixels = (Color**)malloc(sizeof(Color*) * height);
	for (int i = 0; i < height; i++) {
		img->pixels[i] = (Color*)malloc(sizeof(Color) * width);
	}
	return img;
}
/** Crea una imagen a partir de un QuadTree */
void quadtree_to_image(Quadtree* tree, Image* img) {
	quadtree_to_image_recursive(tree, img, 0, 0, tree->width);
}

/** Crea una imagen a partir de un QuadTree */
void quadtree_to_image_recursive(Quadtree* tree, Image* img, int row, int col, int size) {
	// Caso base: el nodo es una hoja
	if (tree->is_leaf) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				img->pixels[row + i][col + j].L = tree->average_L;
				img->pixels[row + i][col + j].a = tree->average_a;
				img->pixels[row + i][col + j].b = tree->average_b;
			}
		}
		return;
	}
	// Caso recursivo: el nodo no es una hoja
	int half_size = size / 2;
	quadtree_to_image_recursive(tree->nw, img, row, col, half_size);
	quadtree_to_image_recursive(tree->ne, img, row, col + half_size, half_size);
	quadtree_to_image_recursive(tree->sw, img, row + half_size, col, half_size);
	quadtree_to_image_recursive(tree->se, img, row + half_size, col + half_size, half_size);
}

// Filtrar un QuadTree segun el alpha
void quadtree_filter(Quadtree* tree, double alpha) {
	quadtree_filter_recursive(tree, alpha);
}

// Filtrar un QuadTree segun el alpha
void quadtree_filter_recursive(Quadtree* tree, double alpha) {
	// Caso base: el nodo es una hoja
	if (tree->is_leaf == 1) {
		return;
	}
	// Caso recursivo: el nodo no es una hoja
	if (tree->stddev <= alpha) {
		printf("Filtrando nodo (%d, %d) con desviacion estandar %f %f %f %f %f %f\n", tree->width, tree->height, tree->stddev, tree->average, tree->average_L, tree->average_b, tree->average_a, tree->depth);
		quadtree_delete(tree->nw);
		quadtree_delete(tree->ne);
		quadtree_delete(tree->sw);
		quadtree_delete(tree->se);
		tree->is_leaf = 1;
	}
	else {
		quadtree_filter_recursive(tree->nw, alpha);
		quadtree_filter_recursive(tree->ne, alpha);
		quadtree_filter_recursive(tree->sw, alpha);
		quadtree_filter_recursive(tree->se, alpha);
	}
}

// Destruir un QuadTree
void quadtree_delete(Quadtree* tree) {
	if (tree->is_leaf == 1) {
		free(tree);
		return;
	}
	quadtree_delete(tree->nw);
	quadtree_delete(tree->ne);
	quadtree_delete(tree->sw);
	quadtree_delete(tree->se);
	free(tree);
}

double find_alpha_who_gives_n_leaves_using_binary_search(Quadtree* tree, int n) {
	double min = 0;
	double max = 128;
	double mid = 0;
	double minimum_mid = 128;
	while (min <= max) {
		int leaves = 0;
		mid = (min + max) / 2;
		leaves = quadtree_filter_return_leaves(tree, mid);
		if (leaves < n) {
			max = mid - 1;
			if (mid < minimum_mid) {
				minimum_mid = mid;
			}
		}
		else {
			min = mid + 1;
		}
	}
	return minimum_mid;
}

int quadtree_count_leaves(Quadtree* tree, int alpha) {
	return quadtree_count_leaves_recursive(tree, alpha);
}

int quadtree_count_leaves_recursive(Quadtree* tree, int alpha) {
	// Caso base: el nodo es una hoja
	if (tree->is_leaf == 1) {
		return 1;
	}
	else {
		return quadtree_count_leaves_recursive(tree->nw, alpha) +
		       quadtree_count_leaves_recursive(tree->ne, alpha) +
		       quadtree_count_leaves_recursive(tree->sw, alpha) +
		       quadtree_count_leaves_recursive(tree->se, alpha);
	}
}

Quadtree* quadtree_copy(Quadtree* tree) {
	Quadtree* copy = (Quadtree*)malloc(sizeof(Quadtree));
	copy->is_leaf = tree->is_leaf;
	copy->average_L = tree->average_L;
	copy->average_a = tree->average_a;
	copy->average_b = tree->average_b;
	copy->stddev_L = tree->stddev_L;
	copy->stddev_a = tree->stddev_a;
	copy->stddev_b = tree->stddev_b;
	copy->stddev = tree->stddev;
	copy->width = tree->width;
	copy->height = tree->height;
	if (tree->is_leaf == 1) {
		return copy;
	}
	copy->nw = quadtree_copy(tree->nw);
	copy->ne = quadtree_copy(tree->ne);
	copy->sw = quadtree_copy(tree->sw);
	copy->se = quadtree_copy(tree->se);
	return copy;
}

void paint_tree_to_image(Quadtree* tree, Image* img, int row, int col, int size) {
	// Caso base: el nodo es una hoja
	if (tree->is_leaf == 1) {
		img_square_paint(img, tree->height, tree->width, size, (Color){.L = tree->average_L, .a = tree->average_a, .b = tree->average_b});
		return;
	}
	// Caso recursivo: el nodo no es una hoja
	int half_size = size / 2;
	paint_tree_to_image(tree->nw, img, row, col, half_size);
	paint_tree_to_image(tree->ne, img, row, col + half_size, half_size);
	paint_tree_to_image(tree->sw, img, row + half_size, col, half_size);
	paint_tree_to_image(tree->se, img, row + half_size, col + half_size, half_size);
}

double calculate_standard_deviation(double average, double average_nw, double average_ne, double average_sw, double average_se) {
	double desviacion_estandar = 0;
	desviacion_estandar = pow(average_nw - average, 2) + pow(average_ne - average, 2) + pow(average_sw - average, 2) + pow(average_se - average, 2);
	return sqrt(desviacion_estandar / 4);
}

double calculate_average_color(double average_nw, double average_ne, double average_sw, double average_se) {
	return (average_nw + average_ne + average_sw + average_se) / 4;
}

double calculate_standard_deviation_from_cuadrant_of_img(Image* img, int row, int col, int size) {
	double average_L = 0;
	double average_a = 0;
	double average_b = 0;
	double desviacion_estandar_L = 0;
	double desviacion_estandar_a = 0;
	double desviacion_estandar_b = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Color color = img->pixels[row + i][col + j];
			average_L += color.L;
			average_a += color.a;
			average_b += color.b;
		}
	}
	average_L /= size * size;
	average_a /= size * size;
	average_b /= size * size;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Color color = img->pixels[row + i][col + j];
			desviacion_estandar_L += pow(color.L - average_L, 2);
			desviacion_estandar_a += pow(color.a - average_a, 2);
			desviacion_estandar_b += pow(color.b - average_b, 2);
		}
	}
	desviacion_estandar_L /= size * size;
	desviacion_estandar_a /= size * size;
	desviacion_estandar_b /= size * size;
	desviacion_estandar_L = sqrt(desviacion_estandar_L);
	desviacion_estandar_a = sqrt(desviacion_estandar_a);
	desviacion_estandar_b = sqrt(desviacion_estandar_b);
	return (desviacion_estandar_L + desviacion_estandar_a + desviacion_estandar_b) / 3;
}

int quadtree_filter_return_leaves(Quadtree* tree, double alpha) {
	if (tree->is_leaf == 1) {
		return 1;
	}
	if (tree->stddev <= alpha) {
		return 1;
	}
	else {
		return quadtree_filter_return_leaves(tree->nw, alpha) +
		       quadtree_filter_return_leaves(tree->ne, alpha) +
		       quadtree_filter_return_leaves(tree->sw, alpha) +
		       quadtree_filter_return_leaves(tree->se, alpha);
	}
}