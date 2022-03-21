/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

//biblioteki
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

/*
    dyrektywy:
    PROD_SPACE odpowiada za wielkosc chunkow ktore beda pobierane do potoku
    KONS_SPACE odpowiada za wielkosc chunkow ktore beda odbierane z potoku
    WRITE_ON_SCREEN odpowiada za ewentualne wypisanie danych do terminala
*/
#define PROD_SPACE 16
#define KONS_SPACE 8
#define WRITE_ON_SCREEN 1

//randomizacja czasu, ktory bedzie wstawiany do sleepa
int randomNo() {
    srand(time(NULL) ^ getpid());
    return rand() % ((2-1)+1)+1;
}


int main(int argc, char *argv[])
{
    //jesli liczba argumentow bedzie rozna od 3 wystapi blad
    if(argc != 3)
    {
        printf("NIEPRAWIDLOWE WYWOLANIE: %s PD_FIlE KS_FILE\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //zmienna przechowujaca PID programu macierzystego
    pid_t M_PID = getpid();

    //filedes[2] - deskryptory | bytes - zmienna, ktora bedzie przechowywac ilosc bitow zwracanych przez read
    int producent, konsument;
    int filedes[2];
    int bytes;

    //tablice przechowujace wiadomosc wypisana na ekran: ON-SCREEN MESSAGE PRODUCENT/KONSUMENT
    char OS_MESS_PD[128], OS_MESS_KS[128];

    //PD - PRODUCENT, KS - KONSUMENT | oba 'chunki' beda przechowywac okreslona ilosc znakow
    char PD_chunk[PROD_SPACE+1], KS_chunk[KONS_SPACE+1];
    PD_chunk[PROD_SPACE] = 0;
    KS_chunk[KONS_SPACE] = 0;

    //uzycie funkcji pipe oraz obsluga bledow
    if(pipe(filedes) == -1)
    {
        perror("PIPE ERROR\n");
        exit(EXIT_FAILURE);
    }

    //otwieranie pliku wejsciowego z opcja READ_ONLY + obsluga bledow
    if((producent = open(argv[1], O_RDONLY, 0644)) == -1)
    {
        perror("OPEN ERROR -> PRODUCENT FILE\n");
        exit(EXIT_FAILURE);
    }

    switch(fork())
    {
        //w przypadku bledu fork zwraca -1 | obsluga bledu dla funkcji fork()
        case -1:
            perror("FORK ERROR\n");
            exit(EXIT_FAILURE);
            break;

        //PROCES POTOMNY -> konsument
        case 0:

            //zamkniecie nieuzywanej czesci potoku
            if(close(filedes[1]) == -1)
            {
                perror("CLOSE ERROR -> KONSUMENT\n");
                exit(EXIT_FAILURE);
            }
            
            //otwieranie pliku wyjsciowego (w przyclearpadku gdy nie istnieje plik)
            if((konsument = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
            {
                perror("OPEN ERROR -> KONSUMENT FILE\n");
                exit(EXIT_FAILURE);
            }

            //dopoki funkcja read zwraca wartosc rozna od zera(czyli dopoki nie napotka konca pliku)
            //bedzie wykonywane wypisywanie do pliku + do terminala
            while((bytes = read(filedes[0], KS_chunk, sizeof(char)*KONS_SPACE)) != 0)
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
                    if(sprintf(OS_MESS_KS, "Zapisano: %s  o rozmiarze <%d> bajtow\n", KS_chunk, bytes) < 0)
                    {
                        perror("SPRINTF ERROR -> KONSUMENT\n");
                        exit(EXIT_FAILURE);
                    }
                    //sleep ustawiony z pseudo-losowymi wartosciami
                    sleep(randomNo());
                    //wypisanie wiadomosci do terminala
                    if(write(STDOUT_FILENO, OS_MESS_KS, sizeof(char)*(strlen(OS_MESS_KS))) == -1)
                    {
                        perror("WRITE ERROR -> KONSUMENT\n");
                        exit(EXIT_FAILURE);
                    }
                }

                //wypisanie KS_chunk do pliku + obsluga bledow
                if(write(konsument, KS_chunk, sizeof(char)*bytes) == -1)
                {
                    perror("WRITE ERROR -> KONSUMENT\n");
                    exit(EXIT_FAILURE);
                }
                //wyczyszczenie tablicy znakowej KS_chunk
                memset(KS_chunk, 0, sizeof(KS_chunk));
            }

            //zamkniecie drugiej czesci potoku + zamkniecie pliku
            if(close(filedes[0]) == -1 || close(konsument) == -1)
            {
                perror("CLOSE ERROR -> KONSUMENT\n");
                exit(EXIT_FAILURE);
            }
            

            break;
        //PROCES MACIERZYSTY -> producent
        default:
            //zamkniecie nieuzywanej czesci potoku
            if(close(filedes[0]) == -1)
            {
                perror("CLOSE ERROR -> PRODUCENT\n");
                exit(EXIT_FAILURE);
            }

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
                    if(sprintf(OS_MESS_PD, "Wczytano: %s  o rozmiarze <%d> bajtow\n", PD_chunk, bytes) == -1)
                    {
                        perror("SPRINTF ERROR -> PRODUCENT\n");
                        exit(EXIT_FAILURE);
                    }
                    //sleep ustawiony z pseudo-losowymi wartosciami
                    sleep(randomNo());
                    //wypisanie wiadomosci do terminala
                    if(write(STDOUT_FILENO, OS_MESS_PD, sizeof(char)*(strlen(OS_MESS_PD))) == -1)
                    {
                        perror("WRITE ERROR -> PRODUCENT -> ON_SCREEN_MSG\n");
                        exit(EXIT_FAILURE);
                    }
                }

                //wypisanie tablicy znakowej PD_chunk do potoku + obsluga bledow
                if(write(filedes[1], PD_chunk, sizeof(char)*bytes) == -1)
                {
                    perror("WRITE ERROR -> PRODUCENT\n");
                    exit(EXIT_FAILURE);
                }
                //wyczyszczenie tablicy znakowej KS_chunk
                memset(PD_chunk, 0, sizeof(PD_chunk));
            }

            //zamkniecie drugiej czesci potoku + zamkniecie pliku
            
            if(close(producent) == -1 || close(filedes[1]) == -1)
            {
                perror("CLOSE ERROR -> PRODUCENT\n");
                exit(EXIT_FAILURE);
            }


            //proces macierzysty bedzie czekal na zakonczenie operacji procesu potomnego + obsluga bledow
            if(wait(NULL) == -1)
            {
                perror("WAIT ERROR -> PRODUCENT\n");
                exit(EXIT_FAILURE);
            }

            break;
    }

    //wypisanie wiadomosci koncowej
    if(getpid() == M_PID)
    {
        char *ENDING_MESSAGE = "\n------------------KONIEC PROGRAMU-----------------\n";
        if(write(STDOUT_FILENO, ENDING_MESSAGE, sizeof(char)*strlen(ENDING_MESSAGE)) == -1)
        {
            perror("WRITE ERROR -> ENDING MESSAGE\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}