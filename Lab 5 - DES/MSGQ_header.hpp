// INCLUDE STATEMENTS
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

// ---------------------------------------------------------
//                  MESSAGE QUEUE FUNCTIONS
// ---------------------------------------------------------

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