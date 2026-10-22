CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCDIR = src
INCDIR = include
BUILDDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(filter-out $(SRCDIR)/main.cpp, $(SOURCES)))

all: scanner test_scanner

scanner: $(BUILDDIR)/main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_scanner: $(BUILDDIR)/test_scanner.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/test_scanner.o: tests/test_scanner.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) scanner test_scanner scanner.exe test_scanner.exe

.PHONY: all clean
