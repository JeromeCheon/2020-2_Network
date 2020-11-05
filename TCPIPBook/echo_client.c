#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

/**
  Make sure that TCP doesn't have the boundaries of data. 
  This client can send more than 2 strng messages to server at the same time. 
  But at this situation, server would respond both as well to client, and it can cause some problem not to get an expected result.
  Even though this program will be able to be looked like working very well. But still there are some potential error probabilities to be occurred.
  If there is a long length data, a server could decide whether they devide the data as 2 packets
  */

void error_handling(char* message) ;

int main(int argc, char* argv[]){
	int sock ;
	char message[BUF_SIZE] ;
	int str_len ;

	struct sockaddr_in serv_adr ;

	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]) ;
		exit(1) ;
	}

	sock = socket(PF_INET, SOCK_STREAM, 0) ;

	if(sock == -1)
		error_handling("socket() error" ) ;

	memset(&serv_adr, 0, sizeof(serv_adr)) ;
	serv_adr.sin_family = AF_INET ;
	serv_adr.sin_addr.s_addr =inet_addr(argv[1]) ;
	serv_adr.sin_port = htons(atoi(argv[2])) ;
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error") ;
	else
		puts("Connected............") ;

	while(1){
		fputs("Input message(Q to quit): ", stdout) ;
		fgets(message, BUF_SIZE, stdin) ;

		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break ;

		/*write(sock, message, strlen(message)) ;
		str_len = read(sock, message, BUF_SIZE-1) ;*/
		
		// this sniffet is for getting as much data as a server sends
		str_len = write(sock, message, strlen(message)) ; 

		recv_len = 0 ;
		while(recv_len < str_len){
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE-1) ;
			if(recv_cnt == -1)
				error_handling("read() error.") ;
			recv_len += recv_cnt ;
		}
		message[str_len] = 0 ;
		printf("Message from server: %s", message) ;
	}
	
	close(sock) ;
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}

// TCP client action procedure : 
// socket() -> connect() -> read()/write() -> close()
// Then how can we supply the lack portion?? If the server send 20 bytes string into client
// and just repeat read() function 'til the end of string. 
