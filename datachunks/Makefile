# Make file for datachunks

CXX = g++

# Source dir
SRCDIR := src
BUILDDIR := build

TARGET = bin/datachunks


SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%.$(SRCEXT),$(BUILDDIR)/%.o,$(SOURCES))
CFLAGS = -g -O2 -std=c++17 -march=native -Wno-deprecated-declarations
INC := -I include

OS := $(shell uname)

ifeq ($(OS), Linux)
	LIB := -pthread
endif
ifeq ($(OS),Darwin)
	LIB := -pthread
endif

all: check_deps $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(TARGET) $(LIB)"
	$(CXX) $^ -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $< to $@" 
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Clearning..."
	@echo "rm -r $(BUILDDIR) $(TARGET)"; rm -r $(BUILDDIR) $(TARGET)

tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

check_deps:
	@echo "Checking dependencies..."
	@case `uname` in \
		Linux*) ldconfig -p | grep libssl > /dev/null || (echo "OpenSSL not found. Please install OpenSSL." && exit 1) ;; \
		Darwin*)ls -l /usr/local/lib/ | egrep -i openssl > /dev/null || (echo "OpenSSL not found. Please install OpenSSL using Homebrew." && exit 1) ;; \
		MINGW*) echo "Windows MINGW Detected!  I will make, but make sure the libraries are installed!" ;; \
		*) echo "Unsupported operating system." && exit 1 ;; \
	esac
	@echo "All dependencies are installed."

.PHONY: clean
