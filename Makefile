CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -lm -fcommon -g
TARGET = ifj21
SOURCES = scanner.o DLList.o
TESTSOURCES = $(SOURCES) $(testDir)/test.c

currDir:=$(PWD)
testDir:=Tests

n = 1
s ?= @

TEST_INPUT = $(testDir)/Inputs/testInput_$(n).txt
TEST_OUTPUT = $(testDir)/Outputs/testOutput_$(n).txt
REF_OUTPUT = $(testDir)/Referals/referOutput_$(n).txt

.PHONY: all test debug clean

all: scanner.o DLList.o
	$(s)$(CC) $(CFLAGS) -o $(currDir)/$(TARGET) $(SOURCES) main.c


scanner.o: scanner.c scanner.h
	$(s)@$(CC) $(CFLAGS) -c scanner.c

DLList.o: DLList.c DLList.h
	$(s)@$(CC) $(CFLAGS) -c DLList.c

scannerTest:
	$(s)bash $(currDir)/Tests/Scripts/scannerTest.sh

test: Tests/test.c
	$(s)$(CC) $(CFLAGS) -o $(testDir)/test.o $(TESTSOURCES)
	$(s)Tests/test.o  < $(TEST_INPUT) > $(TEST_OUTPUT)
	- $(s)diff -us $(REF_OUTPUT) $(TEST_OUTPUT)

debug: Tests/test.c
	$(s)$(CC) $(CFLAGS) -ggdb3 -o $(testDir)/test.o $(TESTSOURCES)
	$(s)valgrind --tool=memcheck --leak-check=yes Tests/./test.o  < $(TEST_INPUT) > $(TEST_OUTPUT)

clean:
	$(s)rm -f $(SOURCES)
	$(s)rm -f $(currDir)/*.o
	$(s)rm -f $(testDir)/Outputs/*
	$(s)rm -f $(currDir)/ifj21
