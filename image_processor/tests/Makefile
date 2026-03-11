# Makefile for image_processor tests
CXX = g++
CXXFLAGS = -std=c++17 -I../../ -I../../common/tests/external -g

IMAGE_OPS_SOURCES = ../processing/image_ops.cpp
TEST_SOURCES = ../../common/tests/test_main.cpp test_image_ops.cpp

SOURCES = $(TEST_SOURCES) $(IMAGE_OPS_SOURCES)
EXECUTABLE = image_processor_tests$(EXT)

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
