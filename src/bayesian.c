#include "bayesian.h"

int run_bayesian_network_example(void){
    int n, i,nd, conx;
    char nombre[100], padre[100], hijo[100];
    float v, f;
    struct nodo *nodo = NULL;
    struct nodo *lista = NULL;

    printf("ingrese la cantidad de variables:");
    scanf("%d",&n);
    printf("\ningrese los nombres de las variables:\n");
    for(i=0;i < n;i++){
        scanf("%s", nombre);
        nodo = crear_nodo(i, nombre);
        insertar_nodo(&lista, nodo);
    }

    printf("\ndefina las conexiones entre las variables\nescriba -fin- si no hay mas conexiones\n");
    while(1){
        printf("\npadre:");
        scanf("%s", padre);
        if(strcmp(padre, "fin") == 0){
            break;
        }
        nd=buscar_padre(&lista, padre);
        
        if(nd != -1){
            printf("  -> hijo:");
            scanf("%s", hijo);
            nodo = crear_nodo(0, hijo);
            conx = insertar_hijos(&lista, nodo, nd);
            printf("\ningrese las probabilidades si es verdadero o falso(de 0 a 1):");
            printf("\nV::si %s entonces %s:", padre, hijo);
            scanf("%f", &v);
            printf("F::no %s entonces %s:", padre, hijo);
            scanf("%f", &f);
            struct probabilidad *nueva_prob = crear_nodo_probabilidad(hijo, v, f);
            insertar_probabilidad(&lista, nd, nueva_prob);
            
            if(conx == 0){
                printf("ya existe esa conexion");
            }

        }
        else{
            printf("\n!!!!!!!! no existe el padre, verifique: !!!!!!!!!!\n");
            imprimir_lista(lista);
        } 
    }
    imprimir_conexiones(lista);
    imprimir_tablas_probabilidad(lista);
    imprimir_lista(lista);
    imprimir_como_grafo(lista);

    return 0;
}

int insertar_hijos(struct nodo **lista, struct nodo *hijo, int padre){
    struct nodo *actual = *lista;
    while (actual != NULL) {
        if (padre == actual->id) {
            if (actual->hijos == NULL) {
                actual->hijos = hijo;
                return 1; 
            } else {
                struct nodo *hijos = actual->hijos;
                while (hijos != NULL) {
                    if (strcmp(hijo->name, hijos->name) == 0) {
                        return 0; 
                    } else if (hijos->sig == NULL) {
                        hijos->sig = hijo;
                        return 1; 
                    }
                    hijos = hijos->sig;
                }
            }
        }
        actual = actual->sig; 
    }
    return -1; 
}


int buscar_padre(struct nodo **lista,char *nombre){
    struct nodo *actual = *lista; 

    while (actual != NULL) {
        if (strcmp(actual->name, nombre) == 0) { 
            return actual->id; 
        }
        actual = actual->sig; 
    }

    return -1;
}


struct nodo * crear_nodo (int id, char *name){
    struct nodo *nodo= NULL;
    nodo =(struct nodo *) malloc (sizeof (struct nodo));
  
    if (NULL != nodo){
      nodo-> id= id;
      strcpy(nodo->name, name);
      nodo-> sig= NULL;
      nodo->hijos= NULL;
      nodo->tabla = NULL;
    }
    return nodo;
}

void insertar_nodo (struct nodo **lista, struct nodo *nodo){
	struct nodo *actual = *lista;
    struct nodo *anterior = NULL;
	if (*lista == NULL) {
        *lista = nodo;
        nodo->sig = NULL;
    } else {
        while (actual != NULL && nodo->id > actual->id) {
            anterior = actual;
            actual = actual->sig;
        }

        if (anterior == NULL) {
            nodo->sig = *lista;
            *lista = nodo;
        } else {
            anterior->sig = nodo;
            nodo->sig = actual;
		}
	}
}

void imprimir_lista(struct nodo *lista){

    while(lista != NULL){
      printf("%d [%s]   ",lista->id, lista->name);
      lista = lista-> sig;
    }
    printf("\n");
}

void imprimir_conexiones(struct nodo *lista) {
    while (lista != NULL) {
        printf("%s", lista->name);

        struct nodo *hijo = lista->hijos;
        if (hijo != NULL) {
            printf(" -> ");
            while (hijo != NULL) {
                printf("%s", hijo->name);
                if (hijo->sig != NULL) {
                    printf(", "); 
                }
                hijo = hijo->sig;
            }
        } else {
            printf(" -> (sin hijos)");
        }

        printf("\n");
        lista = lista->sig;
    }
}

struct probabilidad *crear_nodo_probabilidad(char *hijo, float v, float f) {
    struct probabilidad *nuevo = NULL;
    nuevo = (struct probabilidad *)malloc(sizeof(struct probabilidad));
    
    if (nuevo != NULL) {
        strcpy(nuevo->nombre_hijo, hijo);  
        nuevo->prob_v = v;       
        nuevo->prob_f = f;           
        nuevo->sig = NULL;         
    }
    return nuevo;
}

void insertar_probabilidad(struct nodo **lista, int id_padre, struct probabilidad *nuevo) {
    struct nodo *actual = *lista;
    
    // Buscar el nodo del padre
    while (actual != NULL) {
        if (actual->id == id_padre) {
            if (actual->tabla == NULL) {
                actual->tabla = nuevo;
            } else {
                struct probabilidad *temp = actual->tabla;
                while (temp->sig != NULL) {
                    temp = temp->sig;
                }
                temp->sig = nuevo; 
            }
            break;
        }
        actual = actual->sig;
    }
}

void imprimir_tablas_probabilidad(struct nodo *lista) {
    struct nodo *actual = lista;
    while (actual != NULL) {
        printf("\nTabla de probabilidades para la variable '%s':\n", actual->name);
        
        struct probabilidad *tabla = actual->tabla;
        if (tabla == NULL) {
            printf("  (Sin probabilidades registradas)\n");
        } else {
            printf("  Hijo           P(Verdadero)    P(Falso)\n");
            printf("-------------------------------------------\n");
            while (tabla != NULL) {
                printf("  %-14s %.6f      %.6f\n", tabla->nombre_hijo, tabla->prob_v, tabla->prob_f);
                tabla = tabla->sig;
            }
        }
        printf("\n");
        
        actual = actual->sig;
    }
}

void imprimir_como_grafo(struct nodo *lista) {
    FILE *file = fopen("grafo.dot", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo para escribir el grafo\n");
        return;
    }

    fprintf(file, "digraph G {\n");

    struct nodo *actual = lista;
    while (actual != NULL) {
        // Escribimos el nodo en formato DOT
        fprintf(file, "  \"%s\";\n", actual->name);

        // Escribimos las conexiones con los hijos
        struct nodo *hijo = actual->hijos;
        while (hijo != NULL) {
            fprintf(file, "  \"%s\" -> \"%s\";\n", actual->name, hijo->name);
            hijo = hijo->sig;
        }

        actual = actual->sig;
    }

    fprintf(file, "}\n");
    fclose(file);
    printf("Archivo DOT generado: grafo.dot\n");
}