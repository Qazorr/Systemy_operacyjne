.SUFFIXES: .c .h .txt

#flagi kompilatora
FLAGI = -pedantic -Wall -std=c99

#katalogi
DIR = `basename $(CURDIR)`
LIB_DIR = ./lib

#zmienne nazwowe
NAME_SERWER = serwer
NAME_KLIENT = klient
NAME_TESTER = tester
NAME_KOLEJKA = kolejki
LIB_NAME = functions

#pliki obiektowe
OBJS_SERWER = $(NAME_SERWER).o
OBJS_KLIENT = $(NAME_KLIENT).o
OBJS_TESTER = $(NAME_TESTER).o
OBJS_KOLEJKA = $(NAME_KOLEJKA).o

#pliki executable
EXEC_SERWER = $(NAME_SERWER).x
EXEC_KLIENT = $(NAME_KLIENT).x
EXEC_TESTER = $(NAME_TESTER).x

#pliki naglowkowe
HEAD_KOLEJKA = $(NAME_KOLEJKA).h

#biblioteka statyczna
LIB_STAT = lib$(LIB_NAME).a

#archiwizer oraz jego opcje
AR = ar
ARFLAGS = rsv

#kompilator i linker
CO = gcc
LD = $(CO)

#skompiluj wszystko
all: $(EXEC_SERWER) $(EXEC_KLIENT) $(EXEC_TESTER)

#tworzenie biblioteki statycznej
$(LIB_STAT): $(OBJS_KOLEJKA)
	@$(AR) $(ARFLAGS) $@ $^
	@mkdir -p $(LIB_DIR)
	@mv $(LIB_STAT) $(LIB_DIR)

#tworzenie plikow executable
%.x: %.o $(LIB_STAT)
	@$(LD) -o $@ $(FLAGI) $< -L$(LIB_DIR) -l$(LIB_NAME) -lrt

#uruchamianie z wejsciem wprowadzonym przez uzytkownika
run: all
	@xterm -title SERWER -fa 'Monospace' -fs 11 -bg IndianRed -geometry 120x50+10+10 -e ./$(EXEC_SERWER) &
	@xterm -title KLIENT1 -fa 'Monospace' -fs 11 -bg MistyRose3 -geometry 90x24+1200+0 -e ./$(EXEC_KLIENT) &
	@xterm -title KLIENT2 -fa 'Monospace' -fs 11 -bg MistyRose3 -geometry 90x24+1200+500 -e ./$(EXEC_KLIENT)

#uruchomienie z testerem
run_tester: all
	@xterm -hold -title SERWER -fa 'Monospace' -fs 11 -bg IndianRed -geometry 120x50+10+10 -e ./$(EXEC_SERWER) &
	@xterm -hold -title KLIENT1 -fa 'Monospace' -fs 11 -bg MistyRose3 -geometry 90x24+1200+0 -e ./$(EXEC_TESTER) &
	@xterm -hold -title KLIENT2 -fa 'Monospace' -fs 11 -bg MistyRose3 -geometry 90x24+1200+500 -e ./$(EXEC_TESTER)

#usuwanie kolejki serwera (jesli nastapi jakis blad)
fix: unlink.x
	@echo Fixing in progress...
	@./unlink.x

#czyszczenie folderu
clean:
	@echo Inicjalizowanie czyszczenia...
	@echo Pliki .x *.o zostaly usuniete.
	@rm -f *.x *.o
	@echo Usuwanie katalogu lib...
	@rm -f -r lib

#archiwizacja
tar: clean
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.