#define _POSIX_C_SOURCE 200112L  // For fileno()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For isatty() and fileno()
#include "bayesian.h"
#include "hmm.h"

void print_menu(void) {
    printf("\n=== MODELOS PROBABILISTAS ===\n");
    printf("1. Ejecutar ejemplo de Red Bayesiana\n");
    printf("2. Ejecutar ejemplo de HMM - Predicción del Clima (modo básico)\n");
    printf("3. Ejecutar ejemplo de HMM - Predicción del Clima (modo detallado)\n");
    printf("4. Salir\n");
    printf("Seleccione una opción: ");
}

int main() {
    int opcion;
    int continuar = 1;
    
    printf("=======================================================\n");
    printf("    PROGRAMA DE MODELOS PROBABILISTAS EN C\n");
    printf("=======================================================\n");
    printf("Este programa implementa:\n");
    printf("- Redes Bayesianas con tablas de probabilidad\n");
    printf("- Modelos Ocultos de Markov con algoritmo de Viterbi\n");
    printf("=======================================================\n");
    
    while (continuar) {
        print_menu();
        
        if (scanf("%d", &opcion) != 1) {
            // Check if we reached EOF (pipe closed)
            if (feof(stdin)) {
                printf("\nEntrada terminada. Saliendo del programa.\n");
                break;
            }
            printf("Error: Entrada inválida. Por favor ingrese un número.\n");
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        // Clear any remaining input, but check for EOF
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // If EOF was reached, break out of loop
        if (c == EOF) {
            continuar = 0;
        }
        
        
        switch (opcion) {
            case 1:
                printf("\n=== EJECUTANDO RED BAYESIANA ===\n");
                printf("A continuación podrá construir una red bayesiana interactivamente.\n");
                printf("Siga las instrucciones para definir variables, conexiones y probabilidades.\n\n");
                
                if (run_bayesian_network_example() == 0) {
                    printf("\n=== Red Bayesiana ejecutada exitosamente ===\n");
                } else {
                    printf("\n=== Error al ejecutar Red Bayesiana ===\n");
                }
                break;
                
            case 2:
                printf("\n=== EJECUTANDO HMM - PREDICCIÓN DEL CLIMA (MODO BÁSICO) ===\n");
                printf("Este ejemplo utiliza el algoritmo de Viterbi para predecir\n");
                printf("la secuencia más probable de estados del clima basada en\n");
                printf("observaciones de comportamiento humano.\n\n");
                
                if (run_weather_prediction_example(0) == 0) {
                    printf("\n=== HMM ejecutado exitosamente ===\n");
                } else {
                    printf("\n=== Error al ejecutar HMM ===\n");
                }
                break;
                
            case 3:
                printf("\n=== EJECUTANDO HMM - PREDICCIÓN DEL CLIMA (MODO DETALLADO) ===\n");
                printf("Este ejemplo muestra todos los cálculos paso a paso del\n");
                printf("algoritmo de Viterbi incluyendo matrices de transición,\n");
                printf("emisión, inicialización, recursión y backtracking.\n\n");
                
                if (run_weather_prediction_example(1) == 0) {
                    printf("\n=== HMM ejecutado exitosamente ===\n");
                } else {
                    printf("\n=== Error al ejecutar HMM ===\n");
                }
                break;
                
            case 4:
                printf("\nGracias por usar el programa de Modelos Probabilistas.\n");
                printf("¡Hasta la vista!\n");
                continuar = 0;
                break;
                
            default:
                printf("\nOpción inválida. Por favor seleccione una opción entre 1 y 4.\n");
                break;
        }
        
        if (continuar) {
            // Check if input is from a terminal (interactive) or pipe/redirect
            if (isatty(fileno(stdin))) {
                printf("\nPresione Enter para continuar...");
                getchar(); // Wait for user to press Enter
            } else {
                // If input is from pipe/redirect, don't wait for Enter
                printf("\n"); // Just add a newline for better formatting
            }
        }
    }
    
    return 0;
}
