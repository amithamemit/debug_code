# Compiler and common flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I$(ROOT_DIR)

# Source files
MAIN_SRC = main.cpp processing/image_ops.cpp
SOURCES = $(MAIN_SRC) $(COMMON_SOURCES) $(LODEPNG_SOURCES)

# Output executable
TARGET_BASE = image_processor
TARGET = $(TARGET_BASE)$(EXT)

# Build rules
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	$(RM) $(TARGET)

.PHONY: all clean
