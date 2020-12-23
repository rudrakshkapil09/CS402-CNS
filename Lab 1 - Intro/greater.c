// outputs greater of two numbers
// Rudraksh Kapil - 177154
#include <gmp.h>
#include <stdio.h>

int main()
{
    gmp_printf("\n\n---------------------------------------------------\n");
    gmp_printf("Program outputs greater of two numbers using gmp\n");
    gmp_printf("---------------------------------------------------\n");

    mpz_t a,b,c;
    mpz_inits(a,b,c,NULL);
    gmp_printf("\nEnter value of a: ");
    gmp_scanf("%Zd", a);
    gmp_printf("\nEnter value of b: ");
    gmp_scanf("%Zd", b);

    printf("\n\n-----Results:-----\n");
    if (mpz_cmp(a,b)>0)
        gmp_printf("a (%Zd) is bigger\n", a);
    else
        gmp_printf("b (%Zd) is bigger\n", b);
}
