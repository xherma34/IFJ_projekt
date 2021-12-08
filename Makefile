#
# Projekt: IFJ 2021
# file: Makefile
#
# author: Jan Zdeněk (xzdene01)
#

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -lm -fcommon -g
TARGET = ifj21
SOURCES = scanner.o DLList.o parser.o symtable.o TStack.o expression.o

currDir:=$(PWD)
testDir:=$(currDir)/Tests

n = 1
s ?= @

TEST_INPUT = $(testDir)/Inputs/testInput_$(n).txt
TEST_OUTPUT = $(testDir)/Outputs/testOutput_$(n).txt
REF_OUTPUT = $(testDir)/Referals/referOutput_$(n).txt

.PHONY: all test debug clean

all: DLList.o scanner.o symtable.o TStack.o expression.o parser.o
	$(s)$(CC) $(CFLAGS) -o $(currDir)/$(TARGET) $(SOURCES) main.c

run:
	$(s)./ifj21 < $(TEST_INPUT) >$(TEST_OUTPUT)

scanner.o: scanner.c scanner.h
	$(s)$(CC) $(CFLAGS) -c scanner.c

expression.o: expression.c expression.h
	$(s)$(CC) $(CFLAGS) -c expression.c

TStack.o: TStack.c TStack.h
	$(s)$(CC) $(CFLAGS) -c TStack.c

DLList.o: DLList.c DLList.h
	$(s)$(CC) $(CFLAGS) -c DLList.c

parser.o: parser.c parser.h
	$(s)$(CC) $(CFLAGS) -c parser.c

symtable.o: symtable.c symtable.h
	$(s)$(CC) $(CFLAGS) -c symtable.c

tests:
	if [ ! -d Tests/Outputs ]; then mkdir Tests/Outputs; fi
	$(s)bash $(currDir)/Tests/Scripts/test.sh

clean:
	$(s)rm -f $(SOURCES)
	$(s)rm -f $(currDir)/*.o
	$(s)rm -rf $(testDir)/Outputs/
	$(s)rm -f $(currDir)/ifj21
	$(s)rm -f $(currDir)/a.out
