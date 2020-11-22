#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30

void error_handling(char* message) ;

// make sure there is no connect() function unlike tcp
int main(int argc, char* argv[]){
	int sock ;
	char message[BUFSIZE] ;
	int str_len, i ;
	socklen_t addr_size ;
	
	char MSG1[] = "Good" ;
	char MSG2[] = "Evening" ;
	char MSG3[] = "Everybody!" ;
	// char MSG3[] = "" ; // string with nothing

	struct sockaddr_in serv_addr, from_addr ;

	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]) ;
		exit(1) ;
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0) ;
	if( sock == -1)
		error_handling("socket() error") ;

	memset(&serv_addr, 0, sizeof(serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = inet_addr(argv[2]) ;
	serv_addr.sin_port = htons(atoi(argv[1])) ;

	sendto(sock, MSG1, strlen(MSG1), 0,
			(struct sockaddr*)&serv_addr, sizeof(serv_addr)) ;
	sendto(sock, MSG2, strlen(MSG2), 0,
			(struct sockaddr*)&serv_addr, sizeof(serv_addr)) ;
	sendto(sock, MSG3, strlen(MSG3), 0,
			(struct sockaddr*)&serv_addr, sizeof(serv_addr)) ;

	for(i = 0 ; i < 3 ; i++){
		addr_size = sizeof(from_addr) ;
		str_len = recvfrom(sock, message, BUFSIZE, 0,
				(struct sockaddr*)&from_addr, &addr_size) ;
		message[str_len] = 0 ;
		printf(" Message %d th from server: %s", i, message) ;
	}

	close(sock) ;
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}
