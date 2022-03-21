/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

//* biblioteki
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

//* wlasne biblioteki
#include "funkcje.h"

//* randomizacja czasu, ktory bedzie wstawiany do sleepa
int randomNo() {
    return (rand() % 3) + 1;
}

/*
   *    argv[1] - nazwa semafora
   *    argv[2] - nazwa pliku z numerem
   *    argv[3] - ilosc sekcji
*/
int main(int argc, char *argv[])
{

    //* otwarcie semafora Semafor funkcja sem_otworz (patrz funkcje.c line 13)
    sem_t *Semafor = sem_otworz(argv[1], O_RDWR);

    int sem_val;    //* sem_val -> przechowuje wartosc semafora
    int numer_value;    //* numer_value -> odpowiada za wartosc w pliku 

    //* numer -> zmienna plikowa zawierajaca plik .txt (default: numer.txt)
    FILE *numer;

    int sections;   //* sections -> ilosc sekcji
    if((sections = atoi(argv[3])) == 0)
    {
        printf("ATOI ERROR -> POTOMEK\n");
        _exit(EXIT_FAILURE);
    }

    //* prosty komunikat wypisujacy PID procesu potomnego
    printf("PID PROCESU POTOMNEGO -> %d\n", getpid());
    sleep(1);

    //! ------------------------- PRZED STREFA KRYTYCZNA -------------------------

    //* program wypisuje komunikat, zasypia i potem opuszcza semafor
    sem_pobierz_wartosc(Semafor, &sem_val);
    printf("PRZED STREFA KRYTYCZNA | WARTOSC SEMAFORA  =  %d | PID -> %d\n", sem_val, getpid());
    sleep(randomNo());

    //! ------------------------- W STREFIE KRYTYCZNEJ -------------------------

    /* 
        * otwierany jest plik do czytania (default: numer.txt)
        * po czym zczytywana jest jego wartosc, zostaje ona inkrementowana tyle razy w ile sekcji ma wejsc proces
        * i wartosc koncowa zostaje wpisana do tego samego pliku.
        * Po wypisaniu komunikatów podnosimy semafor (V(S))
    */

    int i;
    for(i = 1; i <= sections; i++)
    {
        P(Semafor);
        if((numer = fopen(argv[2], "r")) == NULL)
        {
            perror("FOPEN ERROR -> POTOMEK -> READ-MODE\n");
            exit(EXIT_FAILURE);
        }
        if(fscanf(numer, "%d", &numer_value) == EOF)
        {
            printf("FSCANF ERROR -> POTOMEK -> READ-MODE\n");
            exit(EXIT_FAILURE);
        }
        if(fclose(numer) == EOF)
        {
            perror("FCLOSE ERROR -> POTOMEK -> READ-MODE\n");
            exit(EXIT_FAILURE);
        }

        sleep(randomNo());

        printf("\t%d SEKCJA KRYTYCZNA -> ", i);

        sem_pobierz_wartosc(Semafor, &sem_val);
        printf("\tWARTOSC PRZECZYTANA Z PLIKU = %d | WARTOSC SEMAFORA =  %d | PID -> %d\n", numer_value, sem_val, getpid());

        numer_value++; 

        if(i!=sections)
            sleep(randomNo());

        if((numer = fopen(argv[2], "w")) == NULL)
        {
            perror("FOPEN ERROR -> POTOMEK -> READ-MODE\n");
            exit(EXIT_FAILURE);
        }
        if(fprintf(numer, "%d", numer_value) < 0)
        {
            perror("FOPEN ERROR -> POTOMEK -> READ-MODE\n");
            exit(EXIT_FAILURE);
        }
        if(fclose(numer) == EOF)
        {
            perror("FOPEN ERROR -> POTOMEK -> READ-MODE\n");
            exit(EXIT_FAILURE);
        }

        V(Semafor);

        //! ------------------------- PO STREFIE KRYTYCZNEJ -------------------------

        //* zostaje pobierana wartosc semafora i wypisywany komunikat na ekran
        sem_pobierz_wartosc(Semafor, &sem_val);
        printf("PO STREFIE KRYTYCZNEJ | WARTOSC SEMAFORA =  %d | PID -> %d\n", sem_val, getpid());
    }

    _exit(EXIT_SUCCESS);
}