/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

#ifndef SHARED_MEM_H   // zapewnia najwyzej jednokrotne wlaczenie tego pliku
#define SHARED_MEM_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define NELE 20 //@param NELE rozmiar elementu bufora (jednostki towaru w bajtach)
#define NBUF 5 //@param NBUF liczba elementow bufora

/**
 * * stworz/otworz pamiec dzielona
 * @param name nazwa pamieci dzielonej
 * @param flags flagi z ktora tworzymy/otwieramy pamiec dzielona (mozliwa suma bitowa "|")
 * @param mode prawa dostepu
*/
int shmemory_otworz(const char *name, int flags, mode_t mode);
/**
 * * stworz/otworz pamiec dzielona
 * @param fd deskryptor do pamieci dzielonej
 * @param flags dlugosc na jaka chcemy ustawic pamiec
*/
void shmemory_ustaw(int fd, off_t lenght);
/**
 * * zamknij pamiec dzielona
 * @param fd deskryptor do pamieci dzielonej
*/
void shmemory_zamknij(int fd);
/**
 * * usun pamiec dzielona
 * @param name nazwa pamieci dzielonej
*/
void shmemory_unlink(const char *name);
/**
 * * odwzoruj w pamieci
 * @param addr adres poczatkowy odwzorowania (ustaw NULL jesli ma byc dobrany przez jadro systemu)
 * @param lenght dlugosc odwzorowanego obszaru
 * @param prot ochrona pamieci
 * @param flags opcje odwzorowania pamieci
 * @param fd deskryptor do pamieci dzielonej
 * @param offset miejsce od ktorego jest liczona pamiec (zwykle 0)
*/
void *memory_map(void *addr, size_t lenght, int prot, int flags, int fd, off_t offset);
/**
 * * usun odwzorowanie w pamieci
 * @param addr adres obszaru
 * @param lenght dlugosc odwzorowanego obszaru
*/
void memory_unlink_map(void *addr, size_t lenght);


#endif