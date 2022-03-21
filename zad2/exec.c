/* 
 Program wypisuje UID, GID, PID, PPID, PGID procesu potomnego.
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("Potomek: UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", getuid(),getgid(),getpid(),getppid(),getpgid(0));
    return 0;
}