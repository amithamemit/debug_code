# Main Makefile for the Image Processing System

# Common paths
ROOT_DIR = $(CURDIR)
COMMON_DIR = $(ROOT_DIR)/common

# Common source files
COMMON_SOURCES = $(COMMON_DIR)/icd/log_message.cpp \
                 $(COMMON_DIR)/ipc/mq_wrapper.cpp \
                 $(COMMON_DIR)/parsing/png_parser.cpp

LODEPNG_SOURCES = $(COMMON_DIR)/parsing/lodepng/lodepng.cpp

# Export variables for sub-makefiles
export COMMON_SOURCES LODEPNG_SOURCES ROOT_DIR COMMON_DIR

# List of processes to build
SUBDIRS = format_parser image_processor output_router control
TEST_SUBDIRS = common/tests image_processor/tests output_router/tests control/tests

# OS Selection
ifeq ($(OS),Windows_NT)
    LIBS = -lws2_32
    RM = del /Q /F
    EXT = .exe
    MAKE_CMD = "$(MAKE)"
    CXXFLAGS += -D_WIN32_WINNT=0x0600
else
    LIBS = -lpthread
    RM = rm -f
    EXT =
    MAKE_CMD = $(MAKE)
endif
export LIBS RM EXT

# Default rule
all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE_CMD) -C $@

# Test targets
test: $(TEST_SUBDIRS)

$(TEST_SUBDIRS):
	$(MAKE_CMD) -C $@

run_tests: test
	$(foreach dir,$(TEST_SUBDIRS),$(MAKE_CMD) -C $(dir) run &&) @echo All Tests Passed.

clean:
	$(foreach dir,$(SUBDIRS),$(MAKE_CMD) -C $(dir) clean &&) \
	$(foreach dir,$(TEST_SUBDIRS),$(MAKE_CMD) -C $(dir) clean &&) \
	@echo Clean Complete.

.PHONY: all clean $(SUBDIRS) $(TEST_SUBDIRS) test run_tests
