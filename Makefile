CXX=clang++
CXXFLAGS=--std=c++20 -Wall -Wextra -Wpedantic -Werror

BUILD=build
INCLUDE=include
LIB=lib
SRC=src
TEST=test

BIN=$(BUILD)/bin

SOURCES=$(wildcard $(SRC)/*.cpp)
OBJECTS=$(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(SOURCES))

TEST_SOURCES=$(wildcard $(TEST)/*.cpp)
TEST_OBJECTS=$(patsubst $(TEST)/%.cpp, $(BUILD)/%.o, $(TEST_SOURCES))
TEST_BINARIES=$(patsubst $(TEST)/%.cpp, $(BIN)/%, $(TEST_SOURCES))

$(LIB)/libppp.a: $(OBJECTS)
	@mkdir -p $(LIB)
	ar rcs $@ $^

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

$(BIN)/%: $(TEST)/%.cpp $(LIB)/libppp.a
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) $< -L$(LIB) -lppp -o $@

.PHONY: test
test: $(TEST_BINARIES)
	@for test in $^; do \
		$$test; \
	done

.PHONY: clean
clean:
	rm -rf $(BUILD) $(LIB)
