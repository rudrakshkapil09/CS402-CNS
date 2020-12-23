// basic modular arithmetic operations using gmp library
// Rudraksh Kapil - 177154
#include <gmp.h>
#include <stdio.h>

int main()
{
    gmp_printf("\n\n-------------------------------------------------------------\n");
    gmp_printf("Program performs modular arithmetic on two numbers using gmp\n");
    gmp_printf("-------------------------------------------------------------\n");

    //get input
    mpz_t a,b,n,temp1,temp2,temp3;
    mpz_inits(a,b,n,temp1,temp2,temp3,NULL);
    gmp_printf("Enter value of a: ");
    gmp_scanf("%Zd", a);
    gmp_printf("Enter value of b: ");
    gmp_scanf("%Zd", b);
    gmp_printf("Enter value of n: ");
    gmp_scanf("%Zd", n);
    
    //print outputs
    printf("\n\n-----Results:-----\n");
    mpz_add(temp1,a,b);
    mpz_mod(temp2,temp1,n);
    gmp_printf("(a + b) mod n => ");
    gmp_printf("(%Zd + %Zd) mod %Zd => %Zd\n\n",a,b,n,temp2);

    mpz_mod(temp1,a,n);
    gmp_printf("a mod n => ");
    gmp_printf("%Zd mod %Zd => %Zd\n\n",a,n,temp1);

    mpz_mod(temp2,b,n);
    gmp_printf("b mod n => ");
    gmp_printf("%Zd mod %Zd => %Zd\n\n",b,n,temp2);

    mpz_add(temp3,temp1,temp2);
    mpz_mod(temp1,temp3,n);
    gmp_printf("(a mod n + b mod n) mod n => ");
    gmp_printf("(%Zd mod %Zd + %Zd mod %Zd) mod %Zd => %Zd\n\n\n", a,n,b,n,n,temp1);


    mpz_sub(temp1,a,b);
    mpz_mod(temp2,temp1,n);
    gmp_printf("(a - b) mod n => ");
    gmp_printf("(%Zd - %Zd) mod %Zd => %Zd\n\n", a,b,n,temp2);

    mpz_mul(temp1,a,b);
    mpz_mod(temp2,temp1,n);
    gmp_printf("(a * b) mod n => ");
    gmp_printf("(%Zd * %Zd) mod %Zd => %Zd\n\n", a,b,n,temp2);
}
