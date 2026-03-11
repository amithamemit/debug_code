# Makefile for control tests
CXX = g++
CXXFLAGS = -std=c++17 -I../../ -I../../common/tests/external -g

COMMON_DIR = ../../common
LOG_SOURCES = $(COMMON_DIR)/icd/log_message.cpp
MQ_SOURCES = $(COMMON_DIR)/ipc/mq_wrapper.cpp

TEST_SOURCES = ../../common/tests/test_main.cpp test_control_utils.cpp

SOURCES = $(TEST_SOURCES) $(LOG_SOURCES) $(MQ_SOURCES)
EXECUTABLE = control_tests$(EXT)

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
