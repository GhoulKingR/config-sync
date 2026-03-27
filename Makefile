CC = g++
FLAGS = -std=c++20 -DDEBUG
SOURCES = main.cpp app.cpp config.cpp logger.cpp
PROGRAM_NAME = config-sync
BUILD_DIRECTORY = build

clean:
	@if [ -d "$(BUILD_DIRECTORY)" ]; then \
		rm -rf $(BUILD_DIRECTORY); \
	fi

build: clean
	mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

build-debug: clean
	mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) -g -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

