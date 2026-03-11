# Makefile for common tests
CXX = g++
CXXFLAGS = -std=c++17 -I../../ -Iexternal -g

COMMON_DIR = ../..
LODEPNG_SOURCES = $(COMMON_DIR)/common/parsing/lodepng/lodepng.cpp
PNG_PARSER_SOURCES = $(COMMON_DIR)/common/parsing/png_parser.cpp
LOG_SOURCES = $(COMMON_DIR)/common/icd/log_message.cpp

TEST_SOURCES = test_main.cpp \
               ../icd/tests/test_image_message.cpp \
               ../parsing/tests/test_png_parser.cpp

SOURCES = $(TEST_SOURCES) $(PNG_PARSER_SOURCES) $(LODEPNG_SOURCES) $(LOG_SOURCES)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = common_tests$(EXT)

ifeq ($(OS),Windows_NT)
    LIBS = -lws2_32
    RM = del /Q /F
    EXT = .exe
else
    LIBS = -lpthread
    RM = rm -f
    EXT =
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

clean:
	$(RM) $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

.PHONY: all clean run
