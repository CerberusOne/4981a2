/*-----------------------------------------------------------------------
--  SOURCE FILE:   	a2.cpp   
--					An application that allows a user to setup either a 
--					client or a server. 
--					
--					The server will create an IPC channel to allow messages to be 
--					sent through. If the request for a filename	is found in the queue, 
--					the server will attempt to find the file and send the contents of 
--					it back to the process that requested it.
--					
--					The client side of the program will prompt the user for a filename
--					and send the message to the IPC channel setup earlier by the server.
--					The client will wait for the server to respond with the contents
--					of the file and print it for the user.
--
--  PROGRAM:       	COMP 4981 Assignment 2
--
--  FUNCTIONS:		
--                  
--
--  DATE:          	Feb 7, 2017
--
--  DESIGNER:      	Aing Ragunathan
--
--
--  NOTES:
--		
--		
--		
----------------------------------------------------------------------------*/

/** 
	To do:
	------
		-setup msg.h file

**/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>


#define MAXMESSAGEDATA 4096

typedef struct
{
	int mesg_len; 			//bytes in mesg_data
	long mesg_type;			//message type
	char mesg_data[MAXMESSAGEDATA];	//message content
}Mesg;

#define MESGHDRSIZE (sizeof(Mesg) - MAXMESSAGEDATA )

//sets up the server process, client/server processes and waits for signals from the IPC Channel
int serverSetup(key_t mkey, int msg_id);

//sets up the client process
int clientSetup(int msg_id);
void promptUser(char *prompt, char *filename);
Mesg createMesg(long type, char mesg_data[MAXMESSAGEDATA]);
char* createMesgData(char* pidChar, char* filename);
void processRequest(Mesg message, long* pid, char* filename, long* priority);
void getMesgPid(Mesg message, long* pid);
int sendFile(char* filename, char* messageData, long pid, long priority, int msg_id);
int sendSegment(char segment[MAXMESSAGEDATA], long pid, long priority, int msg_id);
void* checkQueue(void* msg_id);

pthread_mutex_t printfLock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]){
	char clientStr[] = "client";
	char serverStr[] = "server";
	char userCommand[128];
	key_t mkey;
	int msg_id;

	//Generate a PIC key value
	mkey = ftok (".", 'm');
	//create IPC with Unix Message queue form
	if((msg_id = msgget(mkey, 0644|IPC_CREAT)) < 0){
		printf("msg_id = %d", msg_id); 
		perror("msgget failed!");
		exit(2);
	}

	//prompt user for client or server creation
	if(argc < 2){
		printf("client || server\n");
		return 1;
	}

	strcpy(userCommand, argv[1]);
	
	if(strcmp(clientStr, userCommand) == 0){
		clientSetup(msg_id);
	}
	else if(strcmp(serverStr, userCommand) == 0){
		serverSetup(mkey, msg_id);
	}	

	return 0;
}

	
//setup server
//WIP
int serverSetup(key_t mkey, int msg_id){
	//int permflags;	//holds the result of msgget
	int result, length;
	Mesg message;
	char filename[1024];
	char messageData[MAXMESSAGEDATA];	//
	long pid = 0;
	long priority = 0;

	//the length is essetnailly the size of the structure minus sizeof(mtype)
	length = sizeof(Mesg) - sizeof(long);
	
	
	//get and print the status information 
	//mqstat_print(mkey, msg_id, &msq_status);

	//While not EOF	
	while(1){
		//While there's input in the Message queue
		printf("checking msgrcv\n");

		//change to signal later
		if((result = msgrcv(msg_id, &message, length, 0, 0)) == -1){
			printf("Failed: msgrcv %d\n", result);
			return -1;
		}

		printf("Successful: msgrcv %d\n", result);
		printf("Message(%ld): %s\n", strlen(message.mesg_data), message.mesg_data);

		//get the pid, filename(data) and priority from the message
		processRequest(message, &pid, filename, &priority);

		//printf("pid: %ld\nfilename: %s\npriority: %ld\n", pid, filename, priority);




		//Loop through every object
				//if the type_priority is not null - ASK
					//if the priority is higher than the highest found
						//save the object's index in the list
			
			//Save the highest priority object in the list
			//Delete it from the message queue
			//Save the type_PID 
			//if the type_PID is null
				//print error: found message with no PID
			//otherwise, Save the message data
				//Open a file with the name of the message data 
				

				printf("successRate: %d\n", sendFile(filename, messageData, pid, priority, msg_id));
				//printf("Success rate: %d\n", successRate);
					//open file in MAXMESSAGEDATA size segments
						//calculate size of the file if possible
							//save stat of how many segments will send
						//loop until EOF is found
							//wait for access
							//send each segment 
							//signal access
							//count each successful send




				//if the fails to open
					//if it can't be found
						//Save the message as "File can't be found"
					//else
						//Save the message as "Error opening file"
				//otherwise, 
					//create a message
						//the length is the length of the message
						//the type_PID is the saved PID 
						//the type_priority is null
						//the message is the file's content
					//Send the message to the IPC Message queue
	}		
	//Destroy the IPC Message queue
	
		//Destroy the server
	return 0;
}


void processRequest(Mesg message, long* pid, char* filename, long* priority){
	char* temp = message.mesg_data;
	char* pidChar;
	int i = 0;
	char* ptr;

	*priority = message.mesg_type;	

	while(temp){
		pidChar = strchr(temp, '\n');

		//change the new line character to null
		if(pidChar != '\0'){
			*pidChar = '\0';
		}

		//obtain PID from first line
		if(i == 0){
			*pid = strtol(temp, &ptr, 10);
		}

		//obtain filename from second line
		if(i == 1){
			strcpy(filename, temp);
		}
		
		//fix the null character back to a new line character
		if(pidChar != '\0'){
			*pidChar = '\n';
		}
	
		temp = pidChar ? (pidChar + 1) : NULL;
		i++;
	}	
}

int sendFile(char* filename, char* messageData, long pid, long priority, int msg_id){
	FILE *fp;	//pointer to file
	char segment[MAXMESSAGEDATA];
	int lineCounter = 0;

	if((fp = fopen(filename, "r+")) == 0){	//open file in append binary mode
		printf("Failed: sendFile/fileopen\n");
		return -1;
	}

	while(fgets(segment, sizeof(segment), fp)){
		printf("%s\n", segment);
		sendSegment(segment, pid, priority, msg_id);
	}

	return lineCounter;
}

//WIP
int sendSegment(char segment[MAXMESSAGEDATA], long pid, long priority, int msg_id){
	Mesg message;
	int result = 0;
	//WAIT(access)
	//make a message
	message = createMesg(pid, segment);
	//write to message queue
	if((result = msgsnd(msg_id, &message, message.mesg_len, 0) == -1))
	{
		printf("Failed: msgsnd %d\n", result);
		return (-1);
	}

	printf("Successful: data: %s\n", message.mesg_data);
	//SIGNAL(access)
	//SIGNAL(Buffer Empty)
	//If segment has EOF
		//FREE(segment)
		//Destroy process
	return 0;
}



//setup client
int clientSetup(int msg_id){
	//int size, flags, retval;
	char quit[] = "q";			//command used to exit program
	char filename[1024];		//filename of requested file to get
	char priorityChar[1024];	//priority of current message 
	long priority;				//priority of current message after being converted to long
	long pid;					//pid of current thread
	char pidChar[1024];			//pid of current thread in char* form
	int result;					//send the message to the IPC Message queue
	char mesg_data[MAXMESSAGEDATA];
	pthread_t messageQueueHelperThread;

	int *message_id = malloc(sizeof(*message_id));
	if(message_id == NULL){
		printf("Failed: allocate memory for (void*) message_id");
		return -1;
	}

	*message_id = msg_id;

	//Create a thread
	pthread_create(&messageQueueHelperThread, NULL, checkQueue, (void*) message_id);
		//if an event occurs in the IPC Message queue 
			//if the new message's type_PID is the same as this process's PID
				//print the message data

	//user input loop
	while(1){

		//prompt for filename from user
		promptUser("Enter input (q quit): ", filename);
		
		//unlock any printing from happening in client
		

		//kill threads and exit
		if(strcmp(filename, quit) == 0){
			printf("Exiting...\n");
			return 0;
		}
	
		//Prompt for the message's priority
		promptUser("Enter priority (1max - 3min): ", priorityChar);
		//conver input into a long
		priority = atol(priorityChar);

		if((priority >= 1) && (priority <= 3)){
			//get pid and save as long
			pid = getpid();
			sprintf(pidChar, "%ld", pid);
	
			//create message data
			strcpy(mesg_data, createMesgData(pidChar, filename));			

			//Create a message object
			Mesg message = createMesg(1, mesg_data);		

			//check if message gets sent properly
			if((result = msgsnd(msg_id, &message, message.mesg_len, 0) == -1))
			{
				printf("Failed: msgsnd %d\n", result);
				return (-1);
			}
		}
		else{
			//alert user that priority is not properly defined
			printf("ERROR: Priority must be between 1 (max) and 3 (min)\n");
		}
	
	}

	return 0;
}

void promptUser(char *prompt, char *filename){
	fflush(stdin);

	//Prompt the user for a filename
	printf("%s", prompt);
	
	pthread_mutex_lock(&printfLock);
	//save the filename
	fgets(filename, 32, stdin);
	
	pthread_mutex_unlock(&printfLock);

	//get rid of the new line character generated by fgets
	filename[strcspn(filename, "\n")] = '\0';
}

//length is the length of the file's conent
//message data is filename set by the user
Mesg createMesg(long type, char mesg_data[MAXMESSAGEDATA]){
	fflush(stdin);
	Mesg message;
	message.mesg_type = type;
	strcpy(message.mesg_data, mesg_data);
	//message.mesg_len = strlen(message.mesg_data);
	message.mesg_len = sizeof(Mesg) - sizeof(long);

	return message;
}

char* createMesgData(char* pidChar, char* filename){
	//string to be properly formatted for a Mesg's mesg_data attribute
	char *mesg_data;

	//format with pid on first line and rest of message on another
	mesg_data = strcat(pidChar, "\n");
	strcat(mesg_data, filename);

	return mesg_data;
}

void* checkQueue(void* msg_id){
	Mesg message;
	int result;
	int length = sizeof(Mesg) - sizeof(long);
	int message_id = *((int*)msg_id);

	printf("Starting: checkQueue thread");

	while(1){
		//WAIT(Buffer Empty)
		//WAIT(Access)

		//lock any printing from happening in client
		pthread_mutex_lock(&printfLock);

		//Check message.mesg_typ <= 3
		if((result = msgrcv(message_id, &message, length, 0, 0)) == -1){
			printf("Failed: msgrcv %d\n", result);
		}
		printf("checkQueue: %s\n", message.mesg_data);
		//unlock any printing from happening in client
		pthread_mutex_unlock(&printfLock);

			//SIGNAL(Buffer Empty)
		//SIGNAL(Acess)
		//print message data to screen
		//If data has EOF
			//Unblock MUTEX

		
	}
}
