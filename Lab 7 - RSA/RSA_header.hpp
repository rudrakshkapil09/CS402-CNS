// INCLUDE STATEMENTS
#include <iostream>
#include <gmp.h>
using namespace std;


// Inverse modulo using gmp library
void InverseModulo(mpz_t res, mpz_t a, mpz_t b)
{
    //create gmp variables
    mpz_t A, B, tp, tpp, q, r, t, m, x, sp, spp, s;
	mpz_inits(A,B,q,r,t,tp,tpp,m,x,s,sp,spp,NULL);

    // initialise i (iteration counter), s0, s1, t0, and t1
	int i=2;
	mpz_set_ui(tp,1);
	mpz_set_ui(tpp,0);
	mpz_set_ui(sp,0);
	mpz_set_ui(spp,1);

    // sed b as the mod, and put a in A and b in B for safe keeping till the end
	mpz_set(m,b);
    mpz_set(A,a);
    mpz_set(B,b);
	mpz_gcd(x,a,b); // get x as the gcd of a and b

    // terminatiomn condition inside
	while(1) {
        // get quotient and remainder
		mpz_fdiv_q(q,b,a);
		mpz_fdiv_r(r,b,a);

        // termination condition - remainder is 0
		if (mpz_cmp_ui(r,0) == 0) 
		    break;

        // update t[i] <- t[i-2] - q*t[i-1], similarly for s
		mpz_mul(t,q,tp);
		mpz_sub(t,tpp,t);
		mpz_mul(s,q,sp);
		mpz_sub(s,spp,s);

        // put a into b, r into a and update t[i-2] and t[i-1], similarly for s
		mpz_set(b,a);
		mpz_set(a,r);
		mpz_set(tpp,tp);
		mpz_set(tp,t);
		mpz_set(spp,sp);
		mpz_set(sp,s);

        // update counter
		i++;
	}

    // check if we get t value -> if yes then cycle back by adding m
	if (mpz_cmp_ui(t,0) < 0)    
        mpz_add(t,m,t);

    // retore original A and B values, and put t (inverse) into result
    mpz_set(a,A);
    mpz_set(b,B);
    mpz_set(res,t);
}

// Miller-Rabin Primality Test function on n - using gmp library
bool MillerRabin(mpz_t n)
{
    // gmp variables
    gmp_randstate_t st;
    gmp_randinit_default(st);
    mpz_t r,q,a,b,nm , n_3;
    mpz_inits(r,q,a,b,nm,n_3,NULL);

    // put n into q and nm
    mpz_set(q,n);
    mpz_set(nm,n);
    mpz_sub_ui(nm, nm, 1);     // nm = nm-1 = n-1
    mpz_sub_ui(q, q, 1);       // q = q-1 = n-1
    mpz_sub_ui(n_3, n, 3);     // n_3 = n-3

    // counter 
    int k = 0;
    while (1) {
        // r = q % 2
        mpz_mod_ui(r,q,2); 

        // if remainder is 0
        if (mpz_cmp_ui(r,0) == 0) {
            mpz_fdiv_q_ui(q,q,2);   // q = q - floor(q/2) * 2 
            k++;
        }
        else break;
    }

    mpz_urandomm(a,st,n_3);    // a is a uniform random integer between 0 to n_3 - 1 
    mpz_powm(b,a,q,n);         // b = a^q % n
    if (mpz_cmp_ui(b,1) == 0)  // if b is 1, return prime
        return true;

    // try k times
    for (int j=0;j<=k-1;j++) {
        // temp variables
        mpz_t temp1,temp2;
        mpz_inits(temp1,temp2,NULL);

        mpz_mul_ui(temp1,q,pow(2,j));  // temp1 = q * 2^j
        mpz_powm(temp1,a,temp2,n);     // temp1 = a^temp2 % n

        // if temp2 == n-1 return prime
        if (mpz_cmp(temp2,nm)==0)
            return true;
    }

    //return composite
    return false;

}

