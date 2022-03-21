/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


//biblioteki
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//makra
#define PATH "./"
#define PD_F_NAME "producent.x"
#define KS_F_NAME "konsument.x"
#define WRITE_ON_SCREEN "1"

/*
    argv[1] - wejsciowy z danymi (ciagiem znakow)
    argv[2] - wyjsciowy do ktorego zostana zapisane dane
*/

//pointer ktory uzywamy do wywolania unlinka, ustawiany na argument funkcji main
char *fifo_name = NULL;

typedef void (*sighandler_t)(int);
sighandler_t signal(int sig, sighandler_t handler);

//custom handler do sygnalu
static void handler(int sigNum)
{
    printf("\tSIGNAL INTERRUPT, POTOK ZOSTAL ZAMKNIETY\n");
    exit(EXIT_FAILURE);
}

//uzywane do funkcji atexit()
void ending_sequence()
{
    //wywolanie funkcji unlink() + error handling
    if(unlink(fifo_name) == -1)
    {
        perror("UNLINK ERROR -> ENDING SEQUENCE\n");
        _exit(EXIT_FAILURE);   
    }
    else
    {
        printf("\t    POTOK ZOSTAL POMYSLNIE ZAMKNIETY\n");
    }
}


int main(int argc, char *argv[])
{

    //jesli liczba argumentow bedzie rozna od 3 wystapi blad
    if(argc != 4)
    {
        printf("RUNNING ERROR: URUCHAMIANIE POPRZEZ -> ./executable PLIK_IN PLIK_OUT fifo_name\n");
        exit(EXIT_FAILURE);
    }
    //ustawienie pointera na argv[3] aby obsluzyc unlinka w funkcji atexit
    fifo_name = argv[3];

    //stworzenie fifo (potoku nazwanego)
    if(mkfifo(fifo_name, 0777) == -1)
    {
        perror("MKFIFO ERROR\n");
        _exit(EXIT_FAILURE);
    }
    //uzycie funkcji atexit, ktora usunie potok przy wywolaniu exit()
    if(atexit(ending_sequence)!=0)
    {
        perror("ATEXIT ERROR\n");
        exit(EXIT_FAILURE);
    }
    //sygnal handling dla SIGNAL_INTERRUPT + error handling
    if(signal(SIGINT, handler) == SIG_ERR)
    {
        perror("SIGNAL ERROR\n");
        exit(EXIT_FAILURE);
    }

    int i;

    //stworzenie argumentow dla funkcji execl()
    char pd_exec[32], ks_exec[32];
    sprintf(pd_exec, "%s%s", PATH, PD_F_NAME);
    sprintf(ks_exec, "%s%s", PATH, KS_F_NAME);

    //zmienna zliczajaca ilosc potomkow
    int child_counter;

    for(child_counter = 0; child_counter<2; child_counter++)
    {
        switch(fork())
        {
            case -1:
                //error handling dla forka
                perror("FORK ERROR\n");
                exit(EXIT_FAILURE);
            case 0:
                //sprawienie ze pierwszy program zawsze stanie sie producentem, zas drugi konsumentem
                if(child_counter == 0)
                {
                    //wywolanie funkcji execl(argv[1] - plik wejsciowy, WRITE_ON_SCREEN - mozliwosc wypisania do terminala)
                    //+error handling
                    execl(pd_exec, PD_F_NAME, argv[1], fifo_name, WRITE_ON_SCREEN, NULL);
                    perror("EXECL ERROR -> PRODUCENT\n");
                    _exit(EXIT_FAILURE);
                }            
                else
                {
                    //wywolanie funkcji execl(argv[2] - plik wyjsciowy, WRITE_ON_SCREEN - mozliwosc wypisania do terminala)
                    //+error handling
                    execl(ks_exec, KS_F_NAME, argv[2], fifo_name, WRITE_ON_SCREEN, NULL);
                    perror("EXECL ERROR -> KONSUMENT\n");
                    _exit(EXIT_FAILURE);
                }
                break;

            default:
                break;
        }

    }
    //wywolanie waita dwukrotnie sprawia ze program poczeka az oba z procesow potomnych sie zakoncza + error handling
    for(i = 0; i<2; i++)
    {
        if(wait(NULL) == -1)
        {
            perror("WAIT ERROR\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}