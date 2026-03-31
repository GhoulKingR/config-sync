CC = g++
FLAGS = -std=c++20 -Wall -Wextra
SOURCES = main.cpp app.cpp config.cpp logger.cpp shell.cpp
PROGRAM_NAME = config-sync
BUILD_DIRECTORY = build
BIN_DIRECTORY = /usr/local/bin
INCLUDES = -Itomlplusplus/include
DEBUG_FLAGS = -DDEBUG -g -fsanitize=address -fno-omit-frame-pointer

clean:
	@if [ -d "$(BUILD_DIRECTORY)" ]; then \
		rm -rf $(BUILD_DIRECTORY); \
	fi

build: clean
	mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) $(INCLUDES) -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

build-debug: clean
	mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) $(INCLUDES) $(DEBUG_FLAGS) -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

install: build
	sudo cp $(BUILD_DIRECTORY)/$(PROGRAM_NAME) $(BIN_DIRECTORY)

uninstall:
	@if [ -s "$(BIN_DIRECTORY)/$(PROGRAM_NAME)" ]; then \
		sudo rm "$(BIN_DIRECTORY)/$(PROGRAM_NAME)"; \
	else \
		echo "$(PROGRAM_NAME) not installed; nothing happened"; \
	fi


