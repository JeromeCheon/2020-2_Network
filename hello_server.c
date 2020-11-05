#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      	// ssize_t read(), and write() | int close()
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <fcntl.h> 		// int open()

void error_handling(char *message);

int main(int argc, char* argv[]){
	int serv_sock ;
	int clnt_sock ;

	struct sockaddr_in serv_addr ;
	struct sockaddr_in clnt_addr ;
	socklen_t clnt_addr_size ;

	char message[] = "Hello World!" ;

	if(argc != 2){
		printf("Usage : %s <port> \n", argv[0]) ;
		exit(1) ;

	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0) ; 	// TCP socket way
	if(serv_sock == -1)
		error_handling("socket() error") ;

	memset(&serv_addr, 0, sizeof(serv_addr)) ; // for now, initialize the structure into all zeros.

	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	serv_addr.sin_port = htons(atoi(argv[1])) ;
	
	// bind() the second step
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error") ;
 	// listen() the third step
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error") ;

	clnt_addr_size = sizeof(clnt_addr) ;
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size) ;
	if(clnt_sock == -1)
		error_handling("accept() error") ;

	write(clnt_sock, message, sizeof(message)) ;
	close(clnt_sock) ;
	close(serv_sock) ;
	return 0 ;
}

void error_handling(char* message){
	fputs(message, stderr) ;
	fputc('\n', stderr) ;
	exit(1) ;
}
