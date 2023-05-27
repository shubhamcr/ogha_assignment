CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -Wextra -pthread

SRC_DIR = .
SRC_HEADERS = $(SRC_DIR)/*.h
BUILD_DIR = ./build

create_build_dir :
	mkdir -p $(BUILD_DIR)

question2 : $(SRC_DIR)/question2.cpp
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) $(SRC_DIR)/question2.cpp \
			-o $(BUILD_DIR)/question2

question3 : $(SRC_DIR)/question3.cpp
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) $(SRC_DIR)/question3.cpp \
			-o $(BUILD_DIR)/question3

.PHONY: all
all : create_build_dir question2 question3

.PHONY: clean
clean : ## Cleans the build directory
	rm -rf $(BUILD_DIR)/*
