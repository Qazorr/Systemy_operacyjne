/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


#define _POSIX_C_SOURCE 200112L     //define dla funkcji kill
#define _XOPEN_SOURCE 500           //define dla funkcji getpid

//includy
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("ZA MALO ARGUMENTOW\n");
        exit(EXIT_FAILURE);
    }
    
    //przekonwertowanie argumentow na inty
    int signal_int = atoi(argv[1]);
    int option = atoi(argv[2]);

    if(signal_int == 0 || option == 0)  //sprawdzenie czy argumenty zostaly poprawnie wprowadzone, konwertujac je funkcja atoi
    {
        printf("ZLE WPROWADZONE ARGUMENTY\n");
        exit(EXIT_FAILURE);
    }

    if(signal(signal_int, SIG_IGN) == SIG_ERR){         //obsluga bledow dla ignorowania sygnalu przez proces potomny
        perror("SIGNAL ERROR\n");
        exit(EXIT_FAILURE);
    }

    int i;
    pid_t PID_potomkow[6];      //tablica przechowujaca PID potomkow
    int stat_local;

    for(i=0; i<5; i++)
    {
        PID_potomkow[i] = fork();       //wywolanie funkcji fork i jednoczesne przechwycenie PID potomka (fork zwraca PID potomka)
        switch(PID_potomkow[i])
        {
            case -1:
                perror("FORK ERROR");     //obsluga bledow dla forka
                exit(EXIT_FAILURE);
            case 0:
                execl("./3c.x", "3c.x", argv[1], argv[2], NULL);    //wywolanie funkcji execl, ktora wywola program 2a.x oraz za argumenty tego programu poda argv[1] i argv[2]
                perror("EXECL ERROR");    //obsluga bledow dla execl
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }


    for(i=0; i<5; i++) {
        pid_t cpid = waitpid(PID_potomkow[i], &stat_local, 0);
        if(WIFSIGNALED(stat_local)){
            printf("Process PID: %d terminated with signal %d\n", cpid, WTERMSIG(stat_local));
        }
        if(WIFEXITED(stat_local)) {
            printf("\nProcess PID: %d terminated with status: %d", cpid, WEXITSTATUS(stat_local));
        }
    }

    putchar('\n');

}