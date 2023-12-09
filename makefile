CC = g++
COMMON_CCFLAGS = -Wall -fdiagnostics-color=always

DEBUG_CCFLAGS = $(COMMON_CCFLAGS) -g -DDEBUG
RELEASE_CCFLAGS = $(COMMON_CCFLAGS) -O2

LDFLAGS = -lsqlite3

SRC_DIR = src
TEST_DIR = test
DOCS_DIR = doc
BUILD_DIR_DEBUG = build/debug
BUILD_DIR_RELEASE = build/release
BUILD_DIR_TEST_DEBUG = build/test-debug
BUILD_DIR_TEST_RELEASE = build/test-release
EXECUTABLE_DEBUG = $(BUILD_DIR_DEBUG)/sqlite-demo.exe
EXECUTABLE_RELEASE = $(BUILD_DIR_RELEASE)/sqlite-demo.exe
TEST_EXECUTABLES_DEBUG = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR_TEST_DEBUG)/%.exe,$(wildcard $(TEST_DIR)/*.cpp))
TEST_EXECUTABLES_RELEASE = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR_TEST_RELEASE)/%.exe,$(wildcard $(TEST_DIR)/*.cpp))

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS_DEBUG = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR_DEBUG)/%.o,$(SOURCES))
OBJECTS_RELEASE = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR_RELEASE)/%.o,$(SOURCES))
DEPS_DEBUG = $(OBJECTS_DEBUG:.o=.d)
DEPS_RELEASE = $(OBJECTS_RELEASE:.o=.d)

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS_DEBUG = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR_TEST_DEBUG)/%.o,$(TEST_SOURCES)) $(OBJECTS_DEBUG)
TEST_OBJECTS_RELEASE = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR_TEST_RELEASE)/%.o,$(TEST_SOURCES)) $(OBJECTS_RELEASE)

# Default value for VERSION (can be overridden from the command line)
VERSION ?= release

.PHONY: all debug release test clean clean-docs docs purge distclean

all: debug release test

run: all
	make run-$(VERSION)

run-debug: $(EXECUTABLE_DEBUG)
	./$(EXECUTABLE_DEBUG)

run-release: $(EXECUTABLE_RELEASE)
	./$(EXECUTABLE_RELEASE)

debug: $(EXECUTABLE_DEBUG)

release: $(EXECUTABLE_RELEASE)


test: test-$(VERSION)

test-debug: $(TEST_EXECUTABLES_DEBUG)
	for test in $(TEST_EXECUTABLES_DEBUG); do $$test; done

test-release: $(TEST_EXECUTABLES_RELEASE)
	for test in $(TEST_EXECUTABLES_RELEASE); do $$test; done

$(BUILD_DIR_TEST_DEBUG)/%.exe: TEST_BUILD = 1
$(BUILD_DIR_TEST_DEBUG)/%.exe: $(TEST_DIR)/%.cpp $(OBJECTS_DEBUG) | $(BUILD_DIR_TEST_DEBUG)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $< $(OBJECTS_DEBUG) $(LDFLAGS)

$(BUILD_DIR_TEST_RELEASE)/%.exe: TEST_BUILD = 1
$(BUILD_DIR_TEST_RELEASE)/%.exe: $(TEST_DIR)/%.cpp $(OBJECTS_RELEASE) | $(BUILD_DIR_TEST_RELEASE)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $< $(OBJECTS_RELEASE) $(LDFLAGS)


$(EXECUTABLE_DEBUG): $(OBJECTS_DEBUG)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $^ $(LDFLAGS)

$(EXECUTABLE_RELEASE): $(OBJECTS_RELEASE)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR_TEST_DEBUG)/%.exe: $(TEST_DIR)/%.cpp $(OBJECTS_DEBUG) | $(BUILD_DIR_TEST_DEBUG)
	$(CC) $(DEBUG_CCFLAGS) -o $@ $< $(OBJECTS_DEBUG) $(LDFLAGS)

$(BUILD_DIR_TEST_RELEASE)/%.exe: $(TEST_DIR)/%.cpp $(OBJECTS_RELEASE) | $(BUILD_DIR_TEST_RELEASE)
	$(CC) $(RELEASE_CCFLAGS) -o $@ $< $(OBJECTS_RELEASE) $(LDFLAGS)

$(BUILD_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR_DEBUG)
	$(CC) $(DEBUG_CCFLAGS) -MMD -c -o $@ $<

$(BUILD_DIR_RELEASE)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR_RELEASE)
	$(CC) $(RELEASE_CCFLAGS) -MMD -c -o $@ $<

$(BUILD_DIR_TEST_DEBUG)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR_TEST_DEBUG)
	$(CC) $(DEBUG_CCFLAGS) -MMD -c -o $@ $<

$(BUILD_DIR_TEST_RELEASE)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR_TEST_RELEASE)
	$(CC) $(RELEASE_CCFLAGS) -MMD -c -o $@ $<

$(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE) $(BUILD_DIR_TEST_DEBUG) $(BUILD_DIR_TEST_RELEASE):
	mkdir -p $@

docs:
	doxygen Doxyfile

clean-all: clean-code clean-docs

clean-code:
	rm -rf $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE) $(BUILD_DIR_TEST_DEBUG) $(BUILD_DIR_TEST_RELEASE)

clean-docs:
	rm -rf $(DOCS_DIR)

clean-purge:
	rm -rf $(BUILD_DIR_DEBUG)/*.o $(BUILD_DIR_RELEASE)/*.o $(BUILD_DIR_TEST_DEBUG)/*.o $(BUILD_DIR_TEST_RELEASE)/*.o
	rm -rf $(BUILD_DIR_DEBUG)/*.d $(BUILD_DIR_RELEASE)/*.d $(BUILD_DIR_TEST_DEBUG)/*.d $(BUILD_DIR_TEST_RELEASE)/*.d

