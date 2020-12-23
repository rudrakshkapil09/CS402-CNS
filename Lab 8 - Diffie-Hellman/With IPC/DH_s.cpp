// Sender side of Diffie-Hellman - using message queue
// All Message Queue functions are defined in MSGQ_header.hpp
// Rudraksh Kapil - 177154
#include <gmp.h>
#include <iostream>
#include "MSGQ_header.hpp"
#include <math.h>
using namespace std;

// MAIN
int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "     Sender Side of Diffie Hellman Key Exchange\n";
    cout << "---------------------------------------------------\n";

    // set up message queue
    int msq_id = setup_msq(); 
    
    // initialization of variables and random seed
    mpz_t c2,temp,x,y,a,b,A,B,gen,q,prk,pubk,n,t,c1;
    int i, bt;
    unsigned long seed;
    mpz_inits(c2,temp,x,y,a,b,A,B,gen,q,prk,pubk,n,t,c1,NULL);
  
    mpz_set_ui(b, 1000);  			// Random number generation
    gmp_randstate_t s;
    gmp_randinit_mt(s);

    seed = 0;
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
    // mpz_urandomm(y,s,temp); // y = ^^
    mpz_powm(a,gen,x,n);    // a = gen ^ x % n
    // mpz_powm(b,gen,y,n);    // b = gen ^ y % n
    gmp_printf("\n[Private Key] A sends => %Zd\n", a);

    // send a and receive b
    // store info in data object
    mpz_inits(to_send.msg, NULL);
    mpz_set(to_send.msg, a);  

    //send message to receiver
    m.msg_type = 1;
    m.msg = to_send;
    assert(msgsnd(msq_id, &m, sizeof(m), 0) == 0);
    cout << "[a sent]\n";    

    //receive message of b
    cout << "Waiting for receiver...\n";
    assert(msgrcv(msq_id, &m, sizeof(m), 2, MSG_NOERROR) == sizeof(struct msg_ds));
    mpz_set(b, to_send.msg);
    gmp_printf("Got b - %Zd", a);

    mpz_powm(A,b,x,n);      // A = b ^ x % n
    //mpz_powm(B,a,y,n);      // B = a ^ y % n
    gmp_printf("\n[Common Secret Key] A computes => %Zd\n", A);
}
   
