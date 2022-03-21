/* 
 Program wypisuje UID, GID, PID, PPID, PGID procesu macierzystego.
 Po czym wypisywane sa te same ID procesu potomnego tylko i wylacznie
 od procesu macierzystego. PID procesu macierzystego powinno byc PPID
 kazdego procesu potomnego.
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 Stworzenie makra, ktorego zadaniem jest zastapienie PATH, uzytego w funkcji sprintf
 na ./ symbol potrzebny do uruchomienia programu 
*/
#define PATH "./"

int main(int argc, char* argv[])
{
    int i;
    char pathName[128]; //stworzenie tablicy znakowej w ktorej bedzie przechowywana nazwa pliku executable wraz z ./ na poczatku

    //wywolanie funkcji sprintf laczacej napisy (./ laczy sie z argumentem funkcji main podawanym przy wywolaniu programu)
    sprintf(pathName, "%s%s", PATH, argv[1]);   

    printf("\nProces macierzysty: UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", getuid(),getgid(),getpid(),getppid(),getpgid(0));
    for(i=0; i<3; i++)
    {
        switch(fork())
        {   
            case -1:
                perror("FORK ERROR");    //w przypadku bledu forka zostanie wypisany error
                exit(-1);
            case 0:
                execl(pathName, argv[1], NULL); //wywolanie funkcji execl
                perror("EXEC ERROR");   //obsluga bledu funkcji execl
                exit(EXIT_FAILURE);
            default:
            if(wait(NULL) == -1)
            {
                perror("WAIT ERROR\n");
                exit(EXIT_FAILURE);
            }                
            break;
        }
    }

    return 0;
}