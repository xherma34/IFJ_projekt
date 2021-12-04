CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -lm -fcommon -g
TARGET = ifj21
SOURCES = scanner.o DLList.o parser.o symtable.o
TESTSOURCES = $(SOURCES) $(testDir)/test.c

currDir:=$(PWD)
testDir:=$(currDir)/Tests

n = 1
s ?= @

TEST_INPUT = $(testDir)/Inputs/testInput_$(n).txt
TEST_OUTPUT = $(testDir)/Outputs/testOutput_$(n).txt
REF_OUTPUT = $(testDir)/Referals/referOutput_$(n).txt

.PHONY: all test debug clean

all: scanner.o DLList.o parser.o symtable.o
	$(s)$(CC) $(CFLAGS) -o $(currDir)/$(TARGET) $(SOURCES) main.c

run:
	$(s)./ifj21 < $(TEST_INPUT)

scanner.o: scanner.c scanner.h
	$(s)$(CC) $(CFLAGS) -c scanner.c

DLList.o: DLList.c DLList.h
	$(s)$(CC) $(CFLAGS) -c DLList.c

parser.o: parser.c parser.h
	$(s)@$(CC) $(CFLAGS) -c parser.c

symtable.o: symtable.c symtable.h
	$(s)@$(CC) $(CFLAGS) -c symtable.c

tests:
	if [ ! -d Tests/Outputs ]; then mkdir Tests/Outputs; fi
	$(s)$(CC) $(CFLAGS) -o $(testDir)/test.o $(TESTSOURCES)
	$(s)bash $(currDir)/Tests/Scripts/test.sh

test:
	if [ ! -d Tests/Outputs ]; then mkdir Tests/Outputs; fi
	$(s)$(CC) $(CFLAGS) -o $(testDir)/test.o $(TESTSOURCES)
	$(s)Tests/test.o  < $(TEST_INPUT) > $(TEST_OUTPUT)
	-$(s)diff -us $(REF_OUTPUT) $(TEST_OUTPUT)

clean:
	$(s)rm -f $(SOURCES)
	$(s)rm -f $(currDir)/*.o
	$(s)rm -rf $(testDir)/Outputs/
	$(s)rm -f $(testDir)/*.o
	$(s)rm -f $(currDir)/ifj21
