// outputs multiplicative inverse such that a.t + b.s = gcd(a,b)
// Rudraksh Kapil - 177154
#include <gmp.h>
#include <stdio.h>

int main()
{
    gmp_printf("\n\n---------------------------------------------------\n");
    gmp_printf("Program outputs multiplicative inverse of a mod b\n");
    gmp_printf("---------------------------------------------------\n");

    //get input
    mpz_t a,b,r,s,t,old_r,old_s,old_t,q,temp,temp2;
    mpz_inits(a,b,r,s,t,old_r,old_s,old_t,q,temp,temp2,NULL);
    gmp_printf("Enter value of a: ");
    gmp_scanf("%Zd", a);
    gmp_printf("Enter value of b: ");
    gmp_scanf("%Zd", b);

    // b always has to be > a:
    if (mpz_cmp(a,b) > 0) {
        printf("b must be greater than a");
        return 0;
    }

    //initialise
    mpz_set_ui(old_s,1);    //s0
    mpz_set_ui(s,0);        //s1
    mpz_set_ui(old_t,0);    //t0
    mpz_set_ui(t,1);        //t1
    mpz_set(r,a);           //r1 (< r0)     12  -> r
    mpz_set(old_r,b);       //r0            67  -> old_r


    //loop while r != 0
    gmp_printf("\n[Table]:\n   Q    R    S    T\n");
    gmp_printf("--------------------\n");
    while (mpz_cmp_ui(r,0) != 0) {
        //q <- old_r / r
        mpz_fdiv_q(q, old_r, r);

        // (old_r, r) <- (r, old_r mod r)
        mpz_set(temp, r);           // 1
        mpz_mod(r, old_r, r);       // 2
        mpz_set(old_r, temp);       // 1

        // (old_s, s) <- (s, old_s - q * s)
        mpz_set(temp, s);           // 1
        mpz_mul(temp2, q, s);       // 2
        mpz_sub(s, old_s, temp2);   // 2
        mpz_set(old_s, temp);       // 2

        // (old_t, t) <- (t, old_t - q * t)
        mpz_set(temp, t);           // 1
        mpz_mul(temp2, q, t);       // 2
        mpz_sub(t, old_t, temp2);   // 2
        mpz_set(old_t, temp);       // 2

        gmp_printf("%4Zd %4Zd %4Zd %4Zd\n", q, r, s, t);
    }

    //print outputs
    printf("\n\n-----Results:-----\n");

    // check if co-primes:
    gmp_printf("GCD of %Zd and %Zd => %Zd\n\n", a, b, old_r);
    if (mpz_cmp_ui(old_r, 1) != 0) {
        printf("The two numbers are not coprime hence cannot calculate multiplicative inverse!\n\n");
        return 0;
    }

    //print s and t values: (old ones, since we updated s and t once after r == 0)
    gmp_printf("Value of s => %Zd\n", old_s);
    gmp_printf("Value of t => %Zd\n", old_t);
    mpz_mul(temp, old_t, a);
    mpz_mul(temp2, old_s, b);
    mpz_add(temp, temp, temp2);
    gmp_printf("Check => (a * t) + (b * s) -> should be 1\n");
    gmp_printf("      => (%Zd * %Zd) + (%Zd * %Zd) = %Zd\n", a, old_t, b, old_s, temp);
    if (mpz_cmp_ui(temp, 1) == 0)
        printf("Correct implementation!\n\n");
    else printf("Wrong :(\n\n");

    //final output
    gmp_printf("Hence, [%Zd is the inverse of %Zd mod %Zd]\n", old_t, a, b);
    mpz_mul(temp, old_t, a);
    mpz_mod(temp, temp, b);
    gmp_printf("Check => (t * a) mod b -> should be 1\n");
    gmp_printf("      => (%Zd * %Zd) mod %Zd = %Zd\n\n", old_t, a, b, temp);


}
