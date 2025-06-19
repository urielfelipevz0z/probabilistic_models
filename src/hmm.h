#ifndef HMM_H
#define HMM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// State and observation definitions for weather prediction
#define SUNNY 0
#define CLOUDY 1  
#define RAINY 2

#define UMBRELLA 0
#define SUNGLASSES 1
#define STAY_HOME 2

// State and observation names for verbose output
extern const char* STATE_NAMES[3];
extern const char* OBSERVATION_NAMES[3];

/**
 * Hidden Markov Model structure
 * Contains all parameters needed for HMM operations
 */
typedef struct {
    int num_states;       // N = Number of hidden states
    int num_observations; // M = Number of possible observations  
    int sequence_length;  // T = Length of observation sequence
    double **transition;  // Matrix A (NxN) - transition probabilities A[i][j] = P(state_j | state_i)
    double **emission;    // Matrix B (NxM) - emission probabilities B[i][j] = P(obs_j | state_i)
    double *initial;      // Vector π (Nx1) - initial state probabilities π[i] = P(state_i)
} HMM;

/**
 * Viterbi algorithm result structure
 * Contains all computed matrices and the optimal path
 */
typedef struct {
    double **delta;       // Matrix δ (TxN) - maximum probabilities at each time step
    int **psi;           // Matrix ψ (TxN) - backpointer matrix for path reconstruction
    int *path;           // Optimal state sequence (Tx1) - most likely hidden states
    double probability;  // Final probability of optimal path P*
} ViterbiResult;

// =============================================================================
// MEMORY MANAGEMENT FUNCTIONS
// =============================================================================

/**
 * Allocate memory for HMM structure and initialize matrices
 * @param N Number of states
 * @param M Number of possible observations
 * @param T Length of observation sequence
 * @return Pointer to allocated HMM structure or NULL on failure
 */
HMM* allocate_hmm(int N, int M, int T);

/**
 * Free all memory allocated for HMM structure
 * @param hmm Pointer to HMM structure to free
 */
void free_hmm(HMM* hmm);

/**
 * Allocate memory for Viterbi result structure
 * @param T Length of observation sequence
 * @param N Number of states
 * @return Pointer to allocated ViterbiResult structure or NULL on failure
 */
ViterbiResult* allocate_viterbi_result(int T, int N);

/**
 * Free all memory allocated for ViterbiResult structure
 * @param result Pointer to ViterbiResult structure to free
 */
void free_viterbi_result(ViterbiResult* result);

// =============================================================================
// FILE I/O FUNCTIONS
// =============================================================================

/**
 * Load HMM parameters from file
 * Expected format:
 * Line 1: N (number of states)
 * Line 2: M (number of observations)  
 * Line 3: T (sequence length)
 * Lines 4-6: Transition matrix A (3x3)
 * Lines 7-9: Emission matrix B (3x3)
 * Line 10: Initial probabilities π (1x3)
 * Line 11: Observation sequence (1xT)
 * 
 * @param filename Path to input file
 * @return Pointer to loaded HMM structure or NULL on failure
 */
HMM* load_hmm(char* filename);

// =============================================================================
// CORE ALGORITHM FUNCTIONS
// =============================================================================

/**
 * Execute the Viterbi algorithm to find most likely state sequence
 * 
 * Algorithm phases:
 * 1. Initialization: δ₁(i) = π(i) × B(i,o₁), ψ₁(i) = 0
 * 2. Recursion: δₜ(i) = max[δₜ₋₁(j) × A(j,i)] × B(i,oₜ), ψₜ(i) = argmax[δₜ₋₁(j) × A(j,i)]
 * 3. Termination: P* = max[δₜ(i)], q*ₜ = argmax[δₜ(i)]
 * 4. Backtracking: q*ₜ = ψₜ₊₁(q*ₜ₊₁) for t = T-1 down to 1
 * 
 * @param hmm Pointer to HMM structure containing model parameters
 * @param observations Array of observed symbols (length T)
 * @return Pointer to ViterbiResult structure containing optimal path and probability
 */
ViterbiResult* viterbi_algorithm(HMM* hmm, int* observations);

// =============================================================================
// OUTPUT AND DEBUGGING FUNCTIONS
// =============================================================================

/**
 * Print detailed step-by-step Viterbi algorithm execution
 * @param result Pointer to ViterbiResult structure
 * @param hmm Pointer to HMM structure
 * @param observations Array of observed symbols
 * @param verbose Flag to enable detailed mathematical output
 */
void print_step_by_step(ViterbiResult* result, HMM* hmm, int* observations, int verbose);

/**
 * Print matrix in formatted table with headers
 * @param matrix 2D matrix to print
 * @param rows Number of rows
 * @param cols Number of columns
 * @param title Title to display above matrix
 */
void print_matrices_formatted(double** matrix, int rows, int cols, char* title);

/**
 * Print final results in specified format
 * @param result Pointer to ViterbiResult structure
 */
void print_final_results(ViterbiResult* result);

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

/**
 * Validate HMM parameters for correctness
 * - Check if probabilities are between 0 and 1
 * - Verify that transition matrix rows sum to 1.0
 * - Verify that emission matrix rows sum to 1.0  
 * - Check if initial probabilities sum to 1.0
 * @param hmm Pointer to HMM structure to validate
 * @return 1 if valid, 0 if invalid
 */
int validate_hmm(HMM* hmm);

/**
 * Validate observation sequence
 * - Check if all observations are valid symbols (0, 1, 2)
 * @param observations Array of observed symbols
 * @param length Length of observation sequence
 * @return 1 if valid, 0 if invalid
 */
int validate_observations(int* observations, int length);

/**
 * Run the complete HMM weather prediction example
 * Loads model from "clima_ejemplo.txt" and executes Viterbi algorithm
 * @param verbose Flag to enable detailed output
 * @return 0 on success, -1 on failure
 */
int run_weather_prediction_example(int verbose);

#endif // HMM_H
