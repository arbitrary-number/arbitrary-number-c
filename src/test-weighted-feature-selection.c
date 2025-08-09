#include "arbitrary_number.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_FEATURES 16  // Keep small for brute force demo

// Check exact equality of two ArbitraryNumbers (terms must match)
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

// Print indices of selected features
void print_selected_features(int* selected, int count) {
    printf("{ ");
    for (int i = 0; i < count; i++) {
        printf("%d ", selected[i]);
    }
    printf("}\n");
}

int main() {
    // === Example feature weights (symbolic) ===
    // Representing feature importance or contribution to output
    const int n_features = 6;
    ArbitraryNumber* feature_weights[MAX_FEATURES];

    // Initialize some sample fractional weights
    // e.g. Feature 0: 1/5, Feature 1: 2/7, Feature 2: 1/3, ...
    int numerators[] = {1, 2, 1, 3, 5, 7};
    int denominators[] = {5, 7, 3, 10, 20, 14};

    for (int i = 0; i < n_features; i++) {
        feature_weights[i] = arbitrary_create();
        arbitrary_add_term(feature_weights[i], 1, numerators[i], denominators[i]);
    }

    // === Target sum (desired combined feature weight) ===
    ArbitraryNumber* target = arbitrary_create();
    // Let's set target as 1 (exact)
    arbitrary_add_term(target, 1, 1, 1);

    // === Brute force search for subsets whose sum equals target ===
    int max_subsets = 1 << n_features;
    int selected_indices[MAX_FEATURES];
    int selected_count;
    bool found_any = false;

    printf("Weighted Feature Selection - searching subsets that sum to target = ");
    arbitrary_print(target);
    printf("\n");

    for (int subset_mask = 1; subset_mask < max_subsets; subset_mask++) {
        ArbitraryNumber* subset_sum = arbitrary_create();
        selected_count = 0;

        for (int f = 0; f < n_features; f++) {
            if (subset_mask & (1 << f)) {
                selected_indices[selected_count++] = f;
                ArbitraryNumber* old_sum = subset_sum;
                subset_sum = arbitrary_add(subset_sum, feature_weights[f]);
                arbitrary_free(old_sum);
            }
        }

        if (arbitrary_equal(subset_sum, target)) {
            printf("Found exact matching subset: ");
            print_selected_features(selected_indices, selected_count);
            printf("Sum = ");
            arbitrary_print(subset_sum);
            found_any = true;
        }

        arbitrary_free(subset_sum);
    }

    if (!found_any) {
        printf("No exact matching subset found.\n");
    }

    // Cleanup
    for (int i = 0; i < n_features; i++) {
        arbitrary_free(feature_weights[i]);
    }
    arbitrary_free(target);

    return 0;
}
