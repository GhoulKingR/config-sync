CC = g++
FLAGS = -std=c++20 -Wall -Wextra
SOURCES = main.cpp app.cpp config.cpp logger.cpp shell.cpp
PROGRAM_NAME = config-sync
BUILD_DIRECTORY = build
BIN_DIRECTORY = /usr/local/bin
INCLUDES = -Itomlplusplus/include
DEBUG_FLAGS = -DDEBUG -g -fsanitize=address -fno-omit-frame-pointer

ifeq ($(OS),Windows_NT)
	$(error Windows not supported)
endif

$(info Checking for dependencies)

# check for git command
GIT_STATUS = $(shell which git > /dev/null; echo $?)
ifeq ($(GIT_STATUS),1)
	$(error Git not installed!)
else
	$(info Git installed)
endif

# check for zip command
ZIP_STATUS = $(shell which zip > /dev/null; echo $?)
ifeq ($(ZIP_STATUS),1)
	$(error Zip command not installed!)
else
	$(info Zip installed)
endif

# check for unzip command
UNZIP_STATUS = $(shell which unzip > /dev/null; echo $?)
ifeq ($(UNZIP_STATUS),1)
	$(error Unzip command not installed!)
else
	$(info UnZip installed)
endif

clean:
	@if [ -d "$(BUILD_DIRECTORY)" ]; then \
		rm -rf $(BUILD_DIRECTORY); \
	fi

build: clean
	@mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) $(INCLUDES) -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

build-debug: clean
	@mkdir $(BUILD_DIRECTORY)
	$(CC) $(FLAGS) $(SOURCES) $(INCLUDES) $(DEBUG_FLAGS) -o $(BUILD_DIRECTORY)/$(PROGRAM_NAME)

install: build
	@sudo cp $(BUILD_DIRECTORY)/$(PROGRAM_NAME) $(BIN_DIRECTORY)

uninstall:
	@if [ -s "$(BIN_DIRECTORY)/$(PROGRAM_NAME)" ]; then \
		sudo rm "$(BIN_DIRECTORY)/$(PROGRAM_NAME)"; \
	else \
		echo "$(PROGRAM_NAME) not installed; nothing happened"; \
	fi


