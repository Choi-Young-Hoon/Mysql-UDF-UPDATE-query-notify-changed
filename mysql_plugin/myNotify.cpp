#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "public_setting/event_value.hpp"

extern "C" {
	my_bool myNotifyChanged_init(UDF_INIT * initid, UDF_ARGS * args, char * message);
	long long myNotifyChanged(UDF_INIT * initid, UDF_ARGS * args, char * is_null, char * error);
}

static int sockfd = 0;

my_bool myNotifyChanged_init(UDF_INIT * initid, UDF_ARGS *args, char *message){
	struct sockaddr_in saddr, conaddr;
	if(args->arg_count != 1){
		strcpy(message, "Err 1");
		return -1;
	}

	if(args->arg_type[0] != INT_RESULT){
		strcpy(message, "Err 2");
		return -1;
	}

	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd == -1)
		return -1;

	////???????????????
	// @TODO sin_port 에 0이 아닌 다른값이 들어가면 bind Error 
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = PF_INET;
	saddr.sin_port = htons(0);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) == -1){
		strcpy(message, "bind() failed");
		return -1;
	}

	memset(&conaddr, 0x00, sizeof(conaddr));
	conaddr.sin_family = PF_INET;
	conaddr.sin_port = htons(40000);
	conaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(sockfd, (struct sockaddr*)&conaddr, sizeof(conaddr)) == -1){
		strcpy(message, "Connect Failed");
		return -1;
	}

	return 0;
}

void myNotifyChanged_deinit(UDF_INIT * inifid){
	if(sockfd != 0){
		close(sockfd);
	}
}

long long myNotifyChanged(UDF_INIT * initid, UDF_ARGS * args, 
				char *is_null, char * error){
	char data[512] = {0, };
	sprintf(data, "%d",  *((int*)args->args[0]));
	send(sockfd, data, strlen(data), 0);
	return 0;
}
