/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


#ifndef KOLEJKI_H   // zapewnia najwyzej jednokrotne wlaczenie tego pliku
#define KOLEJKI_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <string.h>
#include <time.h>

#define SERWER_NAME "/serwer"
#define MAX_SIZE 128    //wielkosc tablic z wiadomosciami
#define CLIENT_SIZE 64  //wielkosc tablicy z nazwa klienta

//* randomizacja czasu (liczba od 0 do 3)
int randomNo();

/**
 *  * tworzy kolejke
 *  @param name nazwa kolejki
 *  @param oflag flagi z ktora tworzymy kolejke (moga byc laczone suma bitowa '|')
 *  @param mode prawa dostepu
 *  @param attr wskaznik do struktury atrybutow
*/
mqd_t kolejka_stworz(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

/**
 *  * otwiera kolejke
 *  @param name nazwa kolejki
 *  @param oflag flagi z ktora otwieramy kolejke (moga byc laczone suma bitowa '|')
*/
mqd_t kolejka_otworz(const char *name, int oflag);

/**
 *  * zamknij kolejke
 *  @param mqdes deskryptor kolejki
*/
void kolejka_zamknij(mqd_t mqdes);

/**
 *  * usun kolejke
 *  @param name nazwa kolejki
*/
void kolejka_usun(const char *name);

/**
 *  * wyslij do kolejki
 *  @param mqdes deskryptor kolejki do ktorej wysylamy
 *  @param msg_ptr wiadomosc (tablica znakowa)
 *  @param msg_len wielkosc wiadomosci
 *  @param msg_prio priorytet komunikatu
*/
void kolejka_wyslij(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

/**
 *  * odbiera wiadomosc
 *  @param mqdes deskryptor kolejki z ktorej pobieramy
 *  @param msg_ptr tablica do ktorej umieszczamy wiadomosc
 *  @param msg_len dlugosc wiadomosci (>= mq_msgsize)
 *  @param msg_prio priorytet komunikatu
*/
void kolejka_odbierz(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);



#endif