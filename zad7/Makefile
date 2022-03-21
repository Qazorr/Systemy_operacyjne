.SUFFIXES: .c .h .x .o .dat .txt

#katalogi
DIR = `basename $(CURDIR)`
LIB_DIR = ./lib

#flagi kompilatora
FLAGI = -pedantic -Wall -std=gnu99
CFLAGS = -pthread -lrt

#zmienne nazwowe
MAIN_NAME = main
PROD_NAME = _producent
KONS_NAME = _konsument
SEMAPHORES_NAME = semaphores
SHARED_MEM_NAME = shared_mem
IN = in.txt
OUT = out.txt
LIB_NAME = functions

#pliki obiektowe
OBJS_MAIN = $(MAIN_NAME).o
OBJS_PROD = $(PROD_NAME).o
OBJS_KONS = $(KONS_NAME).o
OBJS_SEM = $(SEMAPHORES_NAME).o
OBJS_SHM = $(SHARED_MEM_NAME).o

#pliki executable
EXEC_STATIC = $(MAIN_NAME)S.x
EXEC_DYNAMIC_1 = $(MAIN_NAME)D1.x
EXEC_DYNAMIC_2 = $(MAIN_NAME)D2.x

EXEC_STATIC_PROD = $(PROD_NAME)S.x
EXEC_DYNAMIC_1_PROD = $(PROD_NAME)D1.x
EXEC_DYNAMIC_2_PROD = $(PROD_NAME)D2.x

EXEC_STATIC_KONS = $(KONS_NAME)S.x
EXEC_DYNAMIC_1_KONS = $(KONS_NAME)D1.x
EXEC_DYNAMIC_2_KONS = $(KONS_NAME)D2.x

#pliki naglowkowe
HEAD_SEM = $(SEMAPHORES_NAME).h
HEAD_SHM = $(SHARED_MEM_NAME).h

#biblioteka statyczna
LIB_STAT = lib$(LIB_NAME).a
#biblioteka dzielona
LIB_SHAR = lib$(LIB_NAME).so


#archiwizer oraz jego opcje
AR = ar
ARFLAGS = rsv

#kompilator i linker
CO = gcc
LD = $(CO)

#tworzenie plikow obiektowych
%.o: %.c
	@$(CO) $^ $(FLAGI) -c

#tworzenie biblioteki statycznej
$(LIB_STAT): $(OBJS_SEM) $(OBJS_SHM)
	@$(AR) $(ARFLAGS) $@ $^
	@mkdir -p $(LIB_DIR)
	@mv $(LIB_STAT) $(LIB_DIR)

#tworzenie biblioteki dzielonej
$(LIB_SHAR): $(OBJS_SEM) $(OBJS_SHM)
	@$(LD) -shared -o $@ $(FLAGI) $(OBJS_SEM) $(OBJS_SHM)
	@mkdir -p $(LIB_DIR)
	@mv $(LIB_SHAR) $(LIB_DIR)

test_file:
	@echo Making a random $(IN) file...
	@cat /dev/urandom | tr -cd 'a-zA-Z0-9' | head -c  `shuf -i 150-200 -n 1` > $(IN)

#tworzenie plikow z biblioteka statyczna
%S.x: %.o $(LIB_STAT)
	@$(LD) -o $@ $(FLAGI) $< -L$(LIB_DIR) -l$(LIB_NAME) $(CFLAGS)

#tworzenie plikow z bliblioteka dzielona (1 mozliwosc)
%D1.x: %.o $(LIB_SHAR)
	@$(LD) -o $@ $(FLAGI) $< -L$(LIB_DIR) -l$(LIB_NAME) -Wl,-R $(LIB_DIR) $(CFLAGS)

#tworzenie plikow z bliblioteka dzielona (2 mozliwosc)
%D2.x: %.o $(LIB_SHAR)
	@$(LD) -o $@ $(FLAGI) $< -L$(LIB_DIR) -l$(LIB_NAME) $(CFLAGS)

.PHONY: run_static run_shared1 run_shared2

#uruchamianie programu dla kolejno:
#biblioteki statycznej
#biblioteki dzielonej (1 mozliwosc)
#biblioteki dzielonej (2 mozliwosc)
run_static: clean $(EXEC_STATIC) $(EXEC_STATIC_PROD) $(EXEC_STATIC_KONS) test_file
	@echo Trwa uruchamianie programu uzywajac biblioteki statycznej...
	@./$(EXEC_STATIC) $(EXEC_STATIC_PROD) $(EXEC_STATIC_KONS) $(IN) $(OUT)
	@echo Checking whether $(IN) and $(OUT) are identical...
	@diff -s $(IN) $(OUT)

run_shared1: clean $(EXEC_DYNAMIC_1) $(EXEC_DYNAMIC_1_PROD) $(EXEC_DYNAMIC_1_KONS) test_file
	@echo Trwa uruchamianie programu uzywajac pierwszego sposobu dla biblioteki dzielonej...
	@./$(EXEC_DYNAMIC_1) $(EXEC_DYNAMIC_1_PROD) $(EXEC_DYNAMIC_1_KONS) $(IN) $(OUT)
	@echo Checking whether $(IN) and $(OUT) are identical...
	@diff -s $(IN) $(OUT)

run_shared2: clean $(EXEC_DYNAMIC_2) $(EXEC_DYNAMIC_2_PROD) $(EXEC_DYNAMIC_2_KONS) test_file
	@echo Trwa uruchamianie programu uzywajac drugiego sposobu dla biblioteki dzielonej...
	@(export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC_DYNAMIC_2) $(EXEC_DYNAMIC_2_PROD) $(EXEC_DYNAMIC_2_KONS) $(IN) $(OUT))
	@echo Checking whether $(IN) and $(OUT) are identical...
	@diff -s $(IN) $(OUT)

	
#bezwzgledne wykonanie
.PHONY: clean tar

#czyszczenie folderu
clean:
	@echo Inicjalizowanie czyszczenia...
	@echo Pliki .x *.o $(IN) $(OUT) zostaly usuniete.
	@rm -f *.x *.o $(IN) $(OUT)
	@echo Usuwanie katalogu lib...
	@rm -f -r lib

#archiwizacja
tar: clean
	@(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	@echo Folder zostal zarchiwizowany.