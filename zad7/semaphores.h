/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

#ifndef SEMAPHORES_H   // zapewnia najwyzej jednokrotne wlaczenie tego pliku
#define SEMAPHORES_H

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/** 
 * * tworzy nowy semafor
 * @param name nazwa semafora
 * @param oflag flagi z ktora tworzymy semafor (mozliwe laczenie poprzez sume bitowa "|")
 * @param mode  prawa dostepu
 * @param value wartosc poczatkowa
*/
sem_t *sem_stworz(char *name, int oflag, mode_t mode, unsigned int value);

/**
 * * otwiera semafor o nazwie name, z flaga oflag
 * @param name nazwa semafora
 * @param oflag flagi z ktora otwieramy semafor (mozliwe laczenie poprzez sume bitowa "|")
*/
sem_t *sem_otworz(char *name, int oflag);

/**
 * * pobiera wartosc z semafora
 * @param sem pointer do semafora z ktorego chcemy pobrac wartosc
 * @param sval zmienna do ktorej chcemy pobrac value semafora
*/
void sem_pobierz_wartosc(sem_t *sem, int *sval);

/**
 * * podnosi semafor sem
 * @param sem semafor ktory chcemy podniesc
*/
void V(sem_t *sem);

/**
 * * opuszcza semafor sem
 * @param sem semafor ktory chcemy opuscic
*/
void P(sem_t *sem);

/**
 * * zamyka semafor sem
 * @param sem semafor ktory chcemy zamknac
*/
void sem_zamknij(sem_t *sem);

/**
 * * usuwa semafor
 * @param name nazwa semafora ktory chcemy usunac
*/
void sem_usun(char *name);

#endif