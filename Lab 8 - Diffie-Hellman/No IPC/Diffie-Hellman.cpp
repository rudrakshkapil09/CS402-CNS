// include statements
#include <gmp.h>
#include <iostream>
#include <math.h>
using namespace std;

// MAIN
int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "            Diffie Hellman Key Exchange\n";
    cout << "---------------------------------------------------\n";
    
    // initialization of variables and random seed
    mpz_t c2,temp,x,y,a,b,A,B,gen,q,prk,pubk,n,t,c1;
    int i, bt;
    unsigned long seed;
    mpz_inits(c2,temp,x,y,a,b,A,B,gen,q,prk,pubk,n,t,c1,NULL);
  
    mpz_set_ui(b, 1000);  			// Random number generation
    gmp_randstate_t s;
    gmp_randinit_mt(s);

    seed = time(NULL);
    gmp_randseed_ui(s,seed);
  
    // getting input
    printf("Enter the prime P: ");
    scanf("%d", &bt);
    
    // loop until n is likely prime
    do {
        mpz_rrandomb(q,s,bt);    // random integer with long strings of zeros and ones in the binary representation using bt
        mpz_nextprime(q,q);      // set q to the next greater prime than itself
        mpz_mul_ui(temp,q,2);    // temp = q * 2
        mpz_add_ui(temp,temp,1); // temp = temp + 1
        mpz_set(n,temp);         // n = temp = (q * 2) + 1
    } while (mpz_probab_prime_p(n,100)==1 || mpz_probab_prime_p(n,10) == 2);

    // try from 1-20 to find a co prime smaller than n
    for (i = 1; i < 20; i++) {
        mpz_set_ui(t,i);         // t = i
        mpz_powm_ui(c1,t,2,n);   // c1 = (t ^ 2) % n
        mpz_powm(c2,t,q,n);      // c2 = (t ^ q) % n
    
        // if c1 is not 1 AND c2 is not 1 then we can break the loop
        if ((mpz_cmp_ui(c1,1)!=0) && (mpz_cmp_ui(c2,1)!=0))
            break;
    }

    //print outputs
    printf("\n\n-----Results:-----\n");

    // gen = i
    mpz_set_ui(gen,i);
    gmp_printf("\nGenerator => %Zd\n",gen);

    mpz_sub_ui(temp,n,2);   // temp = n - 2 
    mpz_urandomm(x,s,temp); // x = uniform random num between 0 to temp-1 
    mpz_urandomm(y,s,temp); // y = ^^
    mpz_powm(a,gen,x,n);    // a = gen ^ x % n
    mpz_powm(b,gen,y,n);    // b = gen ^ y % n
    gmp_printf("\n[Private Key] A sends => %Zd\n[Private Key] B sends => %Zd\n", a,b);

    mpz_powm(A,b,x,n);      // A = b ^ x % n
    mpz_powm(B,a,y,n);      // B = a ^ y % n
    gmp_printf("\n[Secret Key]  A computes => %Zd\n[Secret Key]  B computes => %Zd\n", A,B);
}
   
