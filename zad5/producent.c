/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

/*
  argv[1] - plik wejsciowy
  argv[2] - nazwa fifo
  argv[3] - czy wypisac na ekran?
*/
#define PROD_SPACE 16


//randomizacja czasu, ktory bedzie wstawiany do sleepa
int randomNo() {
    srand(time(NULL) ^ getpid());
    return rand() % ((2-1)+1)+1;
}


int main(int argc, char *argv[])
{
    //deskryptory
    int producent, pipe;

    //zmienna ktora decyduje o tym czy informacje zostana wypisane do terminala
    int WRITE_ON_SCREEN;
    //przekonwertowanie na inta + obsluga bledow
    if((WRITE_ON_SCREEN = atoi(argv[3])) == -1)
    {
        printf("ATOI ERROR -> WRITE_ON_SCREEN VARIABLE\n");
        exit(EXIT_FAILURE);
    }

    if((producent = open(argv[1], O_RDONLY, 0644)) == -1 || (pipe = open(argv[2], O_WRONLY, 0644)) == -1)
    {
        perror("OPEN ERROR -> PRODUCENT");
        exit(EXIT_FAILURE);
    }

    //bytes - liczba bitow zwracana przez read(), OS_MESS_PD - on screen message producent - przechowuje wiadomosc wypisana na terminal
    //PD_chunk - jakas czesc ciagu znakow bedzie przechowywana w tej tablicy znakowej, ustawiamy ostatnia pozycje na 0, aby uniknac bledow
    int bytes;
    char OS_MESS_PD[128];
    char PD_chunk[PROD_SPACE+1];
    PD_chunk[PROD_SPACE] = 0;


    //dopoki funkcja read zwraca wartosc rozna od zera(czyli dopoki nie napotka konca pliku)
    //bedzie wykonywane wypisywanie do potoku + do terminala
    while((bytes = read(producent, PD_chunk, sizeof(char)*PROD_SPACE)) != 0)
    {
        //obsluga bledow dla funkcji read()
        if(bytes == -1)
        {
            perror("READ ERROR -> PRODUCENT\n");
            exit(EXIT_FAILURE);
        }
        //Mozliwosc wylaczenia opcji wypisywania na ekran za pomoca zmiany WRITE_ON_SCREEN na 0
        if(WRITE_ON_SCREEN)
        {
            //umieszczenie calej wiadomosci jednej tablicy znakowej (aby nie dzielic write())
            sprintf(OS_MESS_PD, "Wczytano: %s  o rozmiarze <%d> bajtow\n", PD_chunk, bytes);
            //sleep ustawiony z pseudo-losowymi wartosciami
            sleep(randomNo());
            //wypisanie wiadomosci do terminala
            write(STDOUT_FILENO, OS_MESS_PD, sizeof(char)*(strlen(OS_MESS_PD)));
        }
        //wypisanie tablicy znakowej PD_chunk do potoku + obsluga bledow
        if(write(pipe, PD_chunk, sizeof(char)*bytes) == -1)
        {
            perror("WRITE ERROR -> PRODUCENT\n");
            exit(EXIT_FAILURE);
        }
        //wyczyszczenie tablicy znakowej PD_chunk
        memset(PD_chunk, 0, sizeof(PD_chunk));
    }

    //zamykamy deskryptory
    close(producent);
    close(pipe);

    return 0;
}