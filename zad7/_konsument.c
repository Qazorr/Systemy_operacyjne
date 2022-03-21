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
    int size = atoi(argv[2]);   //@param size wielkosc tablicy towar
    int memory_descryptor = shmemory_otworz(argv[1], O_RDWR, 0644); //@param memory_descryptor deskryptor do pamieci dzielonej

    SegmentPD *mem_kons = (SegmentPD *)memory_map(NULL, sizeof(SegmentPD), PROT_WRITE | PROT_READ, MAP_SHARED, memory_descryptor, 0);
    mem_kons->wstaw = 0;    //inicjalizacja wstaw
    mem_kons->wyjmij = 0;   //inicjalizacja wyjmij

    sem_t *S_PROD = sem_otworz(argv[3], 0);     //@param S_PROD semafor producent
    sem_t *S_KONS = sem_otworz(argv[4], 0);     //@param S_KONS semafor konsument
    
    int value_prod;     //@param value_prod zmienna przechowujaca wartosc semafora PROD
    int value_kons;     //@param value_kons zmienna przechowujaca wartosc semafora KONS
    sem_pobierz_wartosc(S_PROD, &value_prod);
    sem_pobierz_wartosc(S_KONS, &value_kons);

    sleep(2);

    //* poczatkowy komunikat
    printf("\nKONSUMENT -> Nazwa semafora: %s | o adresie: %p | wartosci poczatkowej: %d", argv[3], (void *)S_PROD, value_prod);
    printf("\nKONSUMENT -> Nazwa semafora: %s | o adresie: %p | wartosci poczatkowej: %d", argv[4], (void *)S_KONS, value_kons);
    printf("\nKONSUMENT -> Deskryptor pamieci dzielonej: %d\n\n", memory_descryptor);

    //@param konsument deskryptor pliku wejsciowego
    int konsument;
    if((konsument = open(argv[5], O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1){
        perror("OPEN ERR -> PROD\n");
        _exit(EXIT_FAILURE);
    }

    //@param towar tablica znakowa przechowujaca zczytane znaki
    char towar[size];
    memset(towar, 0, NELE);  //inicjalizacja tablicy towar

    while(1)
    {
        P(S_KONS);
        sleep(randomNo());

        strcpy(towar, mem_kons->bufor[mem_kons->wyjmij]);

        if(strcmp(towar, "\0") == 0){       //jesli towar i "\0" beda takie same, konczymy program (taka wiadomosc wysyla producent kiedy zakonczy produkcje)
            printf("KONSUMENT ZAKONCZYL DZIALANIE\n");
            V(S_PROD);
            break;
        }

        sem_pobierz_wartosc(S_KONS, &value_kons);
        printf("KONSUMENT ->  WARTOSC SEMAFORA KONS = %d | INDEKS BUFORA = %d | POBIERANY TEKST:  %s | WIELKOSC TEKSTU = %ld\n", value_kons, mem_kons->wyjmij, towar, strlen(towar));
       
        mem_kons->wyjmij = (mem_kons->wyjmij + 1) % NBUF;   //przesuniecie pozycji wyjmowania o 1

        write(konsument, towar, sizeof(char)*strlen(towar));    //wpisanie ciagu znakow do pliku

        memset(towar, 0, size);     //wyczyszczenie tablicy towar

        V(S_PROD);
    }

    //* Po zakonczeniu dzialania zamykamy wszystkie otwarte wczesniej deskryptory
    if(close(konsument) == -1){
        perror("CLOSE ERROR -> KONSUMENT\n");
        exit(EXIT_FAILURE);
    }
    sem_zamknij(S_PROD);
    sem_zamknij(S_KONS);
    memory_unlink_map(mem_kons, sizeof(SegmentPD));
    shmemory_zamknij(memory_descryptor);

    exit(EXIT_SUCCESS);
}