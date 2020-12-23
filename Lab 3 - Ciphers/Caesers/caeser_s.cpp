// Sender side of Caeser Cipher - using Message Q
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
using namespace std;

//shared memory
struct data
{
	string cipher;
    int n;
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
	if ((key = ftok("./", 100)) == -1)
        err_sys("ftok");

	//get the queue after clearing old one
	msq_id = msgget(key, 0666 | IPC_CREAT);
    msgctl(msq_id, IPC_RMID, NULL);
	msq_id = msgget(key, 0666 | IPC_CREAT);
	printf("Message Queue Created\nID: %d\n", msq_id);

    return msq_id;
}

//encrypt function
string caeser_encrypt(string plain, int n) 
{
    //cipher text is to be returned
    string cipher = "";

    //same effect but smaller number to work with
    n %= 26;    

    //add n to each letter (mod to make it circular)
    for (int i = 0; i < plain.size(); i++) {
        //ignore whitespace
        if (plain[i] == ' ')
            cipher += ' ';
        else {
            int val = (tolower(plain[i]) - 'a' + n) % 26;
            cipher += 'a' + val; 
        }
    }

    cout << "Encrypted text => " << cipher << "\n";
    return cipher;
}


int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "Sender side of Caeser Cipher\n";
    cout << "---------------------------------------------------\n";

    //set up message queue
    int msq_id = setup_msq(); 

    //local variables
    string plain;
    int n;

    //get input
    cout << "Enter text to encrypt: ";
    getline(cin, plain);
    cout << "Enter integer key: ";
    cin >> n;

    //print outputs
    cout << "\n-----Results:-----\n";

    // call function
    string cipher = caeser_encrypt(plain, n); 

    // store info in data object
    to_send.cipher = cipher;
    to_send.n = n;
    cout << "\nKey and cipher text stored in data object\n";

    //send message to receiver
    m.msg_type = 1;
    m.msg = to_send;
    assert(msgsnd(msq_id, &m, sizeof(m), 0) == 0);
    cout << "Message sent - receiver can decrypt now\n";
}
