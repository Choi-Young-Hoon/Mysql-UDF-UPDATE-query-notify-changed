FILES = $(wildcard *.cpp)
OBJECTS = $(subst .cpp,.o,$(FILES))

BUILD_DIR_LIST = ./mysql_plugin

BUILD_DIR = $(shell pwd)

export BUILD_DIR 

CPP = g++
CPPFLAGS = -std=c++0x 
TARGET = start
MYSQL_LIB_FLAGS = $(shell mysql_config --include)

export CPP
export CPPFLAGS
export MYSQL_LIB_FLAGS

all:
	@for dir in $(BUILD_DIR_LIST); do \
		$(MAKE) -C $$dir all; \
	done
	$(CPP) -o $(TARGET) -I$(BUILD_DIR) $(CPPFLAGS) $(FILES)
clean:
	@for dir in $(BUILD_DIR_LIST); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf $(TARGET) $(OBJECTS) 
install:
	@for dir in $(BUILD_DIR_LIST); do \
		$(MAKE) -C $$dir install; \
	done
