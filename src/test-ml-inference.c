#include "arbitrary_number.h"
#include <stdio.h>

int main() {
    // === Step 1: Inputs (fixed real values) ===
    int64_t input_values[3] = {1, 2, 3}; // x1=1, x2=2, x3=3

    // === Step 2: Weights (symbolic) ===
    ArbitraryNumber* weights[3];

    // w1 = 1*(1/3)
    weights[0] = arbitrary_create();
    arbitrary_add_term(weights[0], 1, 1, 3);

    // w2 = -1*(2/5)
    weights[1] = arbitrary_create();
    arbitrary_add_term(weights[1], -1, 2, 5);

    // w3 = 1*(7/8)
    weights[2] = arbitrary_create();
    arbitrary_add_term(weights[2], 1, 7, 8);

    // === Step 3: Bias (symbolic) ===
    ArbitraryNumber* bias = arbitrary_create();
    arbitrary_add_term(bias, 1, 1, 6); // bias = 1*(1/6)

    // === Step 4: Compute weighted sum ===
    ArbitraryNumber* sum = arbitrary_create();

    for (int i = 0; i < 3; ++i) {
        ArbitraryNumber* term_result = arbitrary_create();

        for (size_t j = 0; j < weights[i]->length; ++j) {
            ArbitraryTerm t = weights[i]->terms[j];
            arbitrary_add_term(term_result, t.c * input_values[i], t.a, t.b);
        }

        ArbitraryNumber* temp = sum;
        sum = arbitrary_add(sum, term_result);
        arbitrary_free(temp);
        arbitrary_free(term_result);
    }

    // === Step 5: Add bias ===
    ArbitraryNumber* result = arbitrary_add(sum, bias);

    // === Step 6: Output final result ===
    printf("ML Inference Node Output (symbolic):\n");
    arbitrary_print(result);

    // Cleanup
    for (int i = 0; i < 3; ++i) arbitrary_free(weights[i]);
    arbitrary_free(bias);
    arbitrary_free(sum);
    arbitrary_free(result);

    return 0;
}
