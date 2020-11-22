#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#define BUFSIZE 2048
#define FILENAME 30
void error_handling(char* message) ;

int main(int argc, char* argv[]){

	int serv_sock ;

	char data[BUFSIZE] ;
	char buf[FILENAME];
	char file_name[FILENAME];

	size_t recv_byte = 0;
	FILE * file = NULL;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	int send_ack= 1;
	int recv_ack = 0;
	char ack_message[BUFSIZE] = "ACK!";

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]) ;
		exit(1) ;
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock == -1)
		error_handling("UDP socket creation error!") ;


	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	serv_addr.sin_port = htons(atoi(argv[1])) ;
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error") ;
	
	printf("Start to receive file name from server.\n");
	
	while(1){

		memset(buf, 0, sizeof(buf));
		recvfrom(serv_sock, buf, FILENAME, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		buf[strlen(buf)]='\0';
		
		if(strcmp(buf,"ACK") == 0)
			break;

		// for remaining eof packet in receive buffer(OS)
		else if(strcmp(buf, "eof") != 0)
		{
			strcpy(file_name, buf);
			int send_byte = sendto(serv_sock, ack_message, strlen(ack_message), 0, (struct sockaddr *)&clnt_addr, sizeof(serv_addr));
		}
	}
	
	printf("finish sending an ack to the client.\n");
	printf("Receive the above ack from server and successfully received file name from client.\n");
	printf("File name: %s\n", file_name);
	
	printf("\n\n");
	/////////////// Receive file from client. /////////////

	printf("Start to receive data from client.\n");

	file = fopen(file_name, "w");
	size_t acummlated_size = 0;
	recv_byte = 0;
	
	sleep(5);
	while(1)
	{
		memset(data, 0, sizeof(data));	
		recv_byte = recvfrom(serv_sock, data, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		
		if(recv_byte == 3)
		{
			data[strlen(data)] = '\0';
			if(strcmp(data, "ACK") == 0)
				continue;
		}
	
		if(strcmp(data, "eof") == 0) // eof
			break;

		acummlated_size += recv_byte;

		printf("recv byte: %d\n", recv_byte);
		printf("current(acuumlated) byte: %d\n", acummlated_size);
		printf("\n");
		fwrite(data, sizeof(char), recv_byte, file);
	}
	
	printf("Receive EOF signal from server.\n");
	printf("Connection released...\n");

	close(serv_sock);
	close(file);
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}
