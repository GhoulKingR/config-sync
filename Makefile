CC = g++
FLAGS = -std=c++20 -g -DDEBUG
SOURCES = main.cpp app.cpp config.cpp logger.cpp
PROGRAM_NAME = config-sync
BUILD_DIRECTORY = build

clean:
	@if [ -d "$(BUILD_DIRECTORY)" ]; then \
		rm -rf "$(BUILD_DIRECTORY)"; \
	fi

build: clean
	mkdir "$(BUILD_DIRECTORY)"
	$(CC) $(FLAGS) $(SOURCES) -o build/$(PROGRAM_NAME)

