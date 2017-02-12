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
int serverSetup(key_t mkey, int msg_id){
	int permflags;	//holds the result of msgget
	int result, length;
	Mesg mesgbuf;

	//the length is essetnailly the size of the structure minus sizeof(mtype)
	length = sizeof(Mesg) - sizeof(long);
	
	
	//get and print the status information 
	//mqstat_print(mkey, msg_id, &msq_status);

	//While not EOF	
	while(1){
		//While there's input in the Message queue
		//
		printf("checking msgrcv\n");
		if((result = msgrcv(msg_id, &mesgbuf, length, 0, 0)) == -1){
			printf("Failed: msgrcv %d\n", result);
			return -1;
		}
		printf("Successful: msgrcv %d\n", result);

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

//setup client
int clientSetup(int msg_id){
	int size, flags, retval;
	char* quit = "q";
	char filename[1024];
	char* priorityChar;
	long priority;

	//Create a thread
		//if an event occurs in the IPC Message queue 
			//if the new message's type_PID is the same as this process's PID
				//print the message data
	//while not EOF
	while(1){
		//Prompt the user for a filename
		printf("Enter input (q quit):\n");

		//save the filename
		gets(filename);
		
		//kill threads and exit
		if(strcmp(filename, quit) == 0){
			printf("Exiting...");
			return 0;
		}
		//Prompt for the message's priority
		printf("Enter priority (1max - 3min): \n");

		//save the priority
		gets(priorityChar);
		atoi(priorityChar);

		//if the priority is between 1 and 10
		if((priority >= 1) && (priority <= 3)){
			//Save this process's PID
			//Create a message object
			Mesg message;
			char hello[MAXMESSAGEDATA] = "hello";
			strcpy(message.mesg_data, hello);
			message.mesg_type = 1;
			message.mesg_len = strlen(message.mesg_data);

				
				//length is the length of the file's conent
				//type_PID is this process's PID
				//type_priority is the priority set by the user
				//message data is filename set by the user
			//send the message to the IPC Message queue
			int result, length;

			//size of the message structure minus the type 
			length = message.mesg_len;
			//check if the third parameter (length) is correct. Might be sizeof(message)-size(long)
			if((result = msgsnd(msg_id, &message, length, 0) == -1))
			{
				printf("Failed: msgsnd %d\n", result);
				return (-1);
			}
			printf("Successful: msgsnd %d\n", result);
		//otherwise, 
			//print "priority must be between 1 and 10"
		}
		
	}

	return 0;
}
