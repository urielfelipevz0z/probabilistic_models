#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"

int main() {
    printf("=== TESTING HMM BASIC MODE ===\n");
    
    int result = run_weather_prediction_example(0);
    if (result == 0) {
        printf("\n=== HMM Basic Mode - SUCCESS ===\n");
    } else {
        printf("\n=== HMM Basic Mode - FAILED ===\n");
    }
    
    return result;
}
