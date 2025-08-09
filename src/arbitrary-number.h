#ifndef ARBITRARY_NUMBER_H
#define ARBITRARY_NUMBER_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    int64_t c;  // Coefficient (can be negative)
    int64_t a;  // Numerator
    int64_t b;  // Denominator (non-zero)
} ArbitraryTerm;

typedef struct {
    ArbitraryTerm* terms;
    size_t length;
    size_t capacity;
} ArbitraryNumber;

// === Core API ===

ArbitraryNumber* arbitrary_create();
void arbitrary_free(ArbitraryNumber* num);

void arbitrary_add_term(ArbitraryNumber* num, int64_t c, int64_t a, int64_t b);
void arbitrary_print(const ArbitraryNumber* num);
ArbitraryNumber* arbitrary_add(const ArbitraryNumber* a, const ArbitraryNumber* b);
ArbitraryNumber* arbitrary_multiply(const ArbitraryNumber* a, const ArbitraryNumber* b);

#endif
