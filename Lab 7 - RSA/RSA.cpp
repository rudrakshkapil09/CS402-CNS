// RSA Algrotihm using GMP library - hence we don't use IPC
// All RSA functions are defined in RSA_header.hpp
// Rudraksh Kapil - 177154
#include <iostream>
#include "RSA_header.hpp"
using namespace std;

// MAIN
int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "                  RSA Algortihm\n";
    cout << "---------------------------------------------------\n";

    // initialization of variables and random seed
    mpz_t p,q,N,Phi,p_1,q_1,e,d,Phi_2,M,C;
    mpz_inits(p,q,Phi,Phi_2,p_1,q_1,e,d,N,M,C,NULL);
    gmp_randstate_t mt;
    __gmp_randinit_default(mt);

    // choosing two random large prime numbers p and q
    while(1) {
        // Generate a random integer with long strings of zeros and ones in the binary representation
        // random number will be in the range 2^(16-1) to (2^16)-1 inclusive.
        mpz_rrandomb(p,mt,16);
        if (MillerRabin(p))
            break;
    }
    while(1) {
        mpz_rrandomb(q,mt,16);
        if (MillerRabin(q))
            break;
    }

    // Generating Public and Private Keys
    gmp_printf(" P => %10Zd, Q   => %10Zd \n",p,q);
    mpz_mul(N,p,q);                // N = p*q
    mpz_sub_ui(p_1,p,1);           // p_1 = p-1
    mpz_sub_ui(q_1,q,1);           // q_1 = q-1
    mpz_mul(Phi,p_1,q_1);          // phi = p_1 * q_1
    mpz_sub_ui(Phi_2 , Phi , 2);   // phi2 = phi - 2

    // loop
    while(true) {
        // initialise variable g
        mpz_t g;
        mpz_init(g);
        mpz_urandomm(e,mt,Phi_2); // e is uniform random number between 0 to phi2 - 1
        mpz_add_ui(e,e,2);        // e = e_2
        mpz_gcd(g,e,Phi);         // g = gcd(e, phi)
        if (mpz_cmp_ui(g,1)==0)   // if co prime, then break loop
            break;
    }

    gmp_printf("\n N => %10Zd, Phi => %10Zd, E => %10Zd \n", N , Phi,e);

    // get d by taking inverse modulo for e
    InverseModulo(d,e,Phi);
    gmp_printf("\n D => %10Zd, E   => %10Zd \n", d, e);

    // mpz_set_ui(M,15);
    gmp_printf("\nPublic key pair  (n, e) => { %Zd , %Zd}  \n", N,e);
    gmp_printf("Private key pair (n, d) => { %Zd , %Zd}  \n", N,d);

    // get input and encrypt
    cout<<"\nEnter the plain text to encrypt: ";
    gmp_scanf("%Zd",M);
    mpz_powm(C,M,e,N);   // C = M^e % N  -> C is encrypted ciphertext

    // decrypt
    gmp_printf("Cipher text => %Zd \n", C);
    mpz_powm(C,C,d,N);   // C = C^d % N -> C is decrypted plaintext
    cout << "\nDecrypting...\n";
    gmp_printf("Plain text  => %Zd ", C);
    return 0;
}

