#ifndef __NOTIFY_HANDLER_TCP_SERVER_HEADER__
#define __NOTIFY_HANDLER_TCP_SERVER_HEADER__
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstring>

namespace MyNotify {
	class TcpServer {
	public:
		TcpServer() : buffer(NULL), server_fd(0) {}
		TcpServer(int port) : server_port(port) {}
		virtual ~TcpServer(){
			if (buffer != NULL) 
				delete []buffer;
			Close();
		}

		inline bool CreateSocket(){
			server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (server_fd == -1)
				return false;
			return true;
		}

		inline bool Bind(){
			struct sockaddr_in serv_addr;
			serv_addr.sin_family = PF_INET;
			serv_addr.sin_port = htons(server_port);
			serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

			if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
				return false;
			if (listen(server_fd, 3) == -1)
				return false;
			return true;
		}

		inline int Accept(){
			socklen_t addr_size = sizeof(client_addr);
			client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
			return client_fd;
		}

		//Recv, Send
		inline int Recv(){ return recv(client_fd, buffer, buffer_size, 0); }
		inline int Send(char * buf){ return send(client_fd, buf, strlen(buf), 0); }

		inline void Close(){
			if (server_fd != 0)
				close(server_fd);
		}

		inline bool SetBufferSize(int size){
			buffer = new char[size];
			if (buffer == NULL)
				return false;
			buffer_size = size;
			return true;
		}
		// Getter, Setter 
		inline void SetPort(int port){ server_port = port; }
		inline int GetPort(){ return server_port; }
		inline char * GetBuffer(){ return buffer; }
	private:
		//Server
		int server_fd;
		int server_port;

		//Client
		int client_fd; 
		struct sockaddr_in client_addr;

		//Recv buffer
		char * buffer;
		int buffer_size;
	};
};
#endif
