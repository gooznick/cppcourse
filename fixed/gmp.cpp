#include <gmp.h>
#include <iostream>
int main() {
    mpz_t big;
    mpz_init(big);
    mpz_set_str(big, "123456789123456789", 10);
    mpz_mul_ui(big, big, 2);
    gmp_printf("%Zd\n", big);
    mpz_clear(big);
}