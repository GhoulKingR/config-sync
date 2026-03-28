CC = g++
FLAGS = -std=c++20 -DDEBUG -Wall -Wextra
SOURCES = main.cpp app.cpp config.cpp logger.cpp shell.cpp
PROGRAM_NAME = config-sync
BUILD_DIRECTORY = build
INCLUDES = -Itomlplusplus/include

clean:
	@if [ -d "$(BUILD_DIRECTORY)" ]; then \
		rm -rf $(BUILD_DIRECTORY); \
	fi

build: clean
	mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) $(INCLUDES) -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

build-debug: clean
	mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) $(INCLUDES) -g -fsanitize=address -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

