# Maintainer: @zakuwarrior, Ilya Abramov, Moscow, Russian Federation.
# github.com: @Yushint
# Makefile for OpenGL compilation in Linux environment

SHELL := /bin/bash
PWD := $(shell pwd)

SRC_DIR := $(PWD)/src
OBJ_DIR := $(PWD)/obj
BIN_DIR := $(PWD)/bin

MAIN_SRC := $(SRC_DIR)/main
IMGUI_SRC := $(SRC_DIR)/imgui
GLAD_SRC := $(SRC_DIR)/glad

CXX := g++
CXXFLAGS := -I$(PWD)/headers -Wall -Wextra -O2
LDLIBS := -lGL -lglfw -lX11 -ldl -lpthread
TARGET := $(BIN_DIR)/SceneVisualizer

MAIN_SRCS := $(wildcard $(MAIN_SRC)/*.cpp)
IMGUI_SRCS := $(wildcard $(IMGUI_SRC)/*.cpp)
GLAD_SRCS := $(wildcard $(GLAD_SRC)/*.c)

MAIN_OBJS := $(patsubst $(MAIN_SRC)/%.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRCS))
IMGUI_OBJS := $(patsubst $(IMGUI_SRC)/%.cpp,$(OBJ_DIR)/%.o,$(IMGUI_SRCS))
GLAD_OBJS := $(patsubst $(GLAD_SRC)/%.c,$(OBJ_DIR)/%.o,$(GLAD_SRCS))

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: $(TARGET)
	@echo "Build completed successfully!"

$(TARGET): $(MAIN_OBJS) $(IMGUI_OBJS) $(GLAD_OBJS)
	@echo "Linking files for SceneVisualizer..."
	$(CXX) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(MAIN_SRC)/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(IMGUI_SRC)/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(GLAD_SRC)/%.c
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

clean_without_executable:
	@echo "Deleting object files..."
	rm -rf $(OBJ_DIR)

.PHONY: all clean clean_without_executable
