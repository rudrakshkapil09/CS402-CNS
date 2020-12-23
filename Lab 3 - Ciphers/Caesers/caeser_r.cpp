// Receiver side of Caeser Cipher - using shared memory
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
};

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
string caeser_decrypt(string cipher, int n) 
{
    //plain text is to be returned
    string plain = "";

    // n has to be <= 26 bc we're adding 26 only in the loop
    n %= 26;    

    //subtract n from each letter
    for (int i = 0; i < cipher.size(); i++) {
        if (cipher[i] == ' ')
            plain += ' ';
        else {
            int val = (tolower(cipher[i]) - 'a' - n + 26) % 26;
            plain += 'a' + val; 
        }
    }

    cout << "Decrypted text => " << plain << "\n";
    return plain;
}

int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "Receiver side of Caeser Cipher\n";
    cout << "---------------------------------------------------\n";

    //set up shared memory
    key_t key = ftok("./", 100);
	int msq_id = msgget(key, 0666 | IPC_CREAT); 

    //receive message
    cout << "Waiting for sender...\n";
    assert(msgrcv(msq_id, &m, sizeof(m), 1, MSG_NOERROR) == sizeof(struct msg_ds));


    //print outputs
    cout << "\n\n-----Results:-----\n";
    cout << "Received => " << m.msg.cipher << "\n";
    caeser_decrypt(m.msg.cipher, m.msg.n);

    //destroy msgq
    msgctl(msq_id, IPC_RMID, NULL);
    cout << "Deleted message queue.\n";
}
