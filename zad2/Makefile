#---------MAKEFILE---------#


#aktualny folder, uzyty w pozniejszej kompresji folderu
DIR = `basename $(CURDIR)`

#opcje kompilatora
FLAGI = -Wall -pedantic

#kompilacja do plikow executable

zadanie.x: zadanie.c
	@gcc zadanie.c $(FLAGI) -o zadanie.x
	@echo Plik zadanie.x zostal utworzony
exec.x: exec.c
	@gcc exec.c $(FLAGI) -o exec.x
	@echo Plik exec.x zostal utworzony

#komenda all odpowiadajaca za kompilacje wszystkich plikow do plikow executable
.PHONY: all
all: zadanie.x exec.x

run: zadanie.x exec.x
	@echo 
	@echo Uruchamianie programu...
	./zadanie.x exec.x

.PHONY: clean tar
#czyszczenie wszystkich plikow .x
clean:
	@rm *.x
	@echo Usuwanie wszystkich plikow .x...

#tarowanie calego folderu
tar: all clean
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.