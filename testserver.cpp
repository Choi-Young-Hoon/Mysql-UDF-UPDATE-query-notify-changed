#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void){
	int serv_fd, client_fd;
	struct sockaddr_in serv_addr, client_addr;
	socklen_t addr_size = sizeof(client_addr);
	char buffer[512] = {0,};

	serv_addr.sin_family = PF_INET;
	serv_addr.sin_port = htons(40000);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	serv_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_fd == -1){
		printf("socket() failed\n");
		return -1;
	}
	
	if(bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
		printf("bind() failed\n");
		return -1;
	}

	if(listen(serv_fd, 5) == -1){
		printf("listen() failed\n");
		return -1;
	}

	client_fd = accept(serv_fd, (struct sockaddr *)&client_addr, &addr_size);
	while(1){
		if(client_fd == -1){
			printf("accept() error\n");
			continue;
		}
		printf("Connect Client\n");

		recv(client_fd, buffer, 512, 0);
		printf("[C] : %s\n", buffer);
	}
	close(client_fd);
	close(serv_fd);
	return 0;
}
