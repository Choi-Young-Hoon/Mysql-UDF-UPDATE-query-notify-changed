#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <mysql.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "public_setting/event_value.hpp"

static int sockfd = 0;

extern "C" {
	my_bool myNotifyChanged_init(UDF_INIT * initid, UDF_ARGS * args, char * message);
	long long myNotifyChanged(UDF_INIT * initid, UDF_ARGS * args, char * is_null, char * error);
}

//struct sockaddr_in 초기화.
void setSockAddr(struct sockaddr_in * addr, int port, const char * ip_address){
	memset(addr, 0x00, sizeof(struct sockaddr_in));
	addr->sin_family = PF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = inet_addr(ip_address);
}

//Connect 함수.
int connectToServer(char * message){
	struct sockaddr_in addr;
	
	setSockAddr(&addr, SERVER_PORT, "127.0.0.1");
	if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		strcpy(message, "bind() failed");
		return -1;
	}

	if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		strcpy(message, "Connect Failed");
		return -1;
	}
	return 0;
}

//Mysql myNotifyChanged() 호출시 초기화 함수로 실행.
my_bool myNotifyChanged_init(UDF_INIT * initid, UDF_ARGS *args, char *message){
	if(args->arg_count != 1){ // Args count 체크.
		strcpy(message, "Err arg count");
		return -1;
	}

	if(args->arg_type[0] != INT_RESULT){ // Args type 체크.
		strcpy(message, "Err arg type");
		return -1;
	}

	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd == -1)
		return -1;
	
	if(connectToServer(message) == -1)
		return -1;
	return 0;
}

//myNotifyChanged() 종료시 수행.
void myNotifyChanged_deinit(UDF_INIT * inifid){
	if(sockfd != 0){
		close(sockfd);
	}
}

// DoWork()
long long myNotifyChanged(UDF_INIT * initid, UDF_ARGS * args, 
					char *is_null, char * error){
	char data[512] = {0, };
	sprintf(data, "%d",  *((int*)args->args[0]));
	send(sockfd, data, strlen(data), 0);
	return 0;
}
