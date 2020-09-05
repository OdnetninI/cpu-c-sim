# Compiler Args
CC := gcc
CFLAGS := -O2 -std=c89 -Wall -D_FORTIFY_SOURCE=0 -fno-stack-protector -pipe -fms-extensions -Wno-incompatible-pointer-types -g
LDFLAGS := 

# Folders
SRCDIR := src
OBJDIR := obj
BINDIR := bin

# SRC and OBJ files
SRCFILES := $(shell find ${SRCDIR}"/" -name "*.c")
OBJFILES := $(SRCFILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Binary
BIN := cpu-c-sim
BINARGS := 

# Targets

all: $(OBJDIR) $(BINDIR) $(BIN)

run: $(BIN)
	./$(BINDIR)/$(BIN) $(BINARGS)

debug: $(BIN)
	gdb --args ./$(BINDIR)/$(BIN) $(BINARGS)

memcheck: $(BIN)
	valgrind ./$(BINDIR)/$(BIN) $(BINARGS)

# Helpers

$(BIN): $(OBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINDIR)/$(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Directories

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Clean

clean:
	rm -rf $(OBJDIR) $(BINDIR)
