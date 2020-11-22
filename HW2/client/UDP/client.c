#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#define BUFSIZE 1024
#define FILENAME 30

void error_handling(char* message) ;

// make sure there is no connect() function unlike tcp
int main(int argc, char* argv[]){
	int sock ;
	char ack_message[BUFSIZE] ;
	char file_name[FILENAME];
	char data[BUFSIZE];
	size_t file_size;
	socklen_t serv_addr_size ;
	FILE * file = NULL;
	struct timeval optVal = {5, 0}; // {5, 0}; // Time-Out
	int optLen = sizeof(optVal);
	char * send_ack = "ACK";
	char * end_of_file = "eof";
	char recv_ack[BUFSIZE];
	int recv_byte = 0;
	struct sockaddr_in serv_addr, from_addr ;

	if(argc != 4){
		printf("Usage : %s <IP> <port> <file name>\n", argv[0]) ;
		exit(1) ;
	}

	strcpy(file_name, argv[3]);
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &optVal, optLen);

	if( sock == -1)
		error_handling("socket() error") ;

	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]) ;
	serv_addr.sin_port = htons(atoi(argv[2])) ;

	file = fopen(file_name, "r");
	if(file == NULL)
		error_handling("file open error...\n");

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	// sending file name and size from client to server

	printf("Sending file name to the server!\n");
	printf("File name: %s\n", file_name);
	
	while(1){
		serv_addr_size = sizeof(serv_addr);
		sendto(sock, file_name, FILENAME, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		int ret = recvfrom(sock, recv_ack, BUFSIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_size);
		if(ret != -1){
			break;
		}
	}
	
	printf("Successfully send file name and receive corresponding ack from the server.\n");
	
	int cnt = 0;
	while(cnt < 3)
	{
		sendto(sock, send_ack, strlen(send_ack), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		cnt++;
	}

	sleep(5);
	size_t current_size = 0;
	size_t sending_fsize = 0;
	
	while(current_size != file_size){
		sending_fsize = fread(data, 1, BUFSIZE, file);
		printf("sending size: %d\n", sending_fsize);
		current_size += sending_fsize;
		printf("current(accumlated) size: %d\n", current_size);
		printf("\n");
		sendto(sock, data, sending_fsize, 0,
				(struct sockaddr*)&serv_addr, sizeof(serv_addr)) ;
	}
	printf("Successfully send data to the server.\n");


	// send EOF signal to the server.

	printf("Send EOF signal to the server.\n\n");
	sleep(5);
	cnt = 0;
	while(cnt < 3)
	{
		sendto(sock, end_of_file, strlen(end_of_file), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		cnt++;
	}
	

	printf("Connection released....\n");
	

	close(file);
	close(sock) ;
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}
