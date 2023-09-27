#pragma once

#include <math.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../imagelib/imagelib.h"

typedef struct quadtree_node {
	// promedio de luminosidad
	double average_L;
	// promedio de canal a
	double average_a;
	// promedio de canal b
	double average_b;
	// acumulado de luminosidad
	double total_L;
	// acumulado de canal a
	double total_a;
	// acumulado de canal b
	double total_b;
	// total_L cuadrado
	double total_L2;
	// total_a cuadrado
	double total_a2;
	// total_b cuadrado
	double total_b2;
	// Promedio total
	double average;
	// desviación estándar de luminosidad
	double stddev_L;
	// desviación estándar de canal a
	double stddev_a;
	// desviación estándar de canal b
	double stddev_b;
	// desviacion estandar total
	double stddev;
	// Indica si el nodo es una hoja //
	int is_leaf;
	// with
	int width;
	// height
	int height;
	// Altura Quadtree
	double depth;
	// Nodo noroeste //
	struct quadtree_node* nw;
	// Nodo noreste //
	struct quadtree_node* ne;
	// Nodo sudoeste //
	struct quadtree_node* sw;
	// Nodo sureste //
	struct quadtree_node* se;
} Quadtree;

/** Crea un nuevo nodo de cuadrante */
Quadtree* quadtree_new();

/** Libera la memoria de un nodo de QuadTree */
void quadtree_node_free(Quadtree* node);

/** Libera la memoria de un QuadTree */
void quadtree_free(Quadtree* tree);

/** Crea un nuevo QuadTree a partir de una imagen */
Quadtree* quadtree_from_image(Image* img);

/** Crea un nuevo QuadTree a partir de una imagen */
void quadtree_from_image_recursive(Quadtree* tree, Image* img, int row, int col, int size);

/** Crea una imagen a partir de un QuadTree */
void quadtree_to_image(Quadtree* tree, Image* img);

/** Crea una imagen a partir de un QuadTree */
void quadtree_to_image_recursive(Quadtree* tree, Image* img, int row, int col, int size);

/** Filtrar un QuadTree segun alpha*/
void quadtree_filter(Quadtree* tree, double alpha);

/** Filtrar un QuadTree segun alpha*/
void quadtree_filter_recursive(Quadtree* tree, double alpha);

// Destruir un QuadTree
void quadtree_delete(Quadtree* tree);

// Encontrar Alpha que pruna el arbol en n hojas
double find_alpha_who_gives_n_leaves_using_binary_search(Quadtree* tree, int n);

int quadtree_count_leaves(Quadtree* tree, int alpha);

int quadtree_count_leaves_recursive(Quadtree* tree, int alpha);

Quadtree* quadtree_copy(Quadtree* tree);

void paint_tree_to_image(Quadtree* tree, Image* img, int row, int col, int size);

double calculate_standard_deviation(double average, double average_nw, double average_ne, double average_sw, double average_se);

double calculate_average_color(double average_nw, double average_ne, double average_sw, double average_se);

double calculate_standard_deviation_from_cuadrant_of_img(Image* img, int row, int col, int size);

int quadtree_filter_return_leaves(Quadtree* tree, double alpha);
