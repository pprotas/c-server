CC = clang
CFLAGS = -Iinclude -g
OBJDIR = obj
BINDIR = bin
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/c-server

$(BINDIR)/c-server: $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
