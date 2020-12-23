// outputs gcd of two numbers
// Rudraksh Kapil - 177154
#include <gmp.h>
#include <stdio.h>

int main()
{
    gmp_printf("\n\n---------------------------------------------------\n");
    gmp_printf("Program outputs gcd of two numbers using gmp\n");
    gmp_printf("---------------------------------------------------\n");

    mpz_t a,b,c;
    mpz_inits(a,b,c,NULL);
    gmp_printf("\nEnter value of a: ");
    gmp_scanf("%Zd", a);
    gmp_printf("\nEnter value of b: ");
    gmp_scanf("%Zd", b);

    printf("\n\n-----Results:-----\n");
    gmp_printf("HCF of %Zd and %Zd => ", a, b);
    while (mpz_cmp_ui(b,0) != 0) {
        mpz_set(c,b);
        mpz_mod(b,a,b);
        mpz_set(a,c);
    }
    gmp_printf("%Zd\n", a);

}
