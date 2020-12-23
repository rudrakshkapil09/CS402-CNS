// Receiver side of Vigenere Cipher - using shared memory
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

//decrypt function
string vigenere_decrypt(string cipher, string keyword)
{
    //plain text is to be returned
    string plain = "";

    //get key by repeating keyword
    string key = "";
    for (int i = 0; key.size() != cipher.size(); i = (i+1)%keyword.size())
        key += keyword[i]-'a';

    //convert each letter by adding key[i] (and then modulo 26)
    int spaces = 0;  // keeps track of how many whitespaces encountered in plain text
    for (int i = 0; i < cipher.size(); i++) {
        if (cipher[i] == ' ') {
            plain += ' ';
            spaces++; 
        }
        else {
            int ind = (cipher[i]-'a' - key[max(0,i-spaces)] - 1 + 26) % 26;
            plain += ind+'a';
        }
    }

    cout << "Decrypted text => " << plain << "\n";
    return plain;
}


//MAIN
int main()
{
    printf("\n\n---------------------------------------------------\n");
    printf("Receiver side of Vigenere Cipher\n");
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
    vigenere_decrypt(m.msg.cipher, m.msg.keyword);

    //destroy msgq
    //msgctl(msq_id, IPC_RMID, NULL);
    cout << "Deleted message queue.\n";

}
