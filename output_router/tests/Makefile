# Makefile for output_router tests
CXX = g++
CXXFLAGS = -std=c++17 -I../../ -I../../common/tests/external -g

COMMON_DIR = ../../common
LOG_SOURCES = $(COMMON_DIR)/icd/log_message.cpp
MQ_SOURCES = $(COMMON_DIR)/ipc/mq_wrapper.cpp
PNG_PARSER_SOURCES = $(COMMON_DIR)/parsing/png_parser.cpp
LODEPNG_SOURCES = $(COMMON_DIR)/parsing/lodepng/lodepng.cpp

TEST_SOURCES = ../../common/tests/test_main.cpp test_router_utils.cpp

SOURCES = $(TEST_SOURCES) $(LOG_SOURCES) $(MQ_SOURCES) $(PNG_PARSER_SOURCES) $(LODEPNG_SOURCES)
EXECUTABLE = output_router_tests$(EXT)

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
