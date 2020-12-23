// Receiver side of AES - using message queue
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
    cout << "              Receiver side of AES\n";
    cout << "---------------------------------------------------\n";

    // set up shared memory
    key_t key = ftok("./", 100);
	int msq_id = msgget(key, 0666 | IPC_CREAT); 

    // receive message
    cout << "Waiting for sender...\n";
    assert(msgrcv(msq_id, &m, sizeof(m), 1, MSG_NOERROR) == sizeof(struct msg_ds));

    // print outputs
    vector<int> cipher(m.msg.cipher, m.msg.cipher + 16);                    
    vector<int> keyword(m.msg.keyword, m.msg.keyword + 16);                    
    cout << "\nReceived Hexadecimal Cipher => ";
    print_vector(cipher);
    cout << "\nReceived Hexadecimal Key    => ";
    print_vector(keyword);
    cout << "\n";

    // get round keys - reverse since decryption
    int number_of_rounds = 10;
	vector<vector<int> > round_keys = key_schedule(keyword, number_of_rounds);
    reverse(round_keys.begin(), round_keys.end());

    // apply decryption rounds
    vector<int> plain_text = apply_rounds_decryption(cipher, round_keys, number_of_rounds);

    //final print
    cout << "\nFinal Decrypted Plaintext => ";
    print_vector(plain_text);

    //destroy msgq
    msgctl(msq_id, IPC_RMID, NULL);
    cout << "\nDeleted message queue.\n"; 
  
}
