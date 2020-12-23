// Sender side of DES - using message queue
// All DES functions are defined in DES_header.hpp
// All Message Queue functions are defined in MSGQ_header.hpp
// Rudraksh Kapil - 177154
#include <iostream>
#include "DES_header.hpp"
#include "MSGQ_header.hpp"
using namespace std;

int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "              Sender side of DES\n";
    cout << "---------------------------------------------------\n";

    // set up message queue
    int msq_id = setup_msq(); 

    // local variables -> 16 chars = 64 bits
    string plain = "ABCD123456132536";   
    string keyword = "09182736AABBCCDD"; 

    // get input
    cout << "\nHexadecimal Plaintext: " << plain << "\n";
    cout << "Hexadecimal Key:       " << keyword << "\n\n";

	// convert to binary:
	string plain_bin = hex2bin(plain);
	string keyword_bin = hex2bin(keyword);

	// apply permutation
	plain_bin = apply_permutation(plain_bin, initial_permutation, 64);
	cout << "After Initial Permutation: " << bin2hex(plain_bin) << "\n";

	// get round keys
	vector<string> round_keys = get_round_keys(keyword_bin);

	// apply 16 rounds to the plain text
	plain_bin = apply_rounds(plain_bin, round_keys);

	// apply final permutation to get cipher text
	string cipher = apply_permutation(plain_bin, final_permutation, 64);
	cout << "Final Encrypted Ciphertext => " << bin2hex(cipher) << "\n";

    // store info in data object - both hex representations
    to_send.cipher = bin2hex(cipher);
    to_send.keyword = keyword; 
    cout << "\nKey and cipher text stored in data object\n";

    //send message to receiver
    m.msg_type = 1;
    m.msg = to_send;
    assert(msgsnd(msq_id, &m, sizeof(m), 0) == 0);
    cout << "Message sent - receiver can decrypt now\n";    
}
