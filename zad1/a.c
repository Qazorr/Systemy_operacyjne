/* 
 Program wypisuje UID, GID, PID, PPID, PGID procesu.
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

int main()
{
    //funkcja wypisujaca proces macierzysty, ktora znajduje sie w pliku funkcje.c
    wypisz_macierzysty();

    return 0;
}