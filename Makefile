# macOS shortcut matching A3_check's clang++ / pkg-config / OpenGL setup.
CXX = clang++
CPPFLAGS += -Iinclude $(shell pkg-config --cflags glfw3)
CXXFLAGS += -std=c++17 -O2 -Wall -Wextra -Wpedantic
LDFLAGS += $(shell pkg-config --libs glfw3) -framework OpenGL
SOURCES := $(wildcard src/*.cpp src/scene/*.cpp)
OBJECTS := $(patsubst src/%.cpp,build/make/%.o,$(SOURCES))
TARGET := build/make/concert_stage

.PHONY: all run test smoke clean
all: $(TARGET)

$(TARGET): $(OBJECTS) $(wildcard shaders/*.vert shaders/*.frag)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	mkdir -p build/make/shaders
	cp shaders/*.vert shaders/*.frag build/make/shaders/

build/make/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

build/make/math_tests: tests/math_tests.cpp $(wildcard include/concert/*.hpp)
	mkdir -p build/make
	$(CXX) -Iinclude $(CXXFLAGS) $< -o $@

test: build/make/math_tests
	./build/make/math_tests

smoke: $(TARGET)
	./$(TARGET) --smoke-test

clean:
	rm -rf build/make

-include $(OBJECTS:.o=.d)
