#ifndef BAYESIAN_H
#define BAYESIAN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nodo {
    int id;
    char name[100];
    struct nodo *sig;
    struct nodo *hijos;
    struct probabilidad *tabla;
};

struct probabilidad {
    char nombre_hijo[100];
    float prob_v; 
    float prob_f; 
    struct probabilidad *sig;
};

// Function declarations for Bayesian network functionality
struct nodo * crear_nodo (int id, char *name);
void insertar_nodo (struct nodo **lista, struct nodo *nodo);
void imprimir_lista(struct nodo *lista);
int buscar_padre(struct nodo **lista,char *nombre);
int insertar_hijos(struct nodo **lista,struct nodo *hijo, int padre);
void imprimir_conexiones(struct nodo *lista);
struct probabilidad *crear_nodo_probabilidad(char *hijo, float v, float f);
void insertar_probabilidad(struct nodo **lista, int id_padre, struct probabilidad *nuevo);
void imprimir_tablas_probabilidad(struct nodo *lista);
void imprimir_como_grafo(struct nodo *lista);

// Main function for Bayesian network example
int run_bayesian_network_example(void);

#endif // BAYESIAN_H
