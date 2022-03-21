.SUFFIXES: .c .h .x .o
#aktualny katalog w ktorym znajduje sie Makefile
DIR = `basename $(CURDIR)`
#flagi kompilatora
FLAGI = -pedantic -Wall -std=c99
#zmienne nazwowe
NAME = 1
#pliki obiektowe
OBJS = $(NAME).o
#pliki executable
EXEC = $(NAME).x
#kompilator
CO = gcc

#tworzenie plikow obiektowych .o
%.o: %.c
	@$(CO) $(FLAGI) -c $<

#bezwzgledne wykonanie
.PHONY: $(EXEC) compile
#stworzenie pliku executable
$(EXEC): $(OBJS)
	@$(CO) -o $@ $(FLAGI) $^
	@rm $^
	@echo Plik executable $@ zostal utworzony
#komenda odpowiadajaca za utworzenie programu
compile: $(EXEC)

.PHONY: run
#komenda odpowiadajaca za uruchamianie programu
run: $(EXEC)
	@echo Uruchamianie programu...
	@echo
	@./$^ $A $B
	@echo Sprawdzanie czy plik $A oraz $B sa takie same.
	@diff -s $A $B
	

#bezwzgledne wykonanie
.PHONY: clean tar
#czyszczenie folderu
clean:
	@rm *.x
#archiwizacja
tar: compile clean
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.