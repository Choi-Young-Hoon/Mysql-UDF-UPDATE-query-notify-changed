#include "notify_handler/mysql_notify_changed.hpp"
#include "public_setting/event_value.hpp"

#include <iostream>
using namespace std;

class MysqlEvent : public myNotify::MysqlNotifyChanged {
public:
	MysqlEvent() : myNotify::MysqlNotifyChanged(40000) {}
	virtual ~MysqlEvent() {}

	virtual void InsertEvent(){
		cout << "Insert Event!!" << endl;		
	}

	virtual void DeleteEvent(){
		cout << "Delete Event!!" << endl;
	}

	virtual void UpdateEvent(){
		cout << "Update Event!!" << endl;
	}
};

int main(void){
	MysqlEvent handler;
	handler.StartNotifyChangedEvent();
	return 0;
}
