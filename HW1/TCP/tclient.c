#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30

void error_handling(char* message) ;

int main(int argc, char* argv[]){
	int sock ;
	char message[BUFSIZE] ;
	int str_len, i ;
	socklen_t addr_size ;
	char MSG1[] = "Good" ;
	char MSG2[] = "Evening" ;
	char MSG3[] = "Everybody" ;
	
	struct sockaddr_in serv_addr;

	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]) ;
		exit(1) ;
	}

	sock = socket(PF_INET, SOCK_STREAM, 0) ;
	if( sock == -1)
		error_handling("socket() error") ;

	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = inet_addr(argv[2]) ;
	serv_addr.sin_port = htons(atoi(argv[1])) ;

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	else
		puts("Connection succeeded");

	send(sock, MSG1, strlen(MSG1), 0) ;
	send(sock, MSG2, strlen(MSG2), 0) ;
	send(sock, MSG3, strlen(MSG3), 0) ;

	for(i = 0 ; i < 3 ; i++){
		str_len = recv(sock, message, BUFSIZE, 0);
		message[str_len] = 0;
		printf(" Message %d th from server: %s\n", i, message) ;
		
		/* for problem 5 */
		close(sock) ;
	}
	
	close(sock) ;
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}
