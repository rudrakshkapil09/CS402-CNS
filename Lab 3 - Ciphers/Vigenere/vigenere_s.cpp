// Sender side of Vigenere Cipher - using shared memory
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
//function to set up message queue
int setup_msq()
{
    key_t key;
	int msq_id;

    //get key of IPC and get message Q id
	if ((key = ftok("./", 100)) == -1);

	//get the queue after clearing old one
	msq_id = msgget(key, 0666 | IPC_CREAT);
    msgctl(msq_id, IPC_RMID, NULL);
	msq_id = msgget(key, 0666 | IPC_CREAT);
	printf("Message Queue Created\nID: %d\n", msq_id);

    return msq_id;
}

//encrypt function
string vigenere_encrypt(string plain, string keyword)
{
    //cipher text is to be returned
    string cipher = "";

    //get key by repeating keyword
    string key = "";
    for (int i = 0; key.size() != plain.size(); i = (i+1)%keyword.size()) {
        key += keyword[i]-'a';
    }

    //convert each letter by adding key[i] (and then modulo 26)
    int spaces = 0;  // keeps track of how many whitespaces encountered in plain text
    for (int i = 0; i < plain.size(); i++) {
        if (plain[i] == ' ') {
            cipher += ' ';
            spaces++; 
        }
        else { //key[i-spaces] becuase we dont want to use a key element for whitespace
            int ind = (plain[i]-'a' + key[max(0,i-spaces)] + 1) % 26; //max to avoid -ve index
            cipher += ind+'a';
        }
    }

    //return
    cout << "Encrypted text => " << cipher << "\n";
    return cipher;
}

int main()
{
    printf("\n\n---------------------------------------------------\n");
    printf("Sender side of Vigenere Cipher\n");
    printf("---------------------------------------------------\n");

    //set up message queue
    int msq_id = setup_msq();

    //local variables
    string plain;
    string keyword;

    //get input
    cout << "Enter text to encrypt: ";
    getline(cin, plain);
    cout << "Enter keyword: ";
    cin >> keyword;

    //print outputs
    printf("\n\n-----Results:-----\n");

    // call function
    string cipher = vigenere_encrypt(plain, keyword);

    // store info in data object
    to_send.cipher = cipher;
    to_send.keyword = keyword;
    cout << "\nKey and cipher text stored in data object\n";

    //send message to receiver
    m.msg_type = 1;
    m.msg = to_send;
    assert(msgsnd(msq_id, &m, sizeof(m), 0) == 0);
    cout << "Message sent - receiver can decrypt now\n";
}
