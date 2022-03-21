.SUFFIXES: .c .h .x .o
#aktualny katalog w ktorym znajduje sie Makefile
DIR = `basename $(CURDIR)`
#flagi kompilatora
FLAGI = -pedantic -Wall -std=c99
#zmienne nazwowe
NAME_MAIN = 5a
NAME_PROD = producent
NAME_KONS = konsument
PROD_FILE = in.dat
KONS_FILE = out.dat
FIFO_NAME = FIFO
#pliki obiektowe
OBJS_MAIN = $(NAME_MAIN).o
OBJS_PROD = $(NAME_PROD).o
OBJS_KONS = $(NAME_KONS).o
#pliki executable
EXEC_MAIN = $(NAME_MAIN).x
EXEC_PROD = $(NAME_PROD).x
EXEC_KONS = $(NAME_KONS).x
#kompilator
CO = gcc

#tworzenie plikow obiektowych
%.o: %.c     
	@$(CO) $(FLAGI) -c $< -o $@

#tworzenie plikow executable
%.x: %.o
	@$(CO) $(FLAGI)  $< -o $@

#bezwzgledne wykonanie

#kompilacja wszystkich plikow
compile: $(EXEC_MAIN) $(EXEC_PROD) $(EXEC_KONS)

test_runA: compile
	@echo Uruchamianie programu A...
	@echo 
	@./$(EXEC_MAIN) $(PROD_FILE) $(KONS_FILE) $(FIFO_NAME)
	@echo ----------------------------------------------
	@echo Sprawdzanie czy plik $A oraz $B sa takie same.
	@diff -s $(PROD_FILE) $(KONS_FILE)
	@echo ----------------------------------------------


test_runB: $(EXEC_PROD) $(EXEC_KONS)
	@mkfifo -m 0666 $(FIFO_NAME)
	@xterm -hold -title $(NAME_PROD) -fa 'Monospace' -fs 12 -e ./$(EXEC_PROD) $(PROD_FILE) $(FIFO_NAME) 1 &
	@xterm -hold -title $(NAME_KONS) -fa 'Monospace' -fs 12 -e ./$(EXEC_KONS) $(KONS_FILE) $(FIFO_NAME) 1
	@rm $(FIFO_NAME)
	@diff -s 

#bezwzgledne wykonanie
.PHONY: clean tar
#czyszczenie folderu
clean:
	@rm *.x

#archiwizacja
tar: compile clean
	@rm *.o
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.