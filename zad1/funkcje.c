#include "funkcje.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>


//funkcja wypisujaca dane procesu macierzystego
void wypisz_macierzysty()
{
    printf("Proces macierzysty: UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", getuid(),getgid(),getpid(),getppid(),getpgid(0));
}

//funkcja wypisujaca dane procesu potomnego
void wypisz_potomka()
{
    printf("Potomek: UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", getuid(),getgid(),getpid(),getppid(),getpgid(0));
}