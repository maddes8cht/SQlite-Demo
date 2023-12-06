CC = g++
COMMON_CCFLAGS = -Wall -fdiagnostics-color=always

DEBUG_CCFLAGS = $(COMMON_CCFLAGS) -g -DDEBUG
RELEASE_CCFLAGS = $(COMMON_CCFLAGS) -O2

LDFLAGS = -lsqlite3

SRC_DIR = src
DOCS_DIR = doc
BUILD_DIR_DEBUG = build/debug
BUILD_DIR_RELEASE = build/release
EXECUTABLE_DEBUG = $(BUILD_DIR_DEBUG)/sqlite-demo.exe
EXECUTABLE_RELEASE = $(BUILD_DIR_RELEASE)/sqlite-demo.exe

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS_DEBUG = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR_DEBUG)/%.o,$(SOURCES))
OBJECTS_RELEASE = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR_RELEASE)/%.o,$(SOURCES))
DEPS_DEBUG = $(OBJECTS_DEBUG:.o=.d)
DEPS_RELEASE = $(OBJECTS_RELEASE:.o=.d)

# Default value for VERSION (can be overridden from the command line)
VERSION ?= release

.PHONY: all debug release clean

all: debug release

run: all
	make run-$(VERSION)

run-debug: $(EXECUTABLE_DEBUG)
	./$(EXECUTABLE_DEBUG)

run-release: $(EXECUTABLE_RELEASE)
	./$(EXECUTABLE_RELEASE)

debug: $(EXECUTABLE_DEBUG)

release: $(EXECUTABLE_RELEASE)

$(EXECUTABLE_DEBUG): $(OBJECTS_DEBUG)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $^ $(LDFLAGS)

$(EXECUTABLE_RELEASE): $(OBJECTS_RELEASE)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $^ $(LDFLAGS)

-include $(DEPS_DEBUG)
-include $(DEPS_RELEASE)

$(BUILD_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR_DEBUG)
	$(CC) $(DEBUG_CCFLAGS) -MMD -c -o $@ $<

$(BUILD_DIR_RELEASE)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR_RELEASE)
	$(CC) $(RELEASE_CCFLAGS) -MMD -c -o $@ $<

$(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE):
	mkdir -p $@

docs:
	doxygen Doxyfile

clean:
	rm -rf $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)

clean-docs:
	rm -rf $(DOCS_DIR)

.PHONY: all debug release clean
