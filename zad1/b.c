/* 
 Program wypisuje UID, GID, PID, PPID, PGID procesu macierzystego
 po czym wypisuje 7 potomnych procesow utworzonych przez funkcje fork.
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


//wlasne funkcje
#include "funkcje.h"

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int i;

    wypisz_macierzysty(); //funkcja z pliku funkcje.c
    for(i=0; i<3; i++)
    {
        switch(fork())
        {   
            case -1:
                perror("ERROR");    //w przypadku bledu forka zostanie wypisany error
                exit(-1);
            case 0:
                wypisz_potomka();   //funkcja z pliku funkcje.c
                break;
            default:
                wait(NULL);
                break;
        }
    }

    return 0;
}