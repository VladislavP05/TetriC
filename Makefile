
# File paths
SRC=src src/common
INC=src/include
BIN=bin

# Program name and path
PROGRAM=$(BIN)/Tetric

# Compiler
CC=gcc

# Build flags
OPTLVL=-Og
DEPFLAGS=-MP -MD
CFLAGS= -Wall -Wextra --static -c -g $(OPTLVL) -I$(INC) $(DEPFLAGS)
SDLFLAGS=`pkg-config --cflags --libs sdl2`

# Iterate through all source directories and add all .c files
CFILES=$(foreach DIR,$(SRC),$(wildcard $(DIR)/*.c))

# Creates .o dependencies from .c files
OBJECTS=$(patsubst %.c,$(BIN)/%.o,$(notdir $(CFILES)))

# Creates .d dependencies from .c files (headers)
DEPFILES=$(patsubst %.c,$(BIN)/%.d,$(notdir $(CFILES)))

all: linux clean

# Rule for building linux binary
linux: $(OBJECTS)
	$(CC) -o $(PROGRAM) $^ $(SDLFLAGS)

# Rule for building object files from source files in src
$(BIN)/%.o: src/%.c
	$(CC) -o $@ $< $(CFLAGS)

# Rule for building object files from source files in src/common
$(BIN)/%.o: src/common/%.c
	$(CC) -o $@ $< $(CFLAGS)

# Rule for cleaning the bin directory
.PHONY: clean
clean:
	rm -rf $(BIN)/*.log $(OBJECTS) $(DEPFILES)

# Including the header dependencies
-include $(DEPFILES)
