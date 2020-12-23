// INCLUDE STATEMENTS
#include <iostream>
#include <string>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <assert.h>
#include <signal.h>
#include <vector>
#include <unordered_map>
#include "AES_Lookups.hpp"
using namespace std;


// ---------------------------------------------------------
//           AES ENCRYPTION and UTILITY FUNCTIONS
// ---------------------------------------------------------
// print vector X
void print_vector(vector<int> X)
{
    for (int i = 0; i < X.size(); i++)
        cout << hex << X[i] << " ";
}

// get a slice of a vector (indices X to Y)
vector<int> slicing(vector<int> &arr, int X, int Y)
{
    // starting and Ending iterators
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y + 1;

    // to store the sliced vector
    vector<int> result(Y - X + 1);

    // copy vector and return
    copy(start, end, result.begin());
    return result;
}

// AES g function for key schedule:
vector<int> g(vector<int> Word, int round_num)
{
    // swap positions of the 4 bytes
    vector<int> V(4);
    V[0] = Word[1];
    V[1] = Word[2];
    V[2] = Word[3];
    V[3] = Word[0];

    // apply S box for all 4 bytes
    for (int i = 0; i < 4; i++)
        V[i] = SBox[V[i]];

    // apply XOR operation with RC for this round 
    V[0] ^= RC[round_num];
    return V;
}

// XOR of two vectors A and B -> returns result in A
vector<int> XOR(vector<int> A, vector<int> B)
{
    for (int i = 0; i < A.size(); i++)
        A[i] = A[i] ^ B[i];
    return A;
}

// append vector B to vector A
vector<int> append(vector<int> A, vector<int> B)
{
    for (int i = 0; i < B.size(); i++)
        A.push_back(B[i]);
    return A;
}

// Key schedule to get keys (128 bit)
vector<vector<int>> key_schedule(vector<int> key, int num_rounds)
{
    // First is just the initial key
    vector<vector<int>> round_keys;
    round_keys.push_back(key); 

    //cout << "Round keys:\n";
    for (int i = 0; i < num_rounds; i++)
    {
        // current round key - W[0] to W[3] for this round are appended in turn
        vector<int> current_round_key;

        // for W0: we need to XOR the first byte with g(last byte)
        vector<int> g_result = g(slicing(key, 12, 15), i + 1);
        current_round_key = append(current_round_key, XOR(g_result, slicing(key, 0, 3)));

        // for W1: XOR W0 (rn alone in current_round_key) with second byte
        vector<int> W1 = XOR(current_round_key, slicing(key, 4, 7));
        current_round_key = append(current_round_key, W1);

        // for W2: XOR W1 with third byte 
        vector<int> W2 = XOR(W1, slicing(key, 8, 11));
        current_round_key = append(current_round_key, W2);

        // for W3: XOR W3 with last byte
        vector<int> W3 = XOR(W2, slicing(key, 12, 15));
        current_round_key = append(current_round_key, W3);

        // update key for next iteration and add to list
        key = current_round_key;
        round_keys.push_back(key);

        // print key
        //printf(" [Subkey %2d] => ", i+1);
        //print_vector(key);
    }
    return round_keys;
}

// Mix Columns sublayer function
vector<int> mix_columns(vector<int> B)
{
    // here we use the lookup tables mul2 (multiplication with 02) and mul3 (with 03)
    // 01  is identity so we use it as is. Addition is in GF(2) hence we use XOR
    vector<int> C(16);
    for (int i = 0; i < 16; i += 4)
    {
        C[i + 0] = mul2[B[i]] ^ mul3[B[i + 1]] ^ B[i + 2] ^ B[i + 3];
        C[i + 1] = B[i] ^ mul2[B[i + 1]] ^ mul3[B[i + 2]] ^ B[i + 3];
        C[i + 2] = B[i] ^ B[i + 1] ^ mul2[B[i + 2]] ^ mul3[B[i + 3]];
        C[i + 3] = mul3[B[i]] ^ B[i + 1] ^ B[i + 2] ^ mul2[B[i + 3]];
    }
    return C;
}

// applies the AES rounds 
vector<int> apply_rounds(vector<int> plain_text, vector<vector<int> > round_keys, int number_of_rounds)
{
    // first key addition layer - make a copy of plaintext, X
    vector<int> X = plain_text; 
    X = XOR(X, round_keys[0]);

    // for each round perform the three (4 with sublayers) layer functions
    cout << "\n[ROUNDS]: \n";
	cout << "                        ROUND OUTPUT                                            ROUND KEY                     \n";
	cout << "-------+--------------------------------------------------+--------------------------------------------------+\n";
    for (int i = 0; i < number_of_rounds; i++)
    {
        // byte substitution layer using SBox
        for (int j = 0; j < X.size(); j++)
            X[j] = SBox[X[j]];

        // shift rows sublayer 
        vector<int> B(16);
        for (int j = 0; j < 16; j++)
            B[j] = X[ShiftRowMatrix[j]];

        // mix columns sublayer (all except last round)
        if (i != number_of_rounds-1)
            B = mix_columns(B);

        // key addition layer
        X = B;
        X = XOR(X, round_keys[i + 1]);

        printf("  [%2d] |  ", i+1);
        print_vector(X);
        printf("    ");
        print_vector(round_keys[i+1]);
        printf("\n");
    }

    return X;
}

// ---------------------------------------------------------
//           AES DECRYPTION SPECIFIC FUNCTIONS
// ---------------------------------------------------------

// Inversed mix column operation
vector<int> inverse_mix_columns(vector<int> B)
{
    // here we use different lookup tables since we need to mulitply with 9, 11, 14, and 13
    vector<int> C(16);
    for (int i = 0; i < 16; i += 4)
    {
        C[i] = mul14[B[i]] ^ mul11[B[i + 1]] ^ mul13[B[i + 2]] ^ mul9[B[i + 3]];
        C[i + 1] = mul9[B[i]] ^ mul14[B[i + 1]] ^ mul11[B[i + 2]] ^ mul13[B[i + 3]];
        C[i + 2] = mul13[B[i]] ^ mul9[B[i + 1]] ^ mul14[B[i + 2]] ^ mul11[B[i + 3]];
        C[i + 3] = mul11[B[i]] ^ mul13[B[i + 1]] ^ mul9[B[i + 2]] ^ mul14[B[i + 3]];
    }
    return C;
}


// applies the AES rounds 
vector<int> apply_rounds_decryption(vector<int> cipher_text, vector<vector<int> > round_keys, int number_of_rounds)
{
    // first key addition layer - make a copy of plaintext, X
    vector<int> X = cipher_text; 

    // for each round perform the three (4 with sublayers) layer functions
    cout << "\n[ROUNDS]: \n";
	cout << "                        ROUND OUTPUT                                            ROUND KEY                     \n";
	cout << "-------+--------------------------------------------------+--------------------------------------------------+\n";
    for (int i = 0; i < number_of_rounds; i++)
    {
        // key addition
        X = XOR(X, round_keys[i]);

        // inv mix columns sublayer (all except first round)
        if (i != 0)
            X = inverse_mix_columns(X);

        // shift rows sublayer (inverse)
        vector<int> B(16);
        for (int j = 0; j < 16; j++)
            B[j] = X[InverseShiftRowMatrix[j]];

        // byte substitution layer using SBox
        for (int j = 0; j < B.size(); j++)
            B[j] = InvSBox[B[j]]; 
        X = B;

        printf("  [%2d] |  ", i+1);
        print_vector(X);
        printf("    ");
        print_vector(round_keys[i+1]);
        printf("\n");
    }

    // last key addition
    X = XOR(X, round_keys[10]);
    return X;
}










