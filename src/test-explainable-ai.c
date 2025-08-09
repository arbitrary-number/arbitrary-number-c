#include "arbitrary_number.h"
#include <stdio.h>

// Inputs and symbolic weights for a 2-input node with bias
int main() {
    // === Inputs ===
    int64_t x1 = 1;
    int64_t x2 = 1;

    // === Weights ===
    ArbitraryNumber* w1 = arbitrary_create();  // 2/5
    arbitrary_add_term(w1, 1, 2, 5);

    ArbitraryNumber* w2 = arbitrary_create();  // 1/2
    arbitrary_add_term(w2, 1, 1, 2);

    // === Bias ===
    ArbitraryNumber* bias = arbitrary_create();  // 1/10
    arbitrary_add_term(bias, 1, 1, 10);

    // === Trace output expression ===
    printf("Explainable Inference Trace:\n");

    printf("Input x1 = %lld, Weight w1 = ", x1);
    arbitrary_print(w1);

    printf("Input x2 = %lld, Weight w2 = ", x2);
    arbitrary_print(w2);

    printf("Bias term = ");
    arbitrary_print(bias);

    // === Weighted contributions ===
    ArbitraryNumber* term1 = arbitrary_create();
    ArbitraryNumber* term2 = arbitrary_create();

    arbitrary_add_term(term1, w1->terms[0].c * x1, w1->terms[0].a, w1->terms[0].b);
    arbitrary_add_term(term2, w2->terms[0].c * x2, w2->terms[0].a, w2->terms[0].b);

    // === Output = term1 + term2 + bias ===
    ArbitraryNumber* sum = arbitrary_add(term1, term2);
    ArbitraryNumber* output = arbitrary_add(sum, bias);

    printf("\nSymbolic Output Expression:\n");
    arbitrary_print(output);

    printf("\nBreakdown:\n");
    printf("=> x1 * w1 = "); arbitrary_print(term1);
    printf("=> x2 * w2 = "); arbitrary_print(term2);
    printf("=> bias    = "); arbitrary_print(bias);
    printf("=> Output  = "); arbitrary_print(output);

    // Cleanup
    arbitrary_free(w1);
    arbitrary_free(w2);
    arbitrary_free(bias);
    arbitrary_free(term1);
    arbitrary_free(term2);
    arbitrary_free(sum);
    arbitrary_free(output);

    return 0;
}
