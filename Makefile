CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -lm -fcommon
CEXE = ifj21
SOURCES = scanner.c DLList.c
TEST_INPUT = Tests/testInput.txt
TEST_OUTPUT = Tests/testOutput.txt
REF_OUTPUT = Tests/refOutput.txt

.PHONY: all

ifj21: $(SOURCES)
	$(CC) $(CFLAGS) -o $(CEXE) $(SOURCES)

run: $(CEXE)
	./$(CEXE) < $(TEST_INPUT)

clean: $(CEXE)
	rm -f $(CEXE) Tests/testExe

test: Tests/test.c
	$(CC) $(CFLAGS) -o Tests/testExe $(SOURCES) Tests/test.c
	Tests/./testExe  < $(TEST_INPUT) > $(TEST_OUTPUT)
	diff -u $(REF_OUTPUT) $(TEST_OUTPUT)
