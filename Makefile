CXX=clang++
CXXFLAGS=--std=c++20 -Wall -Wextra -Wpedantic -Werror

BUILD=build
INCLUDE=include
LIB=lib
SRC=src
TEST=test

BIN=$(BUILD)/bin

SOURCES=$(shell find $(SRC) -name '*.cpp')
OBJECTS=$(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SOURCES))

TEST_SOURCES=$(wildcard $(TEST)/*.cpp)
TEST_OBJECTS=$(patsubst $(TEST)/%.cpp,$(BUILD)/%.o,$(TEST_SOURCES))
TEST_BINARIES=$(patsubst $(TEST)/%.cpp,$(BIN)/%,$(TEST_SOURCES))

$(LIB)/libppp.a: $(OBJECTS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

$(BIN)/%: $(TEST)/%.cpp $(LIB)/libppp.a
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) $< -L$(LIB) -lppp -o $@

.PHONY: test
test: $(TEST_BINARIES)
	@for test in $^; do \
		$$test; \
	done

.PHONY: clean
clean:
	rm -rf $(BUILD) $(LIB)

.PHONY: info
info:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "TEST_SOURCES: $(TEST_SOURCES)"
	@echo "TEST_OBJECTS: $(TEST_OBJECTS)"
	@echo "TEST_BINARIES: $(TEST_BINARIES)"
