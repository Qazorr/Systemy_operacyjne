/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

//! W pliku znajduja sie funkcje operujace na semaforach

//* plik naglowkowy
#include "semaphores.h"

sem_t *sem_stworz(char *name, int oflag, mode_t mode, unsigned int value)
{
    sem_t *S;

    if((S = sem_open(name, oflag, mode, value)) == SEM_FAILED)
    {
        perror("SEM_CREATE ERROR -> SEM_OPEN\n");
        _exit(EXIT_FAILURE);
    }

    return S;
}
sem_t *sem_otworz(char *name, int oflag)
{
    sem_t *S;

    if((S = sem_open(name, oflag)) == SEM_FAILED)
    {
        perror("SEM_OTWORZ ERROR -> SEM_OPEN\n");
        exit(EXIT_FAILURE);
    }

    return S;
}
void sem_pobierz_wartosc(sem_t *sem, int *sval)
{
    if(sem_getvalue(sem, sval) == -1)
    {
        perror("SEM_POBIERZ_WARTOSC ERROR -> SEM_GETVALUE\n");
        exit(EXIT_FAILURE);
    }
}
void V(sem_t *sem)
{
    if(sem_post(sem) == -1)
    {
        perror("V ERROR -> SEM_POST\n");
        exit(EXIT_FAILURE);
    }
}
void P(sem_t *sem)
{
    if(sem_wait(sem) == -1)
    {
        perror("P ERROR -> SEM_WAIT\n");
        exit(EXIT_FAILURE);
    }
}
void sem_zamknij(sem_t *sem)
{
    if(sem_close(sem) == -1)
    {
        perror("SEM_ZAMKNIJ ERROR -> SEM_CLOSE\n");
        exit(EXIT_FAILURE);
    }
}
void sem_usun(char *name)
{
    if(sem_unlink(name) == -1)
    {
        perror("SEM_USUN ERROR -> SEM_UNLINK\n");
        exit(EXIT_FAILURE);
    }
}