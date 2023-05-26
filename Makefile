ifeq (0, 1)
	all: main
	
	CC = clang
	override CFLAGS += -g -Wno-everything -pthread -lm
	
	# Path to SQLite folder
	SQLITE_FOLDER = path/to/sqlite3
	
	# Find C files in the current folder and the additional folder
	SRCS := $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
	SRCS += $(shell find $(SQLITE_FOLDER) -name '*.c' -print)
	
	# Find header files in the current folder and the additional folder
	HEADERS := $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)
	HEADERS += $(shell find $(SQLITE_FOLDER) -name '*.h' -print)
	
	main: $(SRCS) $(HEADERS)
		$(CC) $(CFLAGS) $(SRCS) -o "$@"
	
	main-debug: $(SRCS) $(HEADERS)
		$(CC) $(CFLAGS) -O0 $(SRCS) -o "$@"
	
	clean:
		rm -f main main-debug

endif



CC = gcc
override CFLAGS = -g -Wno-everything -pthread -lm -Wall -Wextra

# Path to SQLite folder
SQLITE_FOLDER = ./sqlite3/

# making list of .c files
SRCS := $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)

SRCS += $(shell find $(SQLITE_FOLDER) -name '*.c' -print)



# name of output executable
TARGET = main

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGET)