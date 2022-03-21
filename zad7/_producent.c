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
#include <string.h>

//* wlasne biblioteki
#include "semaphores.h"
#include "shared_mem.h"

//@param SegmentPD Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE]; //@param bufor wspolny bufor danych
    int wstaw, wyjmij; // @param wstaw pozycja wstawiania z bufora @param wyjmij pozycja wyjmowania z bufora
} SegmentPD;

//* randomizacja czasu, ktory bedzie wstawiany do sleepa
int randomNo() {
    return (rand() % 3);
}


/**
    @param argv[1] nazwa pamieci dzielonej
    @param argv[2] wielkosc chunkow
    @param argv[3] nazwa semafora producenta
    @param argv[4] nazwa semafora konsumenta
    @param argv[5] nazwa pliku wejsciowego
*/
int main(int argc, char *argv[])
{
    int size = atoi(argv[2]);       //@param size wielkosc tablicy towar
    int memory_descryptor = shmemory_otworz(argv[1], O_RDWR, 0644); //@param memory_descryptor deskryptor do pamieci dzielonej
    
    SegmentPD *mem_prod = (SegmentPD *)memory_map(NULL, sizeof(SegmentPD), PROT_WRITE, MAP_SHARED, memory_descryptor, 0);   //@param mem_prod pointer do segmentu pamieci
    mem_prod->wstaw = 0;    //inicjalizacja wstaw
    mem_prod->wyjmij = 0;   //inicjalizacja wyjmij

    sem_t *S_PROD = sem_otworz(argv[3], 0);     //@param S_PROD semafor producent
    sem_t *S_KONS = sem_otworz(argv[4], 0);     //@param S_KONS semafor konsument

    int value_prod; //@param value_prod zmienna przechowujaca wartosc semafora PROD
    int value_kons; //@param value_kons zmienna przechowujaca wartosc semafora KONS
    sem_pobierz_wartosc(S_PROD, &value_prod);
    sem_pobierz_wartosc(S_KONS, &value_kons);

    sleep(1);

    //* poczatkowy komunikat
    printf("\nPRODUCENT -> Nazwa semafora: %s | o adresie: %p | wartosci poczatkowej: %d", argv[3], (void *)S_PROD, value_prod);
    printf("\nPRODUCENT -> Nazwa semafora: %s | o adresie: %p | wartosci poczatkowej: %d", argv[4], (void *)S_KONS, value_kons);
    printf("\nPRODUCENT -> Deskryptor pamieci dzielonej: %d\n", memory_descryptor);

    int producent;      //@param producent deskryptor pliku wejsciowego
    if((producent = open(argv[5], O_RDONLY, 0644)) == -1){
        perror("OPEN ERROR -> PROD\n");
        _exit(EXIT_FAILURE);
    }

    int bytes;  //@param bytes ilosc bitow pobieranych przez read
    char towar[size];   //@param towar tablica znakowa przechowujaca zczytane znaki
    memset(towar, 0, NELE);    //inicjalizacja tablicy towar

    while(1)
    {
        P(S_PROD);
        sleep(randomNo());
        bytes = read(producent, towar, size);   //czytanie do "towar" z pliku ciagu znakow
        if(bytes == -1){
            printf("READ ERROR -> PRODUCENT\n");
            break;
        }
        if(bytes == 0){         //jesli bytes == 0 oznacza to ze nasz read nic nie przeczytal (skoczyly sie znaki)
            printf("PRODUCENT ZAKONCZYL DZIALANIE\n");
            V(S_KONS);
            strcpy(mem_prod->bufor[mem_prod->wstaw], "\0");     //przekazanie do tablicy w pamieci znaku "\0" ktory zakonczy konsumenta
            break;
        }
        sem_pobierz_wartosc(S_PROD, &value_prod);
        printf("\tPRODUCENT ->  WARTOSC SEMAFORA PROD = %d | INDEKS BUFORA = %d | PRZESYLANY TEKST: %s | WIELKOSC TEKSTU = %ld\n", value_prod, mem_prod->wstaw, towar, strlen(towar));
        strcpy(mem_prod->bufor[mem_prod->wstaw], towar);    //do bufora w pamieci umieszczamy towar pobrany przez read
        
        mem_prod->wstaw = (mem_prod->wstaw + 1) % NBUF;     //przesuniecie pozycji wstawiania o 1
        memset(towar, 0, size);     //czyszczenie tablicy towar

        V(S_KONS);
    }

    //* Po zakonczeniu dzialania zamykamy wszystkie otwarte wczesniej deskryptory
    if(close(producent) == -1){
        perror("CLOSE ERROR -> PRODUCENT\n");
        exit(EXIT_FAILURE);
    }
    sem_zamknij(S_PROD);
    sem_zamknij(S_KONS);
    memory_unlink_map(mem_prod, sizeof(SegmentPD));
    shmemory_zamknij(memory_descryptor);


    exit(EXIT_SUCCESS);
}