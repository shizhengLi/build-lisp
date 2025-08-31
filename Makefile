CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g
LDFLAGS = 
SRCDIR = src
TESTDIR = test
DOCDIR = docs

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

# Separate test_runner and test_macro_only
TEST_RUNNER_SOURCES = $(filter-out $(TESTDIR)/test_macro_only.c,$(TEST_SOURCES))
TEST_RUNNER_OBJECTS = $(TEST_RUNNER_SOURCES:.c=.o)
TEST_MACRO_ONLY_SOURCES = $(TESTDIR)/test_macro_only.c $(TESTDIR)/test_macro.c
TEST_MACRO_ONLY_OBJECTS = $(TEST_MACRO_ONLY_SOURCES:.c=.o)

TARGET = lispy
TEST_TARGET = test_runner
TEST_MACRO_ONLY_TARGET = test_macro_only

.PHONY: all clean test docs

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TESTDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -I$(SRCDIR) -c -o $@ $<

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(filter-out src/main.o,$(OBJECTS)) $(TEST_RUNNER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_MACRO_ONLY_TARGET): $(filter-out src/main.o,$(OBJECTS)) $(TEST_MACRO_ONLY_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(TEST_MACRO_ONLY_TARGET) $(OBJECTS) $(TEST_OBJECTS)

docs:
	@echo "Documentation is in $(DOCDIR)/"

run: $(TARGET)
	./$(TARGET)

memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)