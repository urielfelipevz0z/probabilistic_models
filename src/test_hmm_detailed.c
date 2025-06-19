#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"

int main() {
    printf("=== TESTING HMM DETAILED MODE ===\n");
    
    int result = run_weather_prediction_example(1);
    if (result == 0) {
        printf("\n=== HMM Detailed Mode - SUCCESS ===\n");
    } else {
        printf("\n=== HMM Detailed Mode - FAILED ===\n");
    }
    
    return result;
}
