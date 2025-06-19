#include "hmm.h"

// Global arrays for state and observation names (for verbose output)
const char* STATE_NAMES[3] = {"SUNNY", "CLOUDY", "RAINY"};
const char* OBSERVATION_NAMES[3] = {"UMBRELLA", "SUNGLASSES", "STAY_HOME"};

// =============================================================================
// MEMORY MANAGEMENT FUNCTIONS
// =============================================================================

HMM* allocate_hmm(int N, int M, int T) {
    // Validate input parameters
    if (N <= 0 || M <= 0 || T <= 0) {
        fprintf(stderr, "Error: Invalid HMM dimensions (N=%d, M=%d, T=%d)\n", N, M, T);
        return NULL;
    }
    
    // Allocate main HMM structure
    HMM* hmm = (HMM*)malloc(sizeof(HMM));
    if (hmm == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for HMM structure\n");
        return NULL;
    }
    
    // Initialize dimensions
    hmm->num_states = N;
    hmm->num_observations = M;
    hmm->sequence_length = T;
    
    // Allocate transition matrix A (NxN)
    hmm->transition = (double**)malloc(N * sizeof(double*));
    if (hmm->transition == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for transition matrix\n");
        free(hmm);
        return NULL;
    }
    
    for (int i = 0; i < N; i++) {
        hmm->transition[i] = (double*)malloc(N * sizeof(double));
        if (hmm->transition[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for transition matrix row %d\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(hmm->transition[j]);
            }
            free(hmm->transition);
            free(hmm);
            return NULL;
        }
    }
    
    // Allocate emission matrix B (NxM)
    hmm->emission = (double**)malloc(N * sizeof(double*));
    if (hmm->emission == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for emission matrix\n");
        // Free transition matrix
        for (int i = 0; i < N; i++) {
            free(hmm->transition[i]);
        }
        free(hmm->transition);
        free(hmm);
        return NULL;
    }
    
    for (int i = 0; i < N; i++) {
        hmm->emission[i] = (double*)malloc(M * sizeof(double));
        if (hmm->emission[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for emission matrix row %d\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(hmm->emission[j]);
            }
            free(hmm->emission);
            // Free transition matrix
            for (int i = 0; i < N; i++) {
                free(hmm->transition[i]);
            }
            free(hmm->transition);
            free(hmm);
            return NULL;
        }
    }
    
    // Allocate initial probability vector π (Nx1)
    hmm->initial = (double*)malloc(N * sizeof(double));
    if (hmm->initial == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for initial probabilities\n");
        // Free emission matrix
        for (int i = 0; i < N; i++) {
            free(hmm->emission[i]);
        }
        free(hmm->emission);
        // Free transition matrix
        for (int i = 0; i < N; i++) {
            free(hmm->transition[i]);
        }
        free(hmm->transition);
        free(hmm);
        return NULL;
    }
    
    return hmm;
}

void free_hmm(HMM* hmm) {
    if (hmm == NULL) return;
    
    // Free transition matrix
    if (hmm->transition != NULL) {
        for (int i = 0; i < hmm->num_states; i++) {
            if (hmm->transition[i] != NULL) {
                free(hmm->transition[i]);
            }
        }
        free(hmm->transition);
    }
    
    // Free emission matrix
    if (hmm->emission != NULL) {
        for (int i = 0; i < hmm->num_states; i++) {
            if (hmm->emission[i] != NULL) {
                free(hmm->emission[i]);
            }
        }
        free(hmm->emission);
    }
    
    // Free initial probabilities
    if (hmm->initial != NULL) {
        free(hmm->initial);
    }
    
    // Free main structure
    free(hmm);
}

ViterbiResult* allocate_viterbi_result(int T, int N) {
    // Validate input parameters
    if (T <= 0 || N <= 0) {
        fprintf(stderr, "Error: Invalid Viterbi result dimensions (T=%d, N=%d)\n", T, N);
        return NULL;
    }
    
    // Allocate main ViterbiResult structure
    ViterbiResult* result = (ViterbiResult*)malloc(sizeof(ViterbiResult));
    if (result == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for ViterbiResult structure\n");
        return NULL;
    }
    
    // Initialize probability
    result->probability = 0.0;
    
    // Allocate delta matrix δ (TxN)
    result->delta = (double**)malloc(T * sizeof(double*));
    if (result->delta == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for delta matrix\n");
        free(result);
        return NULL;
    }
    
    for (int t = 0; t < T; t++) {
        result->delta[t] = (double*)malloc(N * sizeof(double));
        if (result->delta[t] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for delta matrix row %d\n", t);
            // Free previously allocated rows
            for (int i = 0; i < t; i++) {
                free(result->delta[i]);
            }
            free(result->delta);
            free(result);
            return NULL;
        }
    }
    
    // Allocate psi matrix ψ (TxN)
    result->psi = (int**)malloc(T * sizeof(int*));
    if (result->psi == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for psi matrix\n");
        // Free delta matrix
        for (int t = 0; t < T; t++) {
            free(result->delta[t]);
        }
        free(result->delta);
        free(result);
        return NULL;
    }
    
    for (int t = 0; t < T; t++) {
        result->psi[t] = (int*)malloc(N * sizeof(int));
        if (result->psi[t] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for psi matrix row %d\n", t);
            // Free previously allocated rows
            for (int i = 0; i < t; i++) {
                free(result->psi[i]);
            }
            free(result->psi);
            // Free delta matrix
            for (int t = 0; t < T; t++) {
                free(result->delta[t]);
            }
            free(result->delta);
            free(result);
            return NULL;
        }
    }
    
    // Allocate path array (Tx1)
    result->path = (int*)malloc(T * sizeof(int));
    if (result->path == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for path array\n");
        // Free psi matrix
        for (int t = 0; t < T; t++) {
            free(result->psi[t]);
        }
        free(result->psi);
        // Free delta matrix
        for (int t = 0; t < T; t++) {
            free(result->delta[t]);
        }
        free(result->delta);
        free(result);
        return NULL;
    }
    
    return result;
}

void free_viterbi_result(ViterbiResult* result) {
    if (result == NULL) return;
    
    // Free delta matrix
    if (result->delta != NULL) {
        // Note: We need to know T to free properly, but it's not stored in the structure
        // This assumes the caller will manage the dimensions properly
        // In a production system, we might want to store T and N in the structure
        free(result->delta);
    }
    
    // Free psi matrix
    if (result->psi != NULL) {
        free(result->psi);
    }
    
    // Free path array
    if (result->path != NULL) {
        free(result->path);
    }
    
    // Free main structure
    free(result);
}

// Enhanced free function that properly deallocates matrices
void free_viterbi_result_enhanced(ViterbiResult* result, int T) {
    if (result == NULL) return;
    
    // Free delta matrix
    if (result->delta != NULL) {
        for (int t = 0; t < T; t++) {
            if (result->delta[t] != NULL) {
                free(result->delta[t]);
            }
        }
        free(result->delta);
    }
    
    // Free psi matrix
    if (result->psi != NULL) {
        for (int t = 0; t < T; t++) {
            if (result->psi[t] != NULL) {
                free(result->psi[t]);
            }
        }
        free(result->psi);
    }
    
    // Free path array
    if (result->path != NULL) {
        free(result->path);
    }
    
    // Free main structure
    free(result);
}

// =============================================================================
// FILE I/O FUNCTIONS
// =============================================================================

HMM* load_hmm(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    int N, M, T;
    
    // Read dimensions
    if (fscanf(file, "%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "Error: Invalid number of states in file\n");
        fclose(file);
        return NULL;
    }
    
    if (fscanf(file, "%d", &M) != 1 || M <= 0) {
        fprintf(stderr, "Error: Invalid number of observations in file\n");
        fclose(file);
        return NULL;
    }
    
    if (fscanf(file, "%d", &T) != 1 || T <= 0) {
        fprintf(stderr, "Error: Invalid sequence length in file\n");
        fclose(file);
        return NULL;
    }
    
    // Allocate HMM structure
    HMM* hmm = allocate_hmm(N, M, T);
    if (hmm == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Read transition matrix A (NxN)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fscanf(file, "%lf", &hmm->transition[i][j]) != 1) {
                fprintf(stderr, "Error: Failed to read transition matrix element [%d][%d]\n", i, j);
                free_hmm(hmm);
                fclose(file);
                return NULL;
            }
        }
    }
    
    // Read emission matrix B (NxM)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (fscanf(file, "%lf", &hmm->emission[i][j]) != 1) {
                fprintf(stderr, "Error: Failed to read emission matrix element [%d][%d]\n", i, j);
                free_hmm(hmm);
                fclose(file);
                return NULL;
            }
        }
    }
    
    // Read initial probabilities π (Nx1)
    for (int i = 0; i < N; i++) {
        if (fscanf(file, "%lf", &hmm->initial[i]) != 1) {
            fprintf(stderr, "Error: Failed to read initial probability for state %d\n", i);
            free_hmm(hmm);
            fclose(file);
            return NULL;
        }
    }
    
    fclose(file);
    
    // Validate loaded HMM
    if (!validate_hmm(hmm)) {
        fprintf(stderr, "Error: Loaded HMM failed validation\n");
        free_hmm(hmm);
        return NULL;
    }
    
    return hmm;
}

// =============================================================================
// CORE ALGORITHM FUNCTIONS
// =============================================================================

ViterbiResult* viterbi_algorithm(HMM* hmm, int* observations) {
    if (hmm == NULL || observations == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to viterbi_algorithm\n");
        return NULL;
    }
    
    // Validate observations
    if (!validate_observations(observations, hmm->sequence_length)) {
        fprintf(stderr, "Error: Invalid observation sequence\n");
        return NULL;
    }
    
    int N = hmm->num_states;
    int T = hmm->sequence_length;
    
    // Allocate result structure
    ViterbiResult* result = allocate_viterbi_result(T, N);
    if (result == NULL) {
        return NULL;
    }
    
    // ==========================================================================
    // PHASE 1: INITIALIZATION (t=1)
    // δ₁(i) = π(i) × B(i, o₁)
    // ψ₁(i) = 0
    // ==========================================================================
    
    for (int i = 0; i < N; i++) {
        // δ₁(i) = π(i) × B(i, o₁)
        result->delta[0][i] = hmm->initial[i] * hmm->emission[i][observations[0]];
        // ψ₁(i) = 0 (no previous state for first time step)
        result->psi[0][i] = 0;
    }
    
    // ==========================================================================
    // PHASE 2: RECURSION (t=2 to T)
    // δₜ(i) = max[δₜ₋₁(j) × A(j,i)] × B(i,oₜ)
    // ψₜ(i) = argmax[δₜ₋₁(j) × A(j,i)]
    // ==========================================================================
    
    for (int t = 1; t < T; t++) {
        for (int i = 0; i < N; i++) {
            // Find maximum over all previous states j
            double max_prob = -1.0;
            int best_prev_state = 0;
            
            for (int j = 0; j < N; j++) {
                // Calculate δₜ₋₁(j) × A(j,i)
                double prob = result->delta[t-1][j] * hmm->transition[j][i];
                
                if (prob > max_prob) {
                    max_prob = prob;
                    best_prev_state = j;
                }
            }
            
            // δₜ(i) = max[δₜ₋₁(j) × A(j,i)] × B(i,oₜ)
            result->delta[t][i] = max_prob * hmm->emission[i][observations[t]];
            
            // ψₜ(i) = argmax[δₜ₋₁(j) × A(j,i)]
            result->psi[t][i] = best_prev_state;
        }
    }
    
    // ==========================================================================
    // PHASE 3: TERMINATION
    // P* = max[δₜ(i)]
    // q*ₜ = argmax[δₜ(i)]
    // ==========================================================================
    
    double max_final_prob = -1.0;
    int best_final_state = 0;
    
    for (int i = 0; i < N; i++) {
        if (result->delta[T-1][i] > max_final_prob) {
            max_final_prob = result->delta[T-1][i];
            best_final_state = i;
        }
    }
    
    result->probability = max_final_prob;
    result->path[T-1] = best_final_state;
    
    // ==========================================================================
    // PHASE 4: BACKTRACKING
    // q*ₜ = ψₜ₊₁(q*ₜ₊₁) for t = T-1 down to 1
    // ==========================================================================
    
    for (int t = T-2; t >= 0; t--) {
        result->path[t] = result->psi[t+1][result->path[t+1]];
    }
    
    return result;
}

// =============================================================================
// OUTPUT AND DEBUGGING FUNCTIONS
// =============================================================================

void print_step_by_step(ViterbiResult* result, HMM* hmm, int* observations, int verbose) {
    if (result == NULL || hmm == NULL || observations == NULL) {
        printf("Error: NULL pointer in print_step_by_step\n");
        return;
    }
    
    int N = hmm->num_states;
    int T = hmm->sequence_length;
    
    printf("VITERBI ALGORITHM - WEATHER PREDICTION\n");
    printf("=====================================\n\n");
    
    printf("INPUT PARAMETERS:\n");
    printf("States: SUNNY(0), CLOUDY(1), RAINY(2)\n");
    printf("Observations: UMBRELLA(0), SUNGLASSES(1), STAY_HOME(2)\n");
    printf("Sequence: [");
    for (int t = 0; t < T; t++) {
        printf("%s", OBSERVATION_NAMES[observations[t]]);
        if (t < T-1) printf(", ");
    }
    printf("]\n\n");
    
    if (verbose) {
        // Print transition matrix
        printf("TRANSITION MATRIX A:\n");
        printf("        SUNNY   CLOUDY  RAINY\n");
        for (int i = 0; i < N; i++) {
            printf("%-7s ", STATE_NAMES[i]);
            for (int j = 0; j < N; j++) {
                printf("%.3f   ", hmm->transition[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        
        // Print emission matrix
        printf("EMISSION MATRIX B:\n");
        printf("        UMBRELLA SUNGLASSES STAY_HOME\n");
        for (int i = 0; i < N; i++) {
            printf("%-7s ", STATE_NAMES[i]);
            for (int j = 0; j < hmm->num_observations; j++) {
                printf("%.3f    ", hmm->emission[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        
        // Print initialization step
        printf("INITIALIZATION (t=1, obs=%s):\n", OBSERVATION_NAMES[observations[0]]);
        for (int i = 0; i < N; i++) {
            printf("δ₁(%s)  = π(%s)  × B(%s,%s)  = %.3f × %.3f = %.3f\n",
                   STATE_NAMES[i], STATE_NAMES[i], STATE_NAMES[i], 
                   OBSERVATION_NAMES[observations[0]],
                   hmm->initial[i], hmm->emission[i][observations[0]], 
                   result->delta[0][i]);
        }
        printf("\n");
        
        // Print delta matrix for first step
        printf("DELTA MATRIX (t=1):\n");
        printf("        SUNNY   CLOUDY  RAINY\n");
        printf("t=1     ");
        for (int i = 0; i < N; i++) {
            printf("%.3f   ", result->delta[0][i]);
        }
        printf("\n\n");
        
        // Print recursion steps
        printf("RECURSION:\n");
        for (int t = 1; t < T; t++) {
            printf("t=%d, observation=%s(%d):\n", t+1, OBSERVATION_NAMES[observations[t]], observations[t]);
            
            for (int i = 0; i < N; i++) {
                printf("For %s:  max{", STATE_NAMES[i]);
                for (int j = 0; j < N; j++) {
                    printf("%.3f×%.1f", result->delta[t-1][j], hmm->transition[j][i]);
                    if (j < N-1) printf(", ");
                }
                
                // Find the maximum for display
                double max_transition = -1.0;
                for (int j = 0; j < N; j++) {
                    double prob = result->delta[t-1][j] * hmm->transition[j][i];
                    if (prob > max_transition) {
                        max_transition = prob;
                    }
                }
                
                printf("} × %.1f = max{", hmm->emission[i][observations[t]]);
                for (int j = 0; j < N; j++) {
                    printf("%.3f", result->delta[t-1][j] * hmm->transition[j][i]);
                    if (j < N-1) printf(", ");
                }
                printf("} × %.1f = %.3f\n", hmm->emission[i][observations[t]], result->delta[t][i]);
            }
            printf("\n");
        }
        
        // Print termination
        printf("TERMINATION:\n");
        for (int i = 0; i < N; i++) {
            printf("δ₇(%s)  = %.6f\n", STATE_NAMES[i], result->delta[T-1][i]);
        }
        printf("Maximum probability: %.6f\n", result->probability);
        printf("Optimal final state: %s\n\n", STATE_NAMES[result->path[T-1]]);
        
        // Print backtracking
        printf("BACKTRACKING:\n");
        for (int t = T-1; t >= 0; t--) {
            printf("t=%d: state = %s\n", t+1, STATE_NAMES[result->path[t]]);
        }
        printf("\n");
    }
}

void print_matrices_formatted(double** matrix, int rows, int cols, char* title) {
    if (matrix == NULL || title == NULL) {
        printf("Error: NULL pointer in print_matrices_formatted\n");
        return;
    }
    
    printf("%s:\n", title);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.6f  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_final_results(ViterbiResult* result) {
    if (result == NULL) {
        printf("Error: NULL result in print_final_results\n");
        return;
    }
    
    printf("FINAL RESULTS:\n");
    printf("Optimal state sequence: [");
    
    // We need to know T, but it's not stored in result structure
    // For the weather example, we know T=7
    int T = 7; // This should ideally be passed as parameter
    
    for (int t = 0; t < T; t++) {
        printf("%d", result->path[t]);
        if (t < T-1) printf(", ");
    }
    printf("]\n");
    
    printf("Translation: [");
    for (int t = 0; t < T; t++) {
        printf("%s", STATE_NAMES[result->path[t]]);
        if (t < T-1) printf(", ");
    }
    printf("]\n");
    
    printf("Maximum probability: %.10f\n", result->probability);
}

// Enhanced version that takes sequence length as parameter
void print_final_results_enhanced(ViterbiResult* result, int T) {
    if (result == NULL) {
        printf("Error: NULL result in print_final_results\n");
        return;
    }
    
    printf("FINAL RESULTS:\n");
    printf("Optimal state sequence: [");
    
    for (int t = 0; t < T; t++) {
        printf("%d", result->path[t]);
        if (t < T-1) printf(", ");
    }
    printf("]\n");
    
    printf("Translation: [");
    for (int t = 0; t < T; t++) {
        printf("%s", STATE_NAMES[result->path[t]]);
        if (t < T-1) printf(", ");
    }
    printf("]\n");
    
    printf("Maximum probability: %.10f\n", result->probability);
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

int validate_hmm(HMM* hmm) {
    if (hmm == NULL) {
        fprintf(stderr, "Validation error: HMM is NULL\n");
        return 0;
    }
    
    const double TOLERANCE = 1e-6;
    
    // Validate dimensions
    if (hmm->num_states <= 0 || hmm->num_observations <= 0 || hmm->sequence_length <= 0) {
        fprintf(stderr, "Validation error: Invalid dimensions\n");
        return 0;
    }
    
    // Validate transition matrix
    for (int i = 0; i < hmm->num_states; i++) {
        double row_sum = 0.0;
        for (int j = 0; j < hmm->num_states; j++) {
            if (hmm->transition[i][j] < 0.0 || hmm->transition[i][j] > 1.0) {
                fprintf(stderr, "Validation error: Transition probability [%d][%d] = %.6f is not in [0,1]\n", 
                        i, j, hmm->transition[i][j]);
                return 0;
            }
            row_sum += hmm->transition[i][j];
        }
        if (fabs(row_sum - 1.0) > TOLERANCE) {
            fprintf(stderr, "Validation error: Transition matrix row %d sums to %.6f instead of 1.0\n", 
                    i, row_sum);
            return 0;
        }
    }
    
    // Validate emission matrix
    for (int i = 0; i < hmm->num_states; i++) {
        double row_sum = 0.0;
        for (int j = 0; j < hmm->num_observations; j++) {
            if (hmm->emission[i][j] < 0.0 || hmm->emission[i][j] > 1.0) {
                fprintf(stderr, "Validation error: Emission probability [%d][%d] = %.6f is not in [0,1]\n", 
                        i, j, hmm->emission[i][j]);
                return 0;
            }
            row_sum += hmm->emission[i][j];
        }
        if (fabs(row_sum - 1.0) > TOLERANCE) {
            fprintf(stderr, "Validation error: Emission matrix row %d sums to %.6f instead of 1.0\n", 
                    i, row_sum);
            return 0;
        }
    }
    
    // Validate initial probabilities
    double initial_sum = 0.0;
    for (int i = 0; i < hmm->num_states; i++) {
        if (hmm->initial[i] < 0.0 || hmm->initial[i] > 1.0) {
            fprintf(stderr, "Validation error: Initial probability [%d] = %.6f is not in [0,1]\n", 
                    i, hmm->initial[i]);
            return 0;
        }
        initial_sum += hmm->initial[i];
    }
    if (fabs(initial_sum - 1.0) > TOLERANCE) {
        fprintf(stderr, "Validation error: Initial probabilities sum to %.6f instead of 1.0\n", initial_sum);
        return 0;
    }
    
    return 1; // All validations passed
}

int validate_observations(int* observations, int length) {
    if (observations == NULL) {
        fprintf(stderr, "Validation error: Observations array is NULL\n");
        return 0;
    }
    
    for (int t = 0; t < length; t++) {
        if (observations[t] < 0 || observations[t] > 2) {
            fprintf(stderr, "Validation error: Observation [%d] = %d is not a valid symbol (0, 1, or 2)\n", 
                    t, observations[t]);
            return 0;
        }
    }
    
    return 1; // All observations are valid
}

int run_weather_prediction_example(int verbose) {
    printf("=== HMM WEATHER PREDICTION EXAMPLE ===\n\n");
    
    // Load HMM from file
    HMM* hmm = load_hmm("clima_ejemplo.txt");
    if (hmm == NULL) {
        printf("Failed to load HMM from file. Make sure 'clima_ejemplo.txt' exists.\n");
        return -1;
    }
    
    printf("Successfully loaded HMM with %d states, %d observations, sequence length %d\n\n", 
           hmm->num_states, hmm->num_observations, hmm->sequence_length);
    
    // Read observation sequence from file (already loaded in hmm structure)
    // For this implementation, we'll read the observations separately
    FILE* file = fopen("clima_ejemplo.txt", "r");
    if (file == NULL) {
        printf("Error: Cannot reopen file to read observations\n");
        free_hmm(hmm);
        return -1;
    }
    
    // Skip the parameters we already read
    int dummy;
    double dummy_double;
    
    // Skip N, M, T
    fscanf(file, "%d %d %d", &dummy, &dummy, &dummy);
    
    // Skip transition matrix (3x3)
    for (int i = 0; i < 9; i++) {
        fscanf(file, "%lf", &dummy_double);
    }
    
    // Skip emission matrix (3x3)
    for (int i = 0; i < 9; i++) {
        fscanf(file, "%lf", &dummy_double);
    }
    
    // Skip initial probabilities (3)
    for (int i = 0; i < 3; i++) {
        fscanf(file, "%lf", &dummy_double);
    }
    
    // Read observation sequence
    int* observations = (int*)malloc(hmm->sequence_length * sizeof(int));
    if (observations == NULL) {
        printf("Error: Failed to allocate memory for observations\n");
        free_hmm(hmm);
        fclose(file);
        return -1;
    }
    
    for (int t = 0; t < hmm->sequence_length; t++) {
        if (fscanf(file, "%d", &observations[t]) != 1) {
            printf("Error: Failed to read observation %d\n", t);
            free(observations);
            free_hmm(hmm);
            fclose(file);
            return -1;
        }
    }
    
    fclose(file);
    
    // Execute Viterbi algorithm
    ViterbiResult* result = viterbi_algorithm(hmm, observations);
    if (result == NULL) {
        printf("Error: Viterbi algorithm failed\n");
        free(observations);
        free_hmm(hmm);
        return -1;
    }
    
    // Print results
    print_step_by_step(result, hmm, observations, verbose);
    print_final_results_enhanced(result, hmm->sequence_length);
    
    // Clean up memory
    free_viterbi_result_enhanced(result, hmm->sequence_length);
    free(observations);
    free_hmm(hmm);
    
    return 0;
}
