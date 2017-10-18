FILES = $(wildcard *.cpp)
OBJECTS = $(subst .cpp,.o,$(FILES))
SO_TARGET = myNotify.so 
all:
	$(CPP) -c -Wall -fpic $(FILES) -I$(BUILD_DIR) $(MYSQL_LIB_FLAGS)
	$(CPP) -shared -o $(SO_TARGET) $(OBJECTS)
clean:
	rm -rf $(OBJECTS) $(SO_TARGET)
install:
	sudo cp $(SO_TARGET) /usr/lib/mysql/plugin/

