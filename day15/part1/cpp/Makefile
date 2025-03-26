# Directories
SRC_DIR := .
GRAPHICS_DIR := $(SRC_DIR)/../../../utils/graphics
BIN_DIR := .
LIB_DIR := lib
UTILS_DIR := $(SRC_DIR)/../../../utils
GLM_DIR := $(UTILS_DIR)/glm
STB_DIR := $(UTILS_DIR)/stb
GLAD_DIR := $(UTILS_DIR)/glad
GLAD_KHR_DIR := $(UTILS_DIR)/glad/KHR
INCLUDE_DIR := -I$(UTILS_DIR) -I$(GRAPHICS_DIR) -I$(GLM_DIR) -I$(STB_DIR) -I$(GLAD_DIR) -I$(GLAD_KHR_DIR)

# Compiler and linker settings
CXX := g++
CC := gcc
CXXFLAGS := -g -std=c++17 -Wall -Wextra $(INCLUDE_DIR)
CFLAGS := -g -Wall -Wextra $(INCLUDE_DIR)
LDFLAGS := -lGL -lGLEW -lglfw $(LIB_DIR)/graphics.a
AR := ar rcs

# Target executable and archive
TARGET := $(BIN_DIR)/main
LIB := $(LIB_DIR)/graphics.a

SRCS := $(wildcard $(GRAPHICS_DIR)/*.cpp)
OBJS := $(patsubst $(GRAPHICS_DIR)/%.cpp,$(LIB_DIR)/utils/graphics/%.o,$(SRCS)) $(LIB_DIR)/glad.o

# Dependency files
DEPS := $(OBJS:.o=.d)

# Default target
all: $(LIB) $(TARGET)

# Link the final executable
$(TARGET): $(LIB) main_glfw.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Create the archive
$(LIB): $(OBJS) | $(LIB_DIR)
	$(AR) $@ $^

# Compile C++ source files into object files
$(LIB_DIR)/utils/graphics/%.o: $(GRAPHICS_DIR)/%.cpp | $(LIB_DIR)/utils/graphics
	@echo "Compiling $< to $@"
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Compile glad.c into object file using gcc
$(LIB_DIR)/glad.o: $(GLAD_DIR)/glad.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Ensure required directories exist
$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIB_DIR)/utils/graphics:
	mkdir -p $(LIB_DIR)/utils/graphics

# Clean up build files
clean:
	rm -rf $(LIB_DIR)
	rm -f $(TARGET)

# Include dependency files if they exist
-include $(DEPS)

# Phony targets
.PHONY: all clean run

