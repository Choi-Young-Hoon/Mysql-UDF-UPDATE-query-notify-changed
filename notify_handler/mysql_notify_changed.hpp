#ifndef __NOTIFY_HANDLER_MYSQL_NOTIFY_CHANGED_HEADER__
#define __NOTIFY_HANDLER_MYSQL_NOTIFY_CHANGED_HEADER__
#include "tcp_server.hpp"
#include "public_setting/event_value.hpp"

#include <cstdlib>

namespace MyNotify {
	class MysqlNotifyChanged : public TcpServer{
	public:
		MysqlNotifyChanged() : TcpServer()
		{}
		MysqlNotifyChanged(int port) : TcpServer(port)
		{}
		virtual ~MysqlNotifyChanged()
		{}

		//Event Loop 시작.
		bool StartNotifyChangedEvent(){
			if (!ServerSetting())
				return false;
			while (true) {
				if (TcpServer::Accept() == -1) {
					TcpServer::Close();
					return false;
				}
				TcpServer::Recv();
				Run(TcpServer::GetBuffer());
			}
			return true;
		}

	private:
		//TcpServer 객체 초기화.
		bool ServerSetting(){
			if (!TcpServer::SetBufferSize(512)) return false;
			if (!TcpServer::CreateSocket()) return false;
			if (!TcpServer::Bind()) return false;
			return true;
		}
		
		//각 쿼리에 대한 이벤트 호출
		void Run(char * buffer){
			switch (atoi(buffer)) {
			case QUERY_EVENT_NOTIFY::INSERT:
				InsertEvent();
				break;
			case QUERY_EVENT_NOTIFY::DELETE:
				DeleteEvent();
				break;
			case QUERY_EVENT_NOTIFY::UPDATE:
				UpdateEvent();
				break;
			}
		}

	protected: // Interface
		virtual void InsertEvent() = 0;
		virtual void DeleteEvent() = 0;
		virtual void UpdateEvent() = 0;
	};
};
#endif
