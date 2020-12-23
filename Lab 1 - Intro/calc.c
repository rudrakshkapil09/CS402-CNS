// basic calculations using gmp library
// Rudraksh Kapil - 177154
#include <gmp.h>
#include <stdio.h>

int main()
{
    gmp_printf("\n\n-------------------------------------------------------\n");
    gmp_printf("Program performs basic calculations using gmp library\n");
    gmp_printf("-------------------------------------------------------\n");

    mpz_t a,b,c;
    mpz_inits(a,b,c,NULL);
    gmp_printf("\nEnter value of a: ");
    gmp_scanf("%Zd", a);
    gmp_printf("\nEnter value of b: ");
    gmp_scanf("%Zd", b);

    printf("\n\n-----Results:-----\n");
    mpz_add(c,a,b);
    gmp_printf("a + b = %Zd\n", c);
    mpz_sub(c,a,b);
    gmp_printf("a - b = %Zd\n", c);
    mpz_mul(c,a,b);
    gmp_printf("a * b = %Zd\n", c);
    mpz_fdiv_q(c,a,b);
    gmp_printf("a / b = %Zd\n", c);
    mpz_mod(c,a,b);
    gmp_printf("a mod b = %Zd\n", c);
}
