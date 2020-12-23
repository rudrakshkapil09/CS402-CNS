// Sender side of Elgamal - using message queue
// All Message Queue functions are defined in MSGQ_header.hpp
// Rudraksh Kapil - 177154
#include <gmp.h>
#include <iostream>
#include "MSGQ_header.hpp"
using namespace std;

int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "              Sender side of ElGamal\n";
    cout << "---------------------------------------------------\n";

    // set up message queue
    int msq_id = setup_msq(); 

    // initialization of variables and random seed
    mpz_t gamma,pubA,q,nx,y,a,b,A,B,gen,m,k,t,c1,c2,temp,prk,pubk,delta,n;
    int i, bt;
    unsigned long seed;
    mpz_inits(gamma,pubA,q,nx,y,a,b,A,B,gen,m,k,t,c1,c2,temp,prk,pubk,delta,n,NULL);

    mpz_set_ui(b,1000);				// Random number generation
    gmp_randstate_t s;
    gmp_randinit_mt(s);

    seed = 0;
    gmp_randseed_ui(s,seed);

    // getting input
    printf("Enter the prime P: ");
    scanf("%d", &bt);
  
    // repeat till n is definitely a prime  
    do {
		mpz_rrandomb(q,s,bt);    // random integer with long strings of zeros and ones in the binary representation using bt
        mpz_nextprime(q,q);      // set q to the next greater prime than itself
        mpz_mul_ui(temp,q,2);    // temp = q * 2
        mpz_add_ui(temp,temp,1); // temp = temp + 1
        mpz_set(n,temp);         // n = temp = (q * 2) + 1
    } while(mpz_probab_prime_p(n,10)!=2);

    // find a co prime for generator
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
    mpz_urandomm(a,s,temp); // a = uniform random num between 0 to temp-1 
    mpz_powm(pubA,gen,a,n); // pubA = gen^a % n
    gmp_printf("\nPublic Key => %Zd\nPrivate Key => %Zd \n",pubA,a);

    // send a and receive b
    // store info in data object
    mpz_inits(to_send.msg, NULL);
    mpz_set(to_send.msg, pubA);  

    // send pubA to receiver
    m_ds.msg_type = 1;
    m_ds.msg = to_send;
    assert(msgsnd(msq_id, &m_ds, sizeof(m_ds), 0) == 0);
    cout << "[pubA sent]\n";  
    
    mpz_urandomm(m,s,n);      // m = uniform random num between 0 to n-1
    mpz_urandomm(k,s,temp);   // k = uniform random num between 0 to temp-1 
    mpz_powm(gamma,gen,k,n);  // gamma = gen  ^k % n
    mpz_powm(delta,pubA,k,n); // delta = pubA ^k % n
    mpz_mul(delta,delta,m);   // delta = delta * m
    mpz_mod(delta,delta,n);   // delta = delta * n
    gmp_printf("\nm => %Zd\nk => %Zd\nCiphertext (C1,C2) => (%Zd,%Zd)\n",m,k,gamma,delta);
    
    mpz_sub(temp,n,a);        // temp = n - a
    mpz_sub_ui(temp,temp,1);  // temp = temp - 1
    mpz_powm(A,gamma,temp,n); // A = gamma ^ temp % n
    mpz_mul(A,A,delta);       // A = A * delta
    mpz_mod(A,A,n);           // A = A * n
    gmp_printf("\nPlaintext => %Zd\n",A);
}
