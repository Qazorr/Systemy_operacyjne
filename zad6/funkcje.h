/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

#ifndef FUNKCJE_H   // zapewnia najwyzej jednokrotne wlaczenie tego pliku
#define FUNKCJE_H

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/*
    * sem_stworz -> tworzy nowy semafor
    * sem_otworz -> otwiera wczesniej stworzony semafor
    * sem_pobierz_wartosc -> pobiera wartosc semafora do zmiennej &sval
        * bedacej drugim argumentem wywolania funkcji
    * V -> podnosi semafor
    * P -> opuszcza semafor
    * sem_zamknij -> zamyka wczesniej otwarty semafor
    * sem_usun -> usuwa wczesniej stworzony semafor
*/

sem_t *sem_stworz(char *name, int oflag, mode_t mode, unsigned int value);
sem_t *sem_otworz(char *name, int oflag);
void sem_pobierz_wartosc(sem_t *sem, int *sval);
void V(sem_t *sem);
void P(sem_t *sem);
void sem_zamknij(sem_t *sem);
void sem_usun(char *name);

#endif