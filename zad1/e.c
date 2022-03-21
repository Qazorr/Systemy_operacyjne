/* 
 Program wypisuje UID, GID, PID, PPID, PGID procesu macierzystego
 po czym wypisuje programy potomne, ktore w tym przypadku staja sie
 liderami grup procesow (PID == PGID).
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
    int PID_potomek;            //zmienna w ktorej pozniej przechowamy PID potomka
    wypisz_macierzysty();       //funkcja z pliku funkcje.c
    for(i=0; i<3; i++)
    {
        switch(fork())
        {   
            case -1:
                perror("ERROR");
                exit(-1);
            case 0:
                PID_potomek = getpid();     //pozyskanie PID potomka, by uzyc go w funkcji setpgid
                setpgid(PID_potomek, 0);    //pierwszy argument odpowiada za to jaka wartosc PGID ma byc ustawiona
                wypisz_potomka();           //funkcja z pliku funkcje.c
                break;
            default:
                wait(NULL);
                break;
        }
    }

    return 0;
}