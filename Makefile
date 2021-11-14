CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -lm -fcommon 
CEXE = ifj21
SOURCES = scanner.c DLList.c
INPUT = test.txt

.PHONY: all

ifj21: $(SOURCES)
	$(CC) $(CFLAGS) -o $(CEXE) $(SOURCES)

run: $(CEXE) 
	./$(CEXE) < $(INPUT)

clean: $(CEXE)
	rm -f $(CEXE)