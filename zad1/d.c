/* 
 Program wypisuje UID, GID, PID, PPID, PGID procesu macierzystego
 po czym wypisuje potomkow pokoleniami, plik z rozrysowanym drzewem
 genealogicznym znajduje sie w pliku drzewo_genealogiczne.txt.
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
    wypisz_macierzysty();   //funkcja z pliku funkcje.c
    for(i=0; i<3; i++)
    {
        switch(fork())
        {   
            case -1:
                perror("ERROR");    //obsluga bledow
                exit(-1);
            case 0:
                wypisz_potomka();   //funkcja z pliku funkcje.c
                sleep(1);
                break;
            default:
                // sleep(1);
                break;
        }
    }
    sleep(4);

    return 0;
}