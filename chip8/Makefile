CC = clang
CFLAGS = -Wall -Wextra -g -O0 -Isrc $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)


SRCDIR = src
OBJDIR = obj
TARGET = chip8.out

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(wildcard $(SRCDIR)/*.h)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET)