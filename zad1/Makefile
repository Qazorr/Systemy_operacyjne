# ------- MAKEFILE ------- #

#nazwa biezacego katalogu, wykorzystywana np do tarowania
DIR = `basename $(CURDIR)`

#opcje kompilatora
FLAGI = -Wall -pedantic

#pomocnicze funkcje
FUNKCJE = funkcje.c


#kompilacja do plikow obiektowych
a.o: a.c
	@gcc a.c $(FLAGI) -c

b.o: b.c
	@gcc b.c $(FLAGI) -c

c.o: c.c
	@gcc c.c $(FLAGI) -c

d.o: d.c
	@gcc d.c $(FLAGI) -c

e.o: e.c
	@gcc e.c $(FLAGI) -c


#kompilacja do plikow executable
a.x: a.o $(FUNKCJE)
	@gcc a.o $(FUNKCJE) $(FLAGI) -o a.x

b.x: b.o $(FUNKCJE)
	@gcc b.o $(FUNKCJE) $(FLAGI) -o b.x

c.x: c.o $(FUNKCJE)
	@gcc c.o $(FUNKCJE) $(FLAGI) -o c.x
	@touch drzewo.txt

d.x: d.o $(FUNKCJE)
	@gcc d.o $(FUNKCJE) $(FLAGI) -o d.x

e.x: e.o $(FUNKCJE)
	@gcc e.o $(FUNKCJE) $(FLAGI) -o e.x


#komenda all sprawiajaca kompilacje wszystkich plikow
#do plikow obiektowych i executable
.PHONY: all
all: a.x b.x c.x d.x e.x
	


.PHONY: runA runB runC runD runE
#komendy odpowiadajace za uruchamianie programow
#za pomoca make
runA: a.x
	./a.x

runB: b.x
	./b.x

runC: c.x
	./c.x 
	@pstree -p > drzewo.txt

runD: d.x
	./d.x

runE: e.x 
	./e.x


.PHONY: clean tar

#clean usuwajacy wszystkie pliki z rozszerzeniem .o oraz .x
clean:
	rm *.o *.x drzewo.txt

#tarowanie calego folderu
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
