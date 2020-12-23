// Sender side of Playfair Cipher - using shared memory
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
//function to set up message queue
int setup_msq()
{
    key_t key;
	int msq_id;

    //get key of IPC and get message Q id
	if ((key = ftok("./", 100)) == -1)
        ;

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

//preprocess plain text to handle duplicates and remove j & have even length
string get_processed_plain(string plain)
{    
    //remove white spaces
    int spaces = 0;
    for (int i = 0; i < plain.size(); i++) {
        if (plain[i] == ' ') {
            spaces++;
            to_send.space_indices[to_send.k++] = i;
        }
        if (i + spaces < plain.size())
            plain[i] = plain[i+spaces];
    }
    plain.resize(plain.size()-spaces);

    //replace j with i
    for (int i = 0; i < plain.size(); i++) {
        char c = tolower(plain[i]);
        if (c == 'j')
            c = 'i';
        plain[i] = c;
    }

    //handle duplicates - add X in between
    for (int i = 0; i < plain.size()-1; i+=2) {
        if (plain[i] == plain[i+1]) {
            if (plain[i] != 'z')
                plain.insert(i+1, 1, 'z');
            else plain.insert(i+1, 1, 'a');
        }
    }

    //if odd length, pad with one letter at end
    int n = plain.size();
    if (n & 1) {
        if (plain[n-1] != 'z')
            plain += 'z';
        else plain += 'a';

        //update message
        to_send.lastadded = true;
    }
    else to_send.lastadded = false;

    cout << "Processed plain -> ";
    for (int i = 0; i < plain.size(); i+=2)
        cout << plain[i] << plain[i+1] << " ";
    cout << "\n";
    return plain;
}

//encrypt function
string playfair_encrypt(string plain, string keyword)
{
    //cipher text is to be returned
    string cipher = "";

    //preprocess string 
    plain = get_processed_plain(plain);

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
    for (int i = 0; i < plain.size(); i+=2) {
        // get positions of both
        int x1, y1, x2, y2;
        x1 = pos[plain[i]].first; 
        y1 = pos[plain[i]].second; 
        x2 = pos[plain[i+1]].first;
        y2 = pos[plain[i+1]].second;


        // if same column take the one below for each
        if (y1 == y2) {
            cipher += key_table[(x1+1) % 5][y1];
            cipher += key_table[(x2+1) % 5][y2];
        }
        // if same row take the one on the right for reach
        else if (x1 == x2) {
            cipher += key_table[x1][(y1+1) % 5];
            cipher += key_table[x2][(y2+1) % 5];
        }
        //other wise take horizontal accross
        else {
            cipher += key_table[x1][y2];
            cipher += key_table[x2][y1];
        }
    }

    //return
    cout << "\nEncrypted text => ";
    for (int i = 0; i < cipher.size(); i+=2)
        cout << cipher[i] << cipher[i+1] << " ";
    cout << "\n";
    return cipher;
}

int main()
{
    printf("\n\n---------------------------------------------------\n");
    printf("Sender side of Playfair Cipher\n");
    printf("---------------------------------------------------\n");

    //set up message queue
    int msq_id = setup_msq();

    //local variables
    string plain;
    string keyword;

    //get input
    cout << "Enter text to encrypt: ";
    getline(cin,plain);
    cout << "Enter keyword: ";
    cin >> keyword;

    //print outputs
    printf("\n\n-----Results:-----\n");

    // call function
    to_send.k = 0;
    string cipher = playfair_encrypt(plain, keyword);

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
