#---------MAKEFILE---------#

#----------------------------------------------------------------------
#Autor: Kacper Piatkowski
#----------------------------------------------------------------------


#aktualny folder, uzyty w pozniejszej kompresji folderu
DIR = `basename $(CURDIR)`

#opcje kompilatora
FLAGI = -Wall -pedantic -std=c99

#kompilator
CO = gcc

#zmienne nazwowe
NAME1 = 1
NAME2a = 2a
NAME2b = 2b
NAME3a = 3a
NAME3b = 3b
NAME3c = 3c

#pliki obiektowe
OBJS1 = $(NAME1).o
OBJS2a = $(NAME2a).o
OBJS2b = $(NAME2b).o
OBJS3a = $(NAME3a).o
OBJS3b = $(NAME3b).o
OBJS3c = $(NAME3c).o

#pliki executable
EXEC1 = $(NAME1).x
EXEC2a = $(NAME2a).x
EXEC2b = $(NAME2b).x
EXEC3a = $(NAME3a).x
EXEC3b = $(NAME3b).x
EXEC3c = $(NAME3c).x

#kompilacja do plikow obiektowych
%.o: %.c
	@$(CO) $(FLAGI) -c $<

.PHONY: $(EXEC1) $(EXEC2a) $(EXEC3b)

$(EXEC1): $(OBJS1)
	@$(CO) -o $@ $(FLAGI) $^

$(EXEC2a): $(OBJS2a)
	@$(CO) -o $@ $(FLAGI) $^

$(EXEC2b): $(OBJS2b)
	@$(CO) -o $@ $(FLAGI) $^

$(EXEC3a): $(OBJS3a)
	@$(CO) -o $@ $(FLAGI) $^
$(EXEC3b): $(OBJS3b)
	@$(CO) -o $@ $(FLAGI) $^
$(EXEC3c): $(OBJS3c)
	@$(CO) -o $@ $(FLAGI) $^

#kompilacja 1 programu
compile1: $(EXEC1)
	@echo Plik executable $^ zostal utworzony 
	@rm *.o

#kompilacja 2 programu
compile2: $(EXEC2a) $(EXEC2b)
	@echo Pliki executable $^ zostaly utworzone 
	@rm *.o

#kompilacja 3 programu
compile3: $(EXEC3a) $(EXEC3b) $(EXEC3c)
	@echo Pliki executable $^ zostaly utworzone 
	@rm *.o

#komenda, ktora pozwala na skompilowanie wszystkich plikow do plikow executable
all: $(EXEC1) $(EXEC2a) $(EXEC2b) $(EXEC3a) $(EXEC3b) $(EXEC3c)
	@echo Pliki $^ zostaly utworzone
	@rm *.o

.PHONY: clean tar
#czyszczenie wszystkich plikow .x

clean:
	@rm *.x
	@echo Usuwanie wszystkich plikow .x...

#tarowanie calego folderu
tar: all clean
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.