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

int main(int argc, char *argv[]){
	char clientStr[] = "client";
	char serverStr[] = "server";
	char userCommand[128];

	//prompt user for client or server creation
	if(argc < 2){
		printf("client || server");
		return 1;
	}

	strcpy(userCommand, argv[1]);
	
	if(strcmp(clientStr, userCommand) == 0){
		//clientSetup();
	}
	else if(strcmp(serverStr, userCommand) == 0){
		//serverSetup();
	}	

	printf("%s\n", userCommand);
	return 0;
}

	//setup server
void serverSetup(){
		//create IPC with Unix Message queue form
		
		//While not EOF		

			//While there's input in the Message queue
				//
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
		
		//Destroy the IPC Message queue
		//Destroy the server
}

	//setup client
		//Create a thread
			//if an event occurs in the IPC Message queue 
				//if the new message's type_PID is the same as this process's PID
					//print the message data
		//while not EOF
			//Prompt the user for a filename
			//save the filename		
			//Prompt for the message's priority
			//save the priority
			//if the priority is between 1 and 10
				//Save this process's PID
				//Create a message object
					//length is the length of the file's conent
					//type_PID is this process's PID
					//type_priority is the priority set by the user
					//message data is filename set by the user
				//send the message to the IPC Message queue
			//otherwise, 
				//print "priority must be between 1 and 10"
		