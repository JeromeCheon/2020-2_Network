#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define BUFSIZE 10000
#define FILENAME 30
// client
// usage: ./execute_file SERVER_IP_ADDR PORT_ADDR FILE_NAME
void error_handling(char* message) ;

int main(int argc, char* argv[]){
	int sock ;
	char data[BUFSIZE];
	int str_len, i ;
	socklen_t addr_size ;
	FILE * file = NULL;
	time_t t_start;
	time_t t_end;
	struct sockaddr_in serv_addr;
	int test_data = 1;

	if(argc != 4){
		printf("Usage : %s <IP> <PORT> <FILE_NAME> \n", argv[0]) ;
		exit(1) ;
	}

	char file_name[FILENAME];
	strcpy(file_name, argv[3]);
	
	printf("File name: %s\n", file_name);
	sock = socket(PF_INET, SOCK_STREAM, 0) ;
	if( sock == -1)
		error_handling("socket() error") ;
	
	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]) ;
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	file = fopen(file_name, "r"); 
	if(file == NULL)
		error_handling("File open error....\n");
	
	// To find file size
	fseek(file, 0, SEEK_END);
	size_t fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	else
		puts("Connection succeeded");
	
	printf("Sending file size: %d\n", fsize);	
	
	printf("Send file name & file size to the server.\n");
	
	// transfer file name & file size to the server
	send(sock, file_name, FILENAME, 0);
	send(sock, &fsize, sizeof(fsize), 0);
	printf("Successfully send two information.\n");
		
	printf("Start to send data to the server.\n");
	size_t current_size = 0;
	size_t sending_fsize = 0;

	time(&t_start);
	char * t_str1 = ctime(&t_start);
	printf("Start time: %s\n", t_str1);

	while(current_size != fsize){
		sending_fsize = fread(data, 1, BUFSIZE, file);
		printf("sending size: %d\n", sending_fsize);
		current_size += sending_fsize;
		printf("current(accumlated) size: %d\n", current_size);
		send(sock, data, sending_fsize, 0);
		recv(sock, &test_data, sizeof(test_data), 0);
	}
	time(&t_end);
	char * t_str2 = ctime(&t_end);
	printf("End time: %s\n", t_str2);
	printf("Successfully send data to the server.\n");
	printf("Connection released...\n");
	
	close(sock);
	close(file);
	
	return 0;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}
