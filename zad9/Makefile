.SUFFIXES: .c .h .txt

#flagi kompilatora
FLAGI = -pedantic -Wall -std=gnu99

#katalog
DIR = `basename $(CURDIR)`

#nazwa glownego pliku
NAME = main

#przykladowa liczba watkow/sekcji
NO_THREADS = 5
NO_SECTIONS = 2

#nazwa pliku executable
EXEC = $(NAME).x

#kompilator
CO = gcc

#tworzenie pliku executable
$(EXEC): $(NAME).c
	@$(CO) $(NAME).c $(FLAGI) -o $@ -lpthread

#uruchomienie z testowymi zmiennymi
test_run: $(EXEC)
	@./$(EXEC) $(NO_SECTIONS) $(NO_THREADS)

#czyszczenie folderu
clean:
	@echo Inicjalizowanie czyszczenia...
	@echo Pliki .x zostaly usuniete.
	@rm -f *.x

#archiwizacja
tar: clean
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.