// Sender side of AES - using message queue
// All AES functions are defined in AES_header.hpp
// All Message Queue functions are defined in MSGQ_header.hpp
// Rudraksh Kapil - 177154
#include <iostream>
#include "AES_header.hpp"
#include "MSGQ_header.hpp"
using namespace std;

int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "              Sender side of AES\n";
    cout << "---------------------------------------------------\n";

    // set up message queue
    int msq_id = setup_msq(); 

    // local variables -> 128 bits hence only 10 rounds
    int number_of_rounds = 10;
    int K[16]    = {0x6E, 0x6D, 0x4B, 0x74, 0x71, 0x20, 0x68, 0x79, 
                          0x61, 0x20, 0x70, 0x75, 0x67, 0x20, 0x44, 0x75};
    int P[16] = {0x14, 0x47, 0x6F, 0x51, 0x47, 0x64, 0x75, 0x20, 
                          0x4A, 0x59, 0x6F, 0x65, 0x20, 0x44, 0x77, 0x6E};
    vector<int> plain_text(P, P + 16);    // 128 bits
    vector<int> keyword(K, K + 16);       // 128 bits
 
    // print input
    cout << "\nPlaintext: ";
    print_vector(plain_text);
    cout << "\nKey:       ";
    print_vector(keyword);
    cout << "\n";

	// get round keys
	vector<vector<int> > round_keys = key_schedule(keyword, number_of_rounds);

    // apply AES rounds
    vector<int> cipher = apply_rounds(plain_text, round_keys, number_of_rounds);

    //final print
    cout << "\nFinal Encrypted Ciphertext => ";
    print_vector(cipher);

    // store info in data object
    for (int i = 0; i < 16; i++) {
        to_send.cipher[i] = cipher[i];
        to_send.keyword[i] = keyword[i]; 
    }
    cout << "\nKey and cipher text stored in data object\n";

    //send message to receiver
    m.msg_type = 1;
    m.msg = to_send;
    assert(msgsnd(msq_id, &m, sizeof(m), 0) == 0);
    cout << "Message sent - receiver can decrypt now\n";    
}
