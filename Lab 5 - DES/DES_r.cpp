// Receiver side of DES - using message queue
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
    cout << "              Receiver side of DES\n";
    cout << "---------------------------------------------------\n";

    //set up shared memory
    key_t key = ftok("./", 100);
	int msq_id = msgget(key, 0666 | IPC_CREAT); 

    //receive message
    cout << "Waiting for sender...\n";
    assert(msgrcv(msq_id, &m, sizeof(m), 1, MSG_NOERROR) == sizeof(struct msg_ds));

    //print outputs
    string cipher = m.msg.cipher;
    string keyword = m.msg.keyword;
    cout << "\nReceived Hexadecimal Cipher => " << cipher << "\n";
    cout << "Received Hexadecimal Key    => " << keyword << "\n";

	// convert to binary:
	string cipher_bin = hex2bin(cipher);
	string keyword_bin = hex2bin(keyword);

	// apply permutation
	cipher_bin = apply_permutation(cipher_bin, initial_permutation, 64);
	cout << "After Initial Permutation: " << bin2hex(cipher_bin) << "\n";

	// get round keys - then reverse order
	vector<string> round_keys = get_round_keys(keyword_bin);
    reverse(round_keys.begin(), round_keys.end());

	// apply 16 rounds to the plain text
	cipher_bin = apply_rounds(cipher_bin, round_keys);

	// apply final permutation to get cipher text
	string plain = apply_permutation(cipher_bin, final_permutation, 64);
	cout << "Final Decrypted Plaintext => " << bin2hex(plain) << "\n";

    //destroy msgq
    msgctl(msq_id, IPC_RMID, NULL);
    cout << "\nDeleted message queue.\n"; 
  
}
