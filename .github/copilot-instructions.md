# GitHub Copilot Instructions for Viterbi Algorithm Implementation

You are an expert C programmer and algorithms specialist with deep knowledge of Hidden Markov Models (HMM) and the Viterbi algorithm. I need you to create separate hmm.h and hmm.c files to extend my existing C project with a complete Viterbi algorithm implementation for weather prediction.

## Project Structure Required

- Create `hmm.h` with all HMM structure definitions and function declarations
- Create `hmm.c` with complete Viterbi algorithm implementation  
- Modify existing `main.c` to include and execute the weather prediction example
- Keep existing Bayesian network code intact - this is an addition, not a replacement

## Technical Requirements

### Data Structures
Create new HMM-specific structures separate from existing code:

```c
typedef struct {
    int num_states;       // N = 3 (SUNNY=0, CLOUDY=1, RAINY=2)
    int num_observations; // M = 3 (UMBRELLA=0, SUNGLASSES=1, STAY_HOME=2)  
    int sequence_length;  // T = 7
    double **transition;  // Matrix A (NxN) - transition probabilities
    double **emission;    // Matrix B (NxM) - emission probabilities
    double *initial;      // Vector π (Nx1) - initial state probabilities
} HMM;

typedef struct {
    double **delta;       // Matrix δ (TxN) - max probabilities
    int **psi;           // Matrix ψ (TxN) - backpointer matrix
    int *path;           // Optimal state sequence (Tx1)
    double probability;  // Final probability of optimal path
} ViterbiResult;
```

### Required Functions
Implement these core functions with exact signatures:

```c
// Memory management
HMM* allocate_hmm(int N, int M, int T);
void free_hmm(HMM* hmm);
ViterbiResult* allocate_viterbi_result(int T, int N);
void free_viterbi_result(ViterbiResult* result);

// File I/O  
HMM* load_hmm(char* filename);  // Read from hardcoded "clima_ejemplo.txt"

// Core algorithm
ViterbiResult* viterbi_algorithm(HMM* hmm, int* observations);

// Output functions
void print_step_by_step(ViterbiResult* result, HMM* hmm, int* observations, int verbose);
void print_matrices_formatted(double** matrix, int rows, int cols, char* title);
void print_final_results(ViterbiResult* result);
```

## Input File Format

The program should read from hardcoded file "clima_ejemplo.txt":
```
3                    // N: Number of states
3                    // M: Number of observations
7                    // T: Sequence length
0.7 0.2 0.1         // Transition matrix A row 1 (SUNNY->)
0.3 0.4 0.3         // Transition matrix A row 2 (CLOUDY->)
0.2 0.3 0.5         // Transition matrix A row 3 (RAINY->)
0.1 0.8 0.1         // Emission matrix B row 1 (SUNNY emits)
0.3 0.4 0.3         // Emission matrix B row 2 (CLOUDY emits)
0.8 0.1 0.1         // Emission matrix B row 3 (RAINY emits)
0.6 0.3 0.1         // Initial probabilities π
1 1 0 2 0 0 1       // Observation sequence
```

## Required Verbose Output Format

When verbose flag is enabled, show detailed step-by-step calculations:

```
VITERBI ALGORITHM - WEATHER PREDICTION
=====================================

INPUT PARAMETERS:
States: SUNNY(0), CLOUDY(1), RAINY(2)
Observations: UMBRELLA(0), SUNGLASSES(1), STAY_HOME(2)
Sequence: [SUNGLASSES, SUNGLASSES, UMBRELLA, STAY_HOME, UMBRELLA, UMBRELLA, SUNGLASSES]

TRANSITION MATRIX A:
        SUNNY   CLOUDY  RAINY
SUNNY   0.700   0.200   0.100
CLOUDY  0.300   0.400   0.300  
RAINY   0.200   0.300   0.500

EMISSION MATRIX B:
        UMBRELLA SUNGLASSES STAY_HOME
SUNNY   0.100    0.800      0.100
CLOUDY  0.300    0.400      0.300
RAINY   0.800    0.100      0.100

INITIALIZATION (t=1, obs=SUNGLASSES):
δ₁(SUNNY)  = π(SUNNY)  × B(SUNNY,SUNGLASSES)  = 0.600 × 0.800 = 0.480
δ₁(CLOUDY) = π(CLOUDY) × B(CLOUDY,SUNGLASSES) = 0.300 × 0.400 = 0.120
δ₁(RAINY)  = π(RAINY)  × B(RAINY,SUNGLASSES)  = 0.100 × 0.100 = 0.010

DELTA MATRIX (t=1):
        SUNNY   CLOUDY  RAINY
t=1     0.480   0.120   0.010

RECURSION:
t=2, observation=SUNGLASSES(1):
For SUNNY:  max{0.480×0.7, 0.120×0.3, 0.010×0.2} × 0.8 = max{0.336, 0.036, 0.002} × 0.8 = 0.269
For CLOUDY: max{0.480×0.2, 0.120×0.4, 0.010×0.3} × 0.4 = max{0.096, 0.048, 0.003} × 0.4 = 0.038
For RAINY:  max{0.480×0.1, 0.120×0.3, 0.010×0.5} × 0.1 = max{0.048, 0.036, 0.005} × 0.1 = 0.005

[Continue for all time steps with formatted matrices...]

TERMINATION:
δ₇(SUNNY)  = [final value]
δ₇(CLOUDY) = [final value]  
δ₇(RAINY)  = [final value]
Maximum probability: [max value]
Optimal final state: [state name]

BACKTRACKING:
t=7: state = [state]
t=6: state = [state]
[... continue backwards...]
t=1: state = [state]

FINAL RESULTS:
Optimal state sequence: [0, 0, 2, 1, 2, 2, 0]
Translation: [SUNNY, SUNNY, RAINY, CLOUDY, RAINY, RAINY, SUNNY]
Maximum probability: [calculated value]
```

## Algorithm Implementation Details

### Phase 1: Initialization
```c
// For t=1, i=0 to N-1:
// δ₁(i) = π(i) × B(i, o₁)
// ψ₁(i) = 0
```

### Phase 2: Recursion  
```c
// For t=2 to T:
//   For i=0 to N-1:
//     δₜ(i) = max[δₜ₋₁(j) × A(j,i)] × B(i,oₜ)
//     ψₜ(i) = argmax[δₜ₋₁(j) × A(j,i)]
```

### Phase 3: Termination
```c
// P* = max[δₜ(i)]
// q*ₜ = argmax[δₜ(i)]
```

### Phase 4: Backtracking
```c
// For t=T-1 down to 1:
//   q*ₜ = ψₜ₊₁(q*ₜ₊₁)
```

## Communication Style Preferences

- **Comprehensive commenting**: Explain each mathematical step with formula references
- **Modular design**: Separate functions for each algorithm phase with clear responsibilities
- **Mathematical notation**: Use variable names matching research literature (delta, psi, transition, emission)
- **Robust error handling**: Check malloc returns, file operations, and validate input data
- **Professional standards**: Consistent indentation, meaningful variable names, proper memory cleanup
- **Technical precision**: Comments should explain the mathematical reasoning behind each calculation
- **Debugging support**: Include formatted matrix printing and intermediate value verification
- **Iterative verification**: Build with incremental testing capability for each phase

## Memory Management Requirements

- Use malloc/free for all dynamic allocations
- Check all malloc return values for NULL
- Free all allocated memory in reverse order of allocation
- Implement proper cleanup functions for both HMM and ViterbiResult structures
- Handle memory allocation failures gracefully with appropriate error messages

## Error Handling Requirements

- Validate file exists and is readable
- Check for malformed input data (wrong dimensions, invalid probabilities)
- Verify probability constraints (rows sum to 1.0, values between 0-1)
- Handle memory allocation failures
- Validate observation sequence contains only valid symbols (0, 1, 2)

This implementation should integrate seamlessly with my existing Bayesian network code while providing a complete, production-ready Viterbi algorithm for Hidden Markov Model weather prediction.