/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

//* biblioteki
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

//* wlasne biblioteki
#include "semaphores.h"
#include "shared_mem.h"

//* makra
#define SIZE 16 //@param SIZE wielkosc chunkow podawanych/pobieranych przez producenta/konsumenta
#define SEM_PROD_NAME "SEM_PROD" //@param SEM_PROD_NAME nazwa semafora producenta
#define SEM_KONS_NAME "SEM_KONS" //@param SEM_KONS_NAME nazwa semafora konsumenta
#define SHM_NAME "/shared_mem" //@param SHM_NAME nazwa pamieci dzielonej
#define KONS_SEMAFORE_INIT_VAL 0 //@param KONS_SEMAFORE_INIT_VAL liczba z jaka inicjalizujemy semafor konsumenta
#define PATH "./"

sem_t *p_ptr = NULL;    //@param pointer do semafora producenta
sem_t *k_ptr = NULL;    //@param pointer do semafora konsumenta
int memory_descryptor;  //@param deskryptor do pamieci


//@param SegmentPD Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE]; //@param bufor wspolny bufor danych
    int wstaw, wyjmij; // @param wstaw pozycja wstawiania z bufora @param wyjmij pozycja wyjmowania z bufora
} SegmentPD;


//handler do wywolania ctrl-c z klawiatury
static void handler(int sigNum)
{
    printf("SIGINT ZOSTAL WYWOLANY\n");
    exit(EXIT_SUCCESS);
}

//funkcja do atexit -> najpierw zamykamy semafory, oraz pamiec dzielona, potem je zamykamy
void ending_sequence()
{
    sem_zamknij(p_ptr);
    sem_usun(SEM_PROD_NAME);
    printf("\n-------------------------------------------------\n");
    printf("\t\tSEMAFOR PRODUCENT USUNIETY");
    printf("\n-------------------------------------------------\n");

    sem_zamknij(k_ptr);
    sem_usun(SEM_KONS_NAME);
    printf("\n-------------------------------------------------\n");
    printf("\t\tSEMAFOR KONSUMENT USUNIETY");
    printf("\n-------------------------------------------------\n");

    shmemory_zamknij(memory_descryptor);
    shmemory_unlink(SHM_NAME);
    printf("\n-------------------------------------------------\n");
    printf("\t\tPAMIEC DZIELONA USUNIETA");
    printf("\n-------------------------------------------------\n");

    
    _exit(EXIT_SUCCESS);
}

/**
    *   @param argv[1]  nazwa producenta
    *   @param argv[2]  nazwa konsumenta
    *   @param argv[3]  nazwa pliku wejsciowego
    *   @param argv[4]  nazwa pliku wyjsciowego
*/
int main(int argc, char *argv[])
{   
    //jesli wielkosc tablic ktore bedziemy chcieli dodawac do pamieci jest za duzy dla pamieci, pokaz blad
    if(SIZE >= NELE){
        printf("SIZE IS TOO BIG -> CHECK README AND SHARED_MEM.H\n");
        exit(EXIT_FAILURE);
    }

    //error handling dla zlej ilosci argumentow
    if(argc != 5){
        printf("ZLE WYWOLANIE!\nPRAWIDLOWE WYWOLANIE: %s PLIK_PROD PLIK_KONS PLIK_IN PLIK_OUT\n", argv[0]);
        _exit(EXIT_FAILURE);
    }

    //ustawienie funkcji atexit z funkcja ending_sequence() -> patrz line 51
    if(atexit(ending_sequence) != 0){
        perror("ATEXIT ERROR -> MACIERZYSTY\n");
        _exit(EXIT_FAILURE);
    }

    //ustawienie handlera dla SIGINT
    if(signal(SIGINT, handler) == SIG_ERR){
        perror("SIGNAL ERROR -> MACIERZYSTY\n");
        exit(EXIT_FAILURE);
    }

    //utworzenie pamieci dzielonej, oraz ustawienie jej wielkosci na wielkosc struktury SegmentPD
    memory_descryptor = shmemory_otworz(SHM_NAME, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0644);
    shmemory_ustaw(memory_descryptor, sizeof(SegmentPD));

    sem_t *SEM_PROD = sem_stworz(SEM_PROD_NAME, O_CREAT | O_EXCL, 0644, NBUF);  //@param SEM_PROD semafor producenta
    sem_t *SEM_KONS = sem_stworz(SEM_KONS_NAME, O_CREAT | O_EXCL, 0644, KONS_SEMAFORE_INIT_VAL);   //@param SEM_KONS semafor konsumenta

    //ustawienie pointerow na semafory (uzywane w funkcji ending_sequence())
    p_ptr = SEM_PROD;
    k_ptr = SEM_KONS;
    
    int value_prod; //@param value_prod zmienna przechowujaca wartosc semafora PROD
    int value_kons; //@param value_kons zmienna przechowujaca wartosc semafora KONS
    sem_pobierz_wartosc(SEM_PROD, &value_prod);
    sem_pobierz_wartosc(SEM_KONS, &value_kons);

    //* komunikaty wypisywane przy uruchomieniu programu
    printf("\nMAIN -> Nazwa semafora: %s | o adresie: %p | wartosci poczatkowej: %d\n", SEM_PROD_NAME, (void *)SEM_PROD, value_prod);
    printf("MAIN -> Nazwa semafora: %s | o adresie: %p | wartosci poczatkowej: %d\n", SEM_KONS_NAME, (void *)SEM_KONS, value_kons);
    printf("MAIN -> Deskryptor pamieci dzielonej: %d | wielkosc pamieci: %ld\n", memory_descryptor, sizeof(SegmentPD));
    sleep(1);


    char exec_prod[32];     //@param exec_prod tablica przechowujaca ./plik_executable
    char exec_kons[32];     //@param exec_kons tablica przechowujaca ./plik_executable
    char mem_d[8];          //@param mem_d tablica przechowujaca memory_descryptor
    char size[8];           //@param size tablica przechowujaca wielkosc chunkow podawanych/pobieranych przez producenta/konsumenta
    sprintf(exec_prod, "%s%s", PATH, argv[1]);
    sprintf(exec_kons, "%s%s", PATH, argv[2]);
    sprintf(mem_d, "%d", memory_descryptor);
    sprintf(size, "%d", SIZE);

    int child_PID;  //@param child_PID zmienna przechowujaca PID potomka
    int i;  //@param i iterator petli

    for(i = 0; i<2; i++)
    {  
        child_PID = fork();
        switch(child_PID)
        {
            //! CASE ERROR
            case -1:
                perror("FORK ERROR\n");
                exit(EXIT_FAILURE);
            //! CASE POTOMEK
            case 0:
                //* URUCHAMIANIE PRODUCENTA
                if(i == 0){
                    execl(exec_prod, argv[1], SHM_NAME, size, SEM_PROD_NAME, SEM_KONS_NAME, argv[3], NULL);
                    perror("EXECL ERROR -> EXEC_PROD\n");
                    _exit(EXIT_FAILURE);
                }
                //* URUCHAMIANIE KONSUMENTA
                else{
                    execl(exec_kons, argv[2], SHM_NAME, size, SEM_PROD_NAME, SEM_KONS_NAME, argv[4], NULL);
                    perror("EXECL ERROR -> EXEC_KONS\n");
                    _exit(EXIT_FAILURE);
                }
            //! CASE MACIERZYSTY
            default:
                break;
        }
    }

    //proces macierzysty czeka na zakoczenie dwoch procesow potomnych (producenta i konsumenta)
    for(i = 0; i<2; i++){
        if(wait(NULL) == -1){
            perror("WAIT ERROR\n");
            _exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}