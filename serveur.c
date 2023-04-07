#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


int main(){
	int connexion = 0;
	char phrase[255];
	socklen_t len = sizeof(struct sockaddr_in);
	struct sockaddr_in information_server;
	struct sockaddr_in information_client;
	// creating server socket
	int socketID = socket(AF_INET,SOCK_STREAM,0);
	if(socketID == -1){
		perror("socket");
		exit(-1);
	}
	// initializing server informations
	memset(&information_server,0,sizeof(struct sockaddr_in));
	information_server.sin_port = htons(6666);
	information_server.sin_family = AF_INET;
	// binding server socket to server informations
	if( bind(socketID, (struct sockaddr*) &information_server, sizeof(struct sockaddr)) == -1){
		perror("bind");
		exit(-1);	
	}
	// specifiying queue length
	if( listen(socketID, 5) == -1 ){
		perror("listen");
		exit(-1);
	}
	// waiting for connection request from client
	memset(&information_client,0,sizeof(struct sockaddr_in));
	connexion = accept(socketID, (struct sockaddr *) &information_client, &len);
	if( connexion == -1){
		perror("accept");
		exit(-1);
	}
	printf("Connection accepted from %s\n",inet_ntoa(information_client.sin_addr));
	// communication
	do{
		// waiting for client message
		memset(phrase,0,255);
		recv(connexion,phrase,255,0);
		printf("<--- Message received: %s\n",phrase);
		// sending response
		if(strcmp(phrase,"EXIT") != 0){
			printf("Message: ");
			scanf("%s",phrase);
			send(connexion,phrase,strlen(phrase),0);
			printf("---> Message sent: %s\n",phrase);
		}
	}while( strcmp(phrase,"EXIT") != 0);
	// closing server socket
	shutdown(socketID,SHUT_RDWR);
	return 0;
}
