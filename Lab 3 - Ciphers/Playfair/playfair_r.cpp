// Receiver side of Playfair Cipher - using shared memory
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
#include <tuple>
#include <functional>
#include <map>
using namespace std;


//shared memory
struct data
{
	string cipher;
    string keyword;
    bool lastadded; //tells receiver if they should drop the last letter since it was added for encrypion purposes
    int space_indices[100];
    int k;
    //vector<int> space_indices; // holds indices of the spaces in the plain text to tell the receiver to add them
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
    //add all letters to key
    string key = keyword + "abcdefghijklmnopqrstuvwxyz";
    string cipher_alphabet = "";

    //go through key and add to c_a only those letters not already present
    for (string::iterator it = key.begin(); it != key.end(); it++) {
        *it = tolower(*it);
        if (*it == 'j') //j is not in table
            continue;
        if (cipher_alphabet.find(*it) == -1)
            cipher_alphabet += *it;
    }

    return cipher_alphabet;
}

//function to generate and return key table
// -> keyword + remaining letters in natural order, arranged in 5x5 table
vector<vector<char> > get_key_table(string keyword)
{
    vector<vector<char> > key_table(5, vector<char>(5));

    // get order of letters and then arrange them in 5x5 grid
    string order = get_cipher_alphabet(keyword);

    // arrange in table
    int k = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%3c", order[k]);
            key_table[i][j] = order[k++];
        }
        cout << "\n";
    }

    return key_table;
}


//decrypt function
string playfair_decrypt(string cipher, string keyword) 
{
    //plain text is to be returned
    string plain = "";

    //print what you got
    cout << "Split -> ";
    for (int i = 0; i < cipher.size(); i+=2)
        cout << cipher[i] << cipher[i+1] << " ";
    cout << "\n";

    //get key table
    cout << "\n [Table]:\n";
    vector<vector<char> > key_table = get_key_table(keyword);

    //create reverse reference for key_table
    map<char, pair<int, int> > pos;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            pos[key_table[i][j]].first  = i;
            pos[key_table[i][j]].second = j;
        }
    }

    //for each pair of elements:
    for (int i = 0; i < cipher.size(); i+=2) {
        // get positions of both
        int x1, y1, x2, y2;
        x1 = pos[cipher[i]].first; 
        y1 = pos[cipher[i]].second; 
        x2 = pos[cipher[i+1]].first;
        y2 = pos[cipher[i+1]].second;


        // if same column take the one below for each
        if (y1 == y2) {
            plain += key_table[(x1+4) % 5][y1];
            plain += key_table[(x2+4) % 5][y2];
        }
        // if same row take the one on the right for reach
        else if (x1 == x2) {
            plain += key_table[x1][(y1+4) % 5];
            plain += key_table[x2][(y2+4) % 5];
        }
        //other wise take horizontal accross
        else {
            plain += key_table[x1][y2];
            plain += key_table[x2][y1];
        }
    }

    //remove last letter if required
    if (m.msg.lastadded)
        plain.pop_back();

    //add white spaces
    for (int i = 0; i < m.msg.k; i++) {
        plain.insert(m.msg.space_indices[i], " ");
    }

    cout << "Decrypted text => " << plain << "\n";
    return plain;
}


int main()
{
    printf("\n\n---------------------------------------------------\n");
    printf("Receiver side of Caeser Cipher\n");
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
    playfair_decrypt(m.msg.cipher, m.msg.keyword);

    //destroy msgq
    msgctl(msq_id, IPC_RMID, NULL);
    cout << "Deleted message queue.\n";

    
}
