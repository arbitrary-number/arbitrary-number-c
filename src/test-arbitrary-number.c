#include "arbitrary_number.h"
#include <stdio.h>

int main() {
    ArbitraryNumber* x = arbitrary_create();
    arbitrary_add_term(x, 1, 1, 3);  // 1*(1/3)
    arbitrary_add_term(x, 1, 1, 2);  // +1*(1/2)

    ArbitraryNumber* y = arbitrary_create();
    arbitrary_add_term(y, 1, 5, 6);  // 1*(5/6)

    ArbitraryNumber* sum = arbitrary_add(x, y);
    ArbitraryNumber* prod = arbitrary_multiply(x, y);

    printf("x: "); arbitrary_print(x);
    printf("y: "); arbitrary_print(y);
    printf("x + y: "); arbitrary_print(sum);
    printf("x * y: "); arbitrary_print(prod);

    arbitrary_free(x);
    arbitrary_free(y);
    arbitrary_free(sum);
    arbitrary_free(prod);

    return 0;
}
