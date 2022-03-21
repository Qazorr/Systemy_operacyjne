/* 
 Program wypisuje UID, GID, PID, PPID, PGID procesu macierzystego,
 dodatkowo wyswietla informacje o przekierowaniu drzewa z komendy
 systemowej pstree -p do pliku drzewo.txt.
 Po czym wypisuje w/w ID potomków, wyszukuje proces macierzysty,
 konczy go a reszte procesow usypia, po to zeby po wywolaniu komendy
 pstree -p bylo widoczne kto zaadoptowal te procesy.
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
    int PID = getpid();        //zmienna przechowujaca PID glownego/macierzystego procesu
    printf("W pliku drzewo.txt znajduja sie przekierowane dane komendy pstree -p\n");
    wypisz_macierzysty();
    
    for(i=0; i<3; i++)
    {
        switch(fork())
        {   
            case -1:
                perror("ERROR");    //obsluga bledow
                exit(-1);
            case 0:
                sleep(1);
                wypisz_potomka();   //funkcja z pliku funkcje.c
                break;
            default:
                break;
        }
    }
    if (getpid() == PID)    //jesli aktualnie pobrane PID (funkcja getpid) bedzie rowne glownemu, konczymy proces
        return 0;
    else                    //w przeciwnym wypadku usypiamy procesy (potomne)
        sleep(20);


    return 0;
}
