// Receiver side of Simple Subsitution Cipher - using shared memory
// Rudraksh Kapil - 177154
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
using namespace std;

//shared memory
struct data
{
	string cipher;
    string keyword;
} to_send;

//structure for messages
struct msg_ds
{
	long msg_type;
	struct data msg;
} m;

//error print function
void err_sys(char *msg)
{
	perror(msg);
	exit(-1);
}


//function to generate and return cipher alphabet
//cipher alphabet -> keyword + remaining letters in natural order
string get_cipher_alphabet(string keyword)
{
    string cipher_alphabet = "";

    // first -> create map to keep track of seen alphabets
    vector<int> seen(26, 0);

    // second -> add keyword letters
    for (int i = 0; i < keyword.size(); i++) {
        int ind = tolower(keyword[i]) - 'a';     //get index of keyword letter 
        
        //iff not seen then add to alphabet
        if (seen[ind] == 0) {
            cipher_alphabet += 'a' + ind;
            seen[ind]++;
        }
    }


    // third -> add remaining letters
    for (int i = 0; i < 26; i++) {
        if (seen[i] == 0) { 
            cipher_alphabet += 'a' + i;
            seen[i]++;
        }
    }

    return cipher_alphabet;
}

//decrypt function
string simple_decrypt(string cipher, string keyword) 
{
    //plain text is to be returned
    string plain = "";

    //get cipher alphabet
    string cipher_alphabet = get_cipher_alphabet(keyword);
    cout << "Cipher Alphabet -> " << cipher_alphabet << "\n";

    //make a lookup for cipher alphabet to plain alphabet
    // element i in lookup is index of letter at i in cipher alphabet
    vector<int> lookup(26, -1); // basically reverse reference
    for (int i = 0; i < 26; i++) {
        int ind = cipher_alphabet[i] - 'a';
        lookup[ind] = i;
    }

    //map using lookup
    for (int i = 0; i < cipher.size(); i++) {
        if (cipher[i] == ' ')
            plain += ' ';
        else {
            int ind = tolower(cipher[i]) - 'a';
            plain += lookup[ind] + 'a';
        }
    }
    
    cout << "Decrypted text => " << plain << "\n";
    return plain;
}

int main()
{
    printf("\n\n---------------------------------------------------\n");
    printf("Receiver side of Simple Subsitution Cipher\n");
    printf("---------------------------------------------------\n");

    //set up shared memory
    key_t key = ftok("./", 100);
	int msq_id = msgget(key, 0666 | IPC_CREAT); 

    //receive message
    cout << "Waiting for sender...\n";
    assert(msgrcv(msq_id, &m, sizeof(m), 1, MSG_NOERROR) == sizeof(struct msg_ds));

    //print outputs
    cout << "\n\n-----Results:-----\n";
    cout << "Received => " << m.msg.cipher << "\n";
    simple_decrypt(m.msg.cipher, m.msg.keyword);

    //destroy msgq
    msgctl(msq_id, IPC_RMID, NULL);
    cout << "Deleted message queue.\n";

    
}
