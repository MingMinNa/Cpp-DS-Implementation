GCC := g++
CFLAGS := -Wall -O2 -std=c++23

SRC_DIR := src
BUILD_DIR := build
MAIN := main
TARGET := main

SRCS = $(MAIN).cpp \
    $(SRC_DIR)/element.cpp \
    $(wildcard $(SRC_DIR)/**/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(GCC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(GCC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)