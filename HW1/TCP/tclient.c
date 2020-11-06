#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message) ;

// make sure there is no connect() function unlike tcp
int main(int argc, char* argv[]){
	int sock ;
	char message[BUF_SIZE] ;
	int str_len, i ;
	socklen_t adr_size ;


	char MSG[3][20] = {"Good", "Evening", "Everybody!"};

	struct sockaddr_in serv_adr;

	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]) ;
		exit(1) ;
	}

	sock = socket(PF_INET, SOCK_STREAM, 0) ;
	if( sock == -1)
		error_handling("socket() error") ;

	memset(&serv_adr, 0, sizeof(serv_adr)) ;
	serv_adr.sin_family = AF_INET ;
	serv_adr.sin_addr.s_addr = inet_addr(argv[2]) ;
	serv_adr.sin_port = htons(atoi(argv[1])) ;

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");
	else
		puts("Connection succeeded");


	for(i = 0 ; i < 3 ; i++){
		write(sock, MSG[i], strlen(MSG[i]));
		str_len = read(sock, message, BUF_SIZE);
		message[str_len] = 0;
		printf(" Message %d th from server: %s\n", i, message) ;
	}
	
	close(sock) ;
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc("\n", stderr) ;
	exit(1) ;
}