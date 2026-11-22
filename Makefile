CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(filter-out $(SRCDIR)/main.cpp, $(SOURCES)))

all: compiler test_scanner test_parser test_ir

compiler: $(BUILDDIR)/main.o $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^
	@cp $(BINDIR)/compiler $(BINDIR)/scanner 2>/dev/null || true
	@cp $(BINDIR)/compiler $(BINDIR)/parser 2>/dev/null || true
	@cp $(BINDIR)/compiler $(BINDIR)/ir 2>/dev/null || true

test_scanner: $(BUILDDIR)/test_scanner.o $(filter-out $(BUILDDIR)/AST% $(BUILDDIR)/Parser% $(BUILDDIR)/Symbol% $(BUILDDIR)/Type% $(BUILDDIR)/Semantic% $(BUILDDIR)/IR%, $(OBJECTS))
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^

test_parser: $(BUILDDIR)/test_parser.o $(filter-out $(BUILDDIR)/Symbol% $(BUILDDIR)/Type% $(BUILDDIR)/Semantic% $(BUILDDIR)/IR%, $(OBJECTS))
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^

test_ir: $(BUILDDIR)/test_ir.o $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/test_scanner.o: tests/test_scanner.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/test_parser.o: tests/test_parser.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/test_ir.o: tests/test_ir.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR) compiler parser scanner ir test_scanner test_parser test_ir *.exe

.PHONY: all clean
