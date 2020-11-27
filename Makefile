### 
## Makefile
## INFO2050: Projet 2
## RANDAXHE Martin && RUSSE Cyril
## 
### 

## Variables

# Tools & flags
CC = gcc
CFLAGS = --std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -DNDEBUG 
LD = gcc
LDFLAGS =

# Files
EXEC = test

MODULES_LISTESET = ListSet.c main.c StringArray.c Intersection.c
OBJECTS_LISTESET = ListSet.o main.o StringArray.o Intersection.o

MODULES_TREESET = TreeSet.c main.c StringArray.c Intersection.c
OBJECTS_TREESET = TreeSet.o main.o StringArray.o Intersection.o

MODULES_HASHSET = HashSet.c main.c StringArray.c Intersection.c
OBJECTS_HASHSET = HashSet.o main.o StringArray.o Intersection.o


## Rules

#compilation

test_list: $(OBJECTS_LISTESET)
	$(LD) -o $(EXEC) $(OBJECTS_LISTESET) $(LDFLAGS) -lm

test_tree: $(OBJECTS_TREESET)
	$(LD) -o $(EXEC) $(OBJECTS_TREESET) $(LDFLAGS) -lm

test_hash: $(OBJECTS_HASHSET)
	$(LD) -o $(EXEC) $(OBJECTS_HASHSET) $(LDFLAGS) -lm


main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

StringArray.o: StringArray.c
	$(CC) -c StringArray.c -o StringArray.o $(CFLAGS)

ListSet.o: ListSet.c
	$(CC) -c ListSet.c -o ListSet.o $(CFLAGS)

TreeSet.o: TreeSet.c
	$(CC) -c TreeSet.c -o TreeSet.o $(CFLAGS)

Intersection.o: Intersection.c
	$(CC) -c Intersection.c -o Intersection.o $(CFLAGS)

HashSet.o: HashSet.c
	$(CC) -c HashSet.c -o HashSet.o $(CFLAGS)



#rapport
rapport:all_rapport clean_log_aux

all_rapport:
	cd ../rapport && pdflatex tad-RusseCyril.tex

clean_log_aux:
	cd ../rapport && rm -f tad-RusseCyril.aux tad-RusseCyril.log tad-RusseCyril.toc

clean_rapport: clean_log_aux
	cd ../rapport && rm -f tad-RusseCyril.pdf

#création archive
archive:
	cd .. && tar -zcvf demineur_01.tar.gz source rapport doc

#supp fichiers objets + executable
clean:
	rm -f *.o $(EXEC)