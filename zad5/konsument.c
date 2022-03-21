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
  argv[1] - plik wyjsciowy
  argv[2] - nazwa fifo
  argv[3] - czy wypisac na ekran?
*/

//makro definiujace wielkosc pobieranych ciagow znakowych
#define KONS_SPACE 8

//randomizacja czasu, ktory bedzie wstawiany do sleepa
int randomNo() {
    srand(time(NULL) ^ getpid());
    return rand() % ((2-1)+1)+1;
}


int main(int argc, char *argv[])
{
    
    //deskryptory
    int konsument, pipe;

    //zmienna ktora decyduje o tym czy informacje zostana wypisane do terminala
    int WRITE_ON_SCREEN;
    //przekonwertowanie na inta + obsluga bledow
    if((WRITE_ON_SCREEN = atoi(argv[3])) == -1)
    {
        printf("ATOI ERROR -> WRITE_ON_SCREEN VARIABLE\n");
        exit(EXIT_FAILURE);
    }
    //otwarcie dwoch deskryptorow, pipe - otwiera argv[2] czyli nasz potok nazwany zas konsument nasz argv[1] czyli plik wyjsciowy
    if((pipe = open(argv[2], O_RDONLY, 0644)) == -1 || (konsument = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        perror("OPEN ERROR -> KONSUMENT");
        exit(EXIT_FAILURE);
    }


    //bytes - liczba bitow zwracana przez read(), OS_MESS_KS - on screen message konsument - przechowuje wiadomosc wypisana na terminal
    //KS_chunk - jakas czesc ciagu znakow bedzie przechowywana w tej tablicy znakowej, ustawiamy ostatnia pozycje na 0, aby uniknac bledow
    int bytes;
    char OS_MESS_KS[128];
    char KS_chunk[KONS_SPACE+1];
    KS_chunk[KONS_SPACE] = 0;


    //dopoki funkcja read zwraca wartosc rozna od zera(czyli dopoki nie napotka konca pliku)
    //bedzie wykonywane wypisywanie do potoku + do terminala
    while((bytes = read(pipe, KS_chunk, sizeof(char)*KONS_SPACE)) != 0)
    {
        //obsluga bledow dla funkcji read()
        if(bytes == -1)
        {
            perror("READ ERROR -> KONSUMENT\n");
            exit(EXIT_FAILURE);
        }
        //Mozliwosc wylaczenia opcji wypisywania na ekran za pomoca zmiany WRITE_ON_SCREEN na 0
        if(WRITE_ON_SCREEN)
        {
            //umieszczenie calej wiadomosci jednej tablicy znakowej (aby nie dzielic write())
            sprintf(OS_MESS_KS, "Zapisano: %s  o rozmiarze <%d> bajtow\n", KS_chunk, bytes);
            //sleep ustawiony z pseudo-losowymi wartosciami
            sleep(randomNo());
            //wypisanie wiadomosci do terminala
            if(write(STDOUT_FILENO, OS_MESS_KS, sizeof(char)*(strlen(OS_MESS_KS))) == -1)
            {
                perror("WRITE ERROR -> KONSUMENT\n");
                exit(EXIT_FAILURE);
            }
        }
        //wypisanie tablicy znakowej KS_chunk do potoku + obsluga bledow
        if(write(konsument, KS_chunk, sizeof(char)*bytes) == -1)
        {
            perror("WRITE ERROR -> KONSUMENT\n");
            exit(EXIT_FAILURE);
        }
        //wyczyszczenie tablicy znakowej KS_chunk
        memset(KS_chunk, 0, sizeof(KS_chunk));
    }

    //zamykamy deskryptory
    close(konsument);
    close(pipe);


    return 0;
}