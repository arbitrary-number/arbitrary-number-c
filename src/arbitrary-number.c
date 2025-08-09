#include "arbitrary_number.h"
#include <stdlib.h>
#include <stdio.h>

ArbitraryNumber* arbitrary_create() {
    ArbitraryNumber* num = malloc(sizeof(ArbitraryNumber));
    num->length = 0;
    num->capacity = 4;
    num->terms = malloc(sizeof(ArbitraryTerm) * num->capacity);
    return num;
}

void arbitrary_free(ArbitraryNumber* num) {
    if (num) {
        free(num->terms);
        free(num);
    }
}

void arbitrary_add_term(ArbitraryNumber* num, int64_t c, int64_t a, int64_t b) {
    if (b == 0) {
        fprintf(stderr, "Error: denominator cannot be zero.\n");
        return;
    }

    if (num->length >= num->capacity) {
        num->capacity *= 2;
        num->terms = realloc(num->terms, sizeof(ArbitraryTerm) * num->capacity);
    }

    num->terms[num->length++] = (ArbitraryTerm){c, a, b};
}

void arbitrary_print(const ArbitraryNumber* num) {
    printf("ArbitraryNumber: ");
    for (size_t i = 0; i < num->length; ++i) {
        ArbitraryTerm t = num->terms[i];
        printf("%s%lld*(%lld/%lld)", (i > 0 ? " + " : ""), t.c, t.a, t.b);
    }
    printf("\n");
}

int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a < 0 ? -a : a;
}

void simplify_term(ArbitraryTerm* t) {
    int64_t common = gcd(t->a, t->b);
    if (common > 1) {
        t->a /= common;
        t->b /= common;
    }
}

ArbitraryNumber* arbitrary_add(const ArbitraryNumber* a, const ArbitraryNumber* b) {
    ArbitraryNumber* result = arbitrary_create();

    for (size_t i = 0; i < a->length; ++i)
        arbitrary_add_term(result, a->terms[i].c, a->terms[i].a, a->terms[i].b);

    for (size_t i = 0; i < b->length; ++i)
        arbitrary_add_term(result, b->terms[i].c, b->terms[i].a, b->terms[i].b);

    return result;
}

ArbitraryNumber* arbitrary_multiply(const ArbitraryNumber* a, const ArbitraryNumber* b) {
    ArbitraryNumber* result = arbitrary_create();

    for (size_t i = 0; i < a->length; ++i) {
        for (size_t j = 0; j < b->length; ++j) {
            ArbitraryTerm t1 = a->terms[i];
            ArbitraryTerm t2 = b->terms[j];

            int64_t c = t1.c * t2.c;
            int64_t num = t1.a * t2.a;
            int64_t denom = t1.b * t2.b;

            arbitrary_add_term(result, c, num, denom);
        }
    }

    return result;
}
