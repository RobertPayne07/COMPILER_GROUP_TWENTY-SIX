CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(filter-out $(SRCDIR)/main.cpp, $(SOURCES)))

SCANNER_SRCS = src/Token.cpp src/SourceBuffer.cpp src/ErrorHandler.cpp src/CommentFilter.cpp src/IdentifierScanner.cpp src/LiteralScanner.cpp src/OperatorScanner.cpp src/Scanner.cpp
SCANNER_OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SCANNER_SRCS))

all: compiler test_scanner test_parser

compiler: $(BUILDDIR)/main.o $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^
	@cp $(BINDIR)/compiler $(BINDIR)/scanner 2>/dev/null || true
	@cp $(BINDIR)/compiler $(BINDIR)/parser 2>/dev/null || true

test_scanner: $(BUILDDIR)/test_scanner.o $(SCANNER_OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^

test_parser: $(BUILDDIR)/test_parser.o $(OBJECTS)
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

clean:
	rm -rf $(BUILDDIR) $(BINDIR) compiler parser scanner test_scanner test_parser *.exe

.PHONY: all clean
