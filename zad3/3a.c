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

    int signal_int = atoi(argv[1]);
    int option = atoi(argv[2]);

    if(signal_int == 0 || option == 0)
    {
        printf("ZLE WPROWADZONE ARGUMENTY\n");
        exit(EXIT_FAILURE);
    }

    pid_t Child_PID = fork();       //wywolanie funkcji fork i jednoczesne przechwycenie PID potomka (fork zwraca PID potomka)

    switch(Child_PID)
    {
        case -1:
            perror("FORK ERROR");     //obsluga bledow dla forka
            exit(EXIT_FAILURE);
        case 0:
            setpgid(0,0);
            execl("./3b.x", "3b.x", argv[1], argv[2], NULL);    //wywolanie funkcji execl, ktora wywola program 2a.x oraz za argumenty tego programu poda argv[1] i argv[2]
            perror("EXECL ERROR");    //obsluga bledow dla execl
            exit(EXIT_FAILURE);
        default:
            sleep(1);
            if(kill(-Child_PID,0) == -1){    //obsluga bledow, sprawdzajaca czy istnieje proces
                perror("KILL ERROR, PROCES NIE ISTNIEJE");
                exit(EXIT_FAILURE);
            }
            if(kill(-Child_PID, signal_int) == -1){  //obsluga bledow, sprawdzajaca czy sygnal jest poprawny
                perror("KILL ERROR, ZLY SYGNAL");
                exit(EXIT_FAILURE);
            }
            if(wait(NULL) == -1)
            {
                perror("WAIT ERROR\n");
                exit(EXIT_FAILURE);
            }
            break;
    }

}