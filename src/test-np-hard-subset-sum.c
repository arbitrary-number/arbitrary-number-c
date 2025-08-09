#include "arbitrary_number.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Check if two ArbitraryNumbers are exactly equal (all terms equal)
bool arbitrary_equal(ArbitraryNumber* a, ArbitraryNumber* b) {
    if (a->length != b->length) return false;
    for (size_t i = 0; i < a->length; i++) {
        if (a->terms[i].a != b->terms[i].a ||
            a->terms[i].b != b->terms[i].b ||
            a->terms[i].c != b->terms[i].c) {
            return false;
        }
    }
    return true;
}

// Print subset indices
void print_subset(int* subset, int size) {
    printf("{ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", subset[i]);
    }
    printf("}\n");
}

// Main test
int main() {
    // === Define weights (small NP-hard instance) ===
    // Example weights: {1/3, 1/2, 1/6, 1/4}
    int n = 4;
    ArbitraryNumber* weights[4];
    weights[0] = arbitrary_create(); arbitrary_add_term(weights[0], 1, 1, 3);
    weights[1] = arbitrary_create(); arbitrary_add_term(weights[1], 1, 1, 2);
    weights[2] = arbitrary_create(); arbitrary_add_term(weights[2], 1, 1, 6);
    weights[3] = arbitrary_create(); arbitrary_add_term(weights[3], 1, 1, 4);

    // === Define target sum (exact) ===
    ArbitraryNumber* target = arbitrary_create();
    arbitrary_add_term(target, 1, 1, 1); // target = 1 (exact)

    // === Brute force subsets and check sums ===
    int max_subsets = 1 << n;
    int subset[4];
    int subset_size;
    bool found_solution = false;

    for (int mask = 1; mask < max_subsets; mask++) {
        ArbitraryNumber* sum = arbitrary_create();
        subset_size = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subset[subset_size++] = i;
                ArbitraryNumber* old_sum = sum;
                sum = arbitrary_add(sum, weights[i]);
                arbitrary_free(old_sum);
            }
        }

        if (arbitrary_equal(sum, target)) {
            printf("Found exact subset sum solution:\n");
            print_subset(subset, subset_size);
            printf("Sum = ");
            arbitrary_print(sum);
            found_solution = true;
        }

        arbitrary_free(sum);
    }

    if (!found_solution) {
        printf("No exact subset sum solution found.\n");
    }

    // Cleanup
    for (int i = 0; i < n; i++) {
        arbitrary_free(weights[i]);
    }
    arbitrary_free(target);

    return 0;
}
