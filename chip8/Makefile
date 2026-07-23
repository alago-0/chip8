CC = clang
CFLAGS = -Wall -Wextra -g -O0 -Isrc $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)


SRCDIR = src
TESTDIR = tests
OBJDIR = obj
TARGET = chip8.out


SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(wildcard $(SRCDIR)/*.h)

TEST_SRCS = $(wildcard $(TESTDIR)/*.c)
TEST_BIN = tests.out
APP_OBJS = $(filter-out $(OBJDIR)/main.o, $(OBJS))
CRIT_CFLAGS = $(shell pkg-config --cflags criterion)
CRIT_LIBS = $(shell pkg-config --libs criterion)


$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TEST_BIN): $(APP_OBJS) $(TEST_SRCS)
	$(CC) $(CFLAGS) $(CRIT_CFLAGS) -o $@ $(APP_OBJS) $(TEST_SRCS) $(CRIT_LIBS) $(LDFLAGS)


.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET) $(TEST_BIN)

.PHONY: test
test: $(TEST_BIN)
	./$(TEST_BIN)
