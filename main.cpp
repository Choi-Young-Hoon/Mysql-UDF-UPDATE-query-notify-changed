#include "notify_handler/mysql_notify_changed.hpp"
#include "public_setting/event_value.hpp"

#include <iostream>
using namespace std;

//MysqlNotifyChanged 인터페이스 구현
class MysqlEvent : public myNotify::MysqlNotifyChanged {
public:
	MysqlEvent() : myNotify::MysqlNotifyChanged(SERVER_CONFIG::SERVER_PORT) {}
	virtual ~MysqlEvent() {}

	// Interface 구현
	virtual void InsertEvent(){ cout << "Insert Event!!" << endl; }
	virtual void DeleteEvent(){ cout << "Delete Event!!" << endl; }
	virtual void UpdateEvent(){ cout << "Update Event!!" << endl; }
};

int main(void){
	MysqlEvent handler;
	handler.StartNotifyChangedEvent();
	return 0;
}
