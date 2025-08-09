#include "arbitrary_number.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N 3 // QAP dimension

// === Helper: Compare two arbitrary numbers approximately using doubles ===
int arbitrary_compare(const ArbitraryNumber* a, const ArbitraryNumber* b) {
    double da = 0.0, db = 0.0;
    for (size_t i = 0; i < a->length; i++)
        da += a->terms[i].c * (double)a->terms[i].a / a->terms[i].b;
    for (size_t i = 0; i < b->length; i++)
        db += b->terms[i].c * (double)b->terms[i].a / b->terms[i].b;

    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

// === Generate all permutations and apply callback ===
void generate_permutations(int* perm, bool* used, int depth,
                           void (*callback)(int*, void*), void* user_data) {
    if (depth == N) {
        callback(perm, user_data);
        return;
    }
    for (int i = 0; i < N; i++) {
        if (!used[i]) {
            used[i] = true;
            perm[depth] = i;
            generate_permutations(perm, used, depth + 1, callback, user_data);
            used[i] = false;
        }
    }
}

// === Structures ===
typedef struct {
    ArbitraryNumber* A[N][N];
    ArbitraryNumber* B[N][N];
    ArbitraryNumber* best_cost;
    int best_perm[N];
    bool found;
} QAPSolver;

// === Cost computation for a given permutation ===
ArbitraryNumber* compute_cost(QAPSolver* solver, int* perm) {
    ArbitraryNumber* total = arbitrary_create();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ArbitraryNumber* prod = arbitrary_multiply(solver->A[i][j], solver->B[perm[i]][perm[j]]);
            ArbitraryNumber* sum = arbitrary_add(total, prod);
            arbitrary_free(prod);
            arbitrary_free(total);
            total = sum;
        }
    }
    return total;
}

// === Callback for each permutation ===
void evaluate_permutation(int* perm, void* user_data) {
    QAPSolver* solver = (QAPSolver*)user_data;
    ArbitraryNumber* cost = compute_cost(solver, perm);

    if (!solver->found || arbitrary_compare(cost, solver->best_cost) < 0) {
        if (solver->found) arbitrary_free(solver->best_cost);
        solver->best_cost = cost;
        memcpy(solver->best_perm, perm, sizeof(int) * N);
        solver->found = true;
    } else {
        arbitrary_free(cost);
    }
}

// === Main program ===
int main() {
    QAPSolver solver = {0};

    // === Define Matrix A (flow) with symbolic entries ===
    int A_num[N][N] = {
        {17, 5, 3},
        {5, 0, 11},
        {3, 11, 0}
    };
    int A_den[N][N] = {
        {23, 13, 19},
        {13, 1, 29},
        {19, 29, 1}
    };

    // === Define Matrix B (distance) with symbolic entries ===
    int B_num[N][N] = {
        {2, 7, 13},
        {7, 0, 17},
        {13, 17, 0}
    };
    int B_den[N][N] = {
        {5, 31, 41},
        {31, 1, 61},
        {41, 61, 1}
    };

    // === Initialize matrices ===
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            solver.A[i][j] = arbitrary_create();
            arbitrary_add_term(solver.A[i][j], 1, A_num[i][j], A_den[i][j]);

            solver.B[i][j] = arbitrary_create();
            arbitrary_add_term(solver.B[i][j], 1, B_num[i][j], B_den[i][j]);
        }
    }

    // === Run permutation search ===
    int perm[N] = {0};
    bool used[N] = {false};

    printf("🔢 Solving 3x3 symbolic QAP with arbitrary precision:\n");
    generate_permutations(perm, used, 0, evaluate_permutation, &solver);

    // === Output best result ===
    if (solver.found) {
        printf("\n✅ Best permutation: [ ");
        for (int i = 0; i < N; i++) {
            printf("%d ", solver.best_perm[i]);
        }
        printf("]\n");

        printf("🎯 Exact symbolic cost: ");
        arbitrary_print(solver.best_cost);
        printf("\n");
    } else {
        printf("No solution found.\n");
    }

    // === Cleanup ===
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arbitrary_free(solver.A[i][j]);
            arbitrary_free(solver.B[i][j]);
        }
    }
    if (solver.best_cost) arbitrary_free(solver.best_cost);

    return 0;
}
