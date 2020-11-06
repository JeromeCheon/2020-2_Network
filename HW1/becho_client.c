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

	
	char MSG1[] = "Good" ;
	char MSG2[] = "Evening " ;
	char MSG3[] = "Everybody!" ;

	struct sockaddr_in serv_adr, from_adr ;

	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]) ;
		exit(1) ;
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0) ;
	if( sock == -1)
		error_handling("socket() error") ;

	memset(&serv_adr, 0, sizeof(serv_adr)) ;
	serv_adr.sin_family = AF_INET ;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]) ;
	serv_adr.sin_port = htons(atoi(argv[1])) ;

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

	/*	while(1){
		fputs("Insert message(q to quit): ", stdout) ;
		fgets(message, sizeof(message), stdin) ;
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break ;

		sendto(sock, message, strlen(message), 0,
				(struct sockaddr*)&serv_adr, sizeof(serv_adr)) ;
		adr_sz = sizeof(from_adr) ;
		str_len = recvfrom(sock, message, BUF_SIZE, 0,
				(struct sockaddr*)&from_adr, &adr_sz) ;

		message[str_len] = 0 ;
		printf("Message from server: %s", message) ;
	}*/
	close(sock) ;
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc("\n", stderr) ;
	exit(1) ;
}
