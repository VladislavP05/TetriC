
# File paths
SRC=src src/common
INC=src/include
BIN=bin

# Program name and path
PROGRAM=$(BIN)/Tetric

# Compiler
CC=gcc

# Build flags
OPTLVL=-O2
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Wextra -g $(OPTLVL) -I$(INC) $(DEPFLAGS)
SDLFLAGS=`pkg-config --cflags --libs sdl2`

# Iterate through all source directories and add all .c files
CFILES=$(foreach DIR,$(SRC),$(wildcard $(DIR)/*.c))

# Creates .o dependencies from .c files
OBJECTS=$(patsubst %.c,$(BIN)/%.o,$(notdir $(CFILES)))

# Creates .d dependencies from .c files (headers)
DEPFILES=$(patsubst %.c,$(BIN)/%.d,$(notdir $(CFILES)))

all: linux windows

# Rule for building linux binary
linux: $(OBJECTS)
	$(CC) -o $(PROGRAM) $^ $(SDLFLAGS)

# Rule for building windows binary
windows: $(OBJECTS)
	$(CC) -o $(PROGRAM).exe $^ $(SDLFLAGS)

# Rule for building object files from source files in src
$(BIN)/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule for building object files from source files in src/common
$(BIN)/%.o: src/common/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule for cleaning the bin directory
.PHONY: clean
clean:
	rm -rf $(PROGRAM).bin $(PROGRAM).exe $(BIN)/*.log $(OBJECTS) $(DEPFILES)

# Including the header dependencies
-include $(DEPFILES)
