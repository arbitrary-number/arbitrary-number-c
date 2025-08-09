#include "arbitrary_number.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 3  // QAP size (keep small for exact brute force)

typedef int permutation_t[N];

// Compare two arbitrary numbers: returns -1 if a < b, 0 if equal, 1 if a > b
int arbitrary_compare(const ArbitraryNumber* a, const ArbitraryNumber* b) {
    // We'll convert to double for comparison here (for simplicity)
    // In production, implement exact rational comparison by cross-multiplcation:
    // sum_a = sum of terms (c*a/b), same for b, compare sums exactly.
    double da = 0.0, db = 0.0;
    for (size_t i = 0; i < a->length; i++)
        da += a->terms[i].c * (double)a->terms[i].a / a->terms[i].b;
    for (size_t i = 0; i < b->length; i++)
        db += b->terms[i].c * (double)b->terms[i].a / b->terms[i].b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

// Generate all permutations of size n recursively
void generate_permutations(int n, int* perm, bool* used, int depth, void (*callback)(int*, int, void*), void* user_data) {
    if (depth == n) {
        callback(perm, n, user_data);
        return;
    }
    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = true;
            perm[depth] = i;
            generate_permutations(n, perm, used, depth + 1, callback, user_data);
            used[i] = false;
        }
    }
}

typedef struct {
    ArbitraryNumber* A[N][N];
    ArbitraryNumber* B[N][N];
    ArbitraryNumber* best_cost;
    permutation_t best_perm;
    bool found;
} QAPState;

// Calculate cost for a given permutation pi
ArbitraryNumber* qap_calculate_cost(QAPState* state, int* pi) {
    ArbitraryNumber* total = arbitrary_create();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // cost term = A[i][j] * B[pi[i]][pi[j]]
            ArbitraryNumber* term = arbitrary_multiply(state->A[i][j], state->B[pi[i]][pi[j]]);
            ArbitraryNumber* old_total = total;
            total = arbitrary_add(total, term);
            arbitrary_free(old_total);
            arbitrary_free(term);
        }
    }
    return total;
}

void qap_permutation_callback(int* perm, int n, void* user_data) {
    QAPState* state = (QAPState*)user_data;
    ArbitraryNumber* cost = qap_calculate_cost(state, perm);

    if (!state->found || arbitrary_compare(cost, state->best_cost) < 0) {
        if (state->found) arbitrary_free(state->best_cost);
        state->best_cost = cost;
        for (int i = 0; i < n; i++) {
            state->best_perm[i] = perm[i];
        }
        state->found = true;
    } else {
        arbitrary_free(cost);
    }
}

int main() {
    // Initialize matrices A and B with symbolic fractional costs
    QAPState state = {0};

    // Example A matrix (flow): fractions with intermediate complexity
    // A = [[1/2, 1/3, 1/4],
    //      [1/3, 1/5, 1/6],
    //      [1/4, 1/6, 1/7]]
    int A_num[N][N] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    int A_den[N][N] = {
        {2, 3, 4},
        {3, 5, 6},
        {4, 6, 7}
    };

    // Example B matrix (distance)
    // B = [[2/3, 1/4, 1/5],
    //      [1/4, 1/2, 1/3],
    //      [1/5, 1/3, 1/7]]
    int B_num[N][N] = {
        {2, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    int B_den[N][N] = {
        {3, 4, 5},
        {4, 2, 3},
        {5, 3, 7}
    };

    // Initialize arbitrary numbers for matrices A and B
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            state.A[i][j] = arbitrary_create();
            arbitrary_add_term(state.A[i][j], 1, A_num[i][j], A_den[i][j]);
            state.B[i][j] = arbitrary_create();
            arbitrary_add_term(state.B[i][j], 1, B_num[i][j], B_den[i][j]);
        }
    }

    // Initialize best cost as empty
    state.best_cost = NULL;
    state.found = false;

    int perm[N];
    bool used[N] = {false};

    printf("Starting exact QAP solver with arbitrary numbers...\n");
    generate_permutations(N, perm, used, 0, qap_permutation_callback, &state);

    if (state.found) {
        printf("Best permutation found with cost: ");
        arbitrary_print(state.best_cost);
        printf("\nPermutation: [ ");
        for (int i = 0; i < N; i++) {
            printf("%d ", state.best_perm[i]);
        }
        printf("]\n");
        arbitrary_free(state.best_cost);
    } else {
        printf("No solution found.\n");
    }

    // Free matrices
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arbitrary_free(state.A[i][j]);
            arbitrary_free(state.B[i][j]);
        }
    }

    return 0;
}
