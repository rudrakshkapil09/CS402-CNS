// Sender side of Simple Substitution Cipher - using message queue
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
	if ((key = ftok("./", 100)) == -1)
        err_sys("ftok");

	//get the queue after clearing old one
	msq_id = msgget(key, 0666 | IPC_CREAT);
    msgctl(msq_id, IPC_RMID, NULL);
	msq_id = msgget(key, 0666 | IPC_CREAT);
	printf("Message Queue Created\nID: %d\n", msq_id);

    return msq_id;
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

//encrypt function
//first get cipher alphabet using keyword then map plain to cyber
string simple_encrypt(string plain, string keyword) 
{
    //cipher text is to be returned
    string cipher = "";

    //get cipher alphabet
    string cipher_alphabet = get_cipher_alphabet(keyword);
    cout << "Cipher Alphabet -> " << cipher_alphabet << "\n";

    //map
    for (int i = 0; i < plain.size(); i++) {
        if (plain[i] == ' ')
            cipher += ' ';
        else {
            int ind = tolower(plain[i]) - 'a';
            cipher += cipher_alphabet[ind];
        }
    }

    //return
    cout << "Encrypted text => " << cipher << "\n";
    return cipher;
}

int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "Sender side of Simple Substitution Cipher\n";
    cout << "---------------------------------------------------\n";

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
    string cipher = simple_encrypt(plain, keyword); 

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
