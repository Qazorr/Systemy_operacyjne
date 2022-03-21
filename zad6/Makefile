.SUFFIXES: .c .h .x .o .dat .txt

#aktualny katalog w ktorym znajduje sie Makefile
DIR = `basename $(CURDIR)`

#flagi kompilatora
FLAGI = -pedantic -Wall -std=c99 -pthread

#zmienne nazwowe
NAME_POWIELACZ = powielacz
NAME_WYKLUCZAJACY = wykluczajacy
NAME_FUNCTIONS = funkcje
TEST_NO_CHILD = 5
TEST_NO_SECTIONS = 2

#pliki obiektowe
OBJS_POWIELACZ = $(NAME_POWIELACZ).o
OBJS_FUNCTIONS = $(NAME_FUNCTIONS).o
OBJS_WYKLUCZAJACY = $(NAME_WYKLUCZAJACY).o

#pliki executable
EXEC_POWIELACZ = $(NAME_POWIELACZ).x
EXEC_WYKLUCZAJACY = $(NAME_WYKLUCZAJACY).x

#pliki naglowkowe
HEADS3 = $(NAME_FUNCTIONS).h

#pliki biblioteki .c
NAME_FUNCTIONS_C =$(NAME_FUNCTIONS).c

#kompilator
CO = gcc

#tworzenie plikow obiektowych
%.o: %.c
	@$(CO) $^ $(FLAGI) -c

#tworzenie plikow executable
$(EXEC_POWIELACZ): $(OBJS_POWIELACZ) $(OBJS_FUNCTIONS)
	@$(CO) $^ $(FLAGI) -o $@

$(EXEC_WYKLUCZAJACY): $(OBJS_WYKLUCZAJACY) $(OBJS_FUNCTIONS)
	@$(CO) $^ $(FLAGI) -o $@

$(OBJS_POWIELACZ) $(OBJS_WYKLUCZAJACY): $(HEADS3)

#bezwzgledne wykonanie
.PHONY: test_run compile

#kompilacja do plikow executable
compile: $(EXEC_POWIELACZ) $(EXEC_WYKLUCZAJACY)
	@echo Pliki $^ zostaly utworzone...
	@rm *.o

#przykladowe uruchomienie programu
test_run: compile
	@echo Uruchamianie programu z testowymi argumentami..
	@echo 
	@./$(EXEC_POWIELACZ) $(EXEC_WYKLUCZAJACY) $(TEST_NO_CHILD) $(TEST_NO_SECTIONS)

#bezwzgledne wykonanie
.PHONY: clean tar

#czyszczenie folderu
clean:
	@echo Inicjalizowanie czyszczenia...
	@echo Pliki .x .txt zostaly usuniete.
	@touch error_dis.txt
	@rm *.x *.txt

#archiwizacja
tar: compile clean
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.