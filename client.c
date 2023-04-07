#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


int main(){
	char phrase[255];
	// initalizing server informations
	struct sockaddr_in informations;
	informations.sin_family = AF_INET;
	informations.sin_port = htons(6666);
	informations.sin_addr.s_addr = inet_addr("127.0.0.1");
	// creating client socket
	int socketID = socket(AF_INET,SOCK_STREAM,0);
	if (socketID == -1){
		perror("socket");
		exit(-1);
	}
	// sending connection request to server socket
	if( connect(socketID, (struct sockaddr*) &informations, sizeof(struct sockaddr_in)) == 1){
		perror("connect");
		exit(-1);
	}
	// communication
	do{
		// reading message from command line
		printf("Message: ");
		scanf("%s",phrase);
		// sending message to server
		if( send(socketID,phrase,strlen(phrase),0) == -1 ) perror("send");
		printf("---> Message sent: %s\n",phrase);
		// waiting for server response
		if(strcmp(phrase,"EXIT") != 0){
			memset(phrase,0,255);
			recv(socketID,phrase,255,0);
			printf("<--- Message received: %s\n",phrase);
		}
	}while(strcmp(phrase,"EXIT") != 0);
	// closing client socket
	shutdown(socketID,SHUT_RDWR);
	return 0;
}

