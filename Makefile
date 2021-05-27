CC=gcc
BIN=detoate
SRCDIR=src
BINDIR=bin
OBJDIR=bin_int
OBJS=$(OBJDIR)/main.o
CLFAGS=-Wall -Wextra -Wpedantic -g


all:$(BINDIR)/$(BIN)

$(BINDIR)/$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*
