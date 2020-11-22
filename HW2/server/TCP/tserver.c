#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// server
#define BUFSIZE 10000
#define FILE_NAME_SIZE 100
void error_handling(char* message) ;

int main(int argc, char* argv[]){
	int serv_sock;
	int clnt_sock;

	char file_name[100];
	char data[BUFSIZE];

	size_t fsize = 0;
	size_t recv_byte = 0;
	int clnt_addr_size ;
	int test_data = 2;

	struct sockaddr_in serv_addr ;
	struct sockaddr_in clnt_addr ;

	FILE * file = NULL;

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]) ;
		exit(1) ;
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("TCP socket creation error!") ;

	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	serv_addr.sin_port = htons(atoi(argv[1])) ;

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error") ;

	/*  TCP	*/
	if(listen(serv_sock, 5) == -1)
		error_handling("listen error.....");

	clnt_addr_size = sizeof(clnt_addr);

	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	if(clnt_sock == -1)
		error_handling("accept error.....");
	else
		printf("Connected!\n");
	
	size_t str_len = recv(clnt_sock, file_name, FILE_NAME_SIZE, 0);
	file_name[str_len] = "\0";

	recv(clnt_sock, &fsize, sizeof(fsize), 0);

	printf("file name: %s\n", file_name);
	printf("file size: %d\n", fsize);

	file = fopen(file_name, "w");
	
	printf("receving file from client...\n");
	
	size_t accumlated_size = 0;
	while(1)
	{
		recv_byte = recv(clnt_sock, data, BUFSIZE, 0);
		accumlated_size += recv_byte;
		printf("recv byte: %d\n", recv_byte);
		printf("current(accumlated) size: %d\n", accumlated_size);
		if (recv_byte == 0)
			break;
		fwrite(data, sizeof(char), recv_byte, file);
		int check = send(clnt_sock, &test_data, sizeof(test_data), 0);
		printf("check: %d\n", check);
	}

	printf("Finish receving data from client...\n");


	close(clnt_sock);
	close(file);
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}

