/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

#include "kolejki.h"


int randomNo()
{
    return (rand() % 3);
}

mqd_t kolejka_stworz(const char *name, int oflag, mode_t mode, struct mq_attr *attr)
{
    mqd_t deskryptor;

    if((deskryptor = mq_open(name, oflag, mode, attr)) == (mqd_t)-1)
    {
        perror("KOLEJKA_STWORZ ERROR -> MQ_OPEN()\n");
        _exit(EXIT_FAILURE);
    }

    return deskryptor;
}

mqd_t kolejka_otworz(const char *name, int oflag)
{
    mqd_t deskryptor;

    if((deskryptor = mq_open(name, oflag)) == (mqd_t)-1)
    {
        perror("KOLEJKA_OTWORZ ERROR -> MQ_OPEN()\n");
        exit(EXIT_FAILURE);
    }

    return deskryptor;

}

void kolejka_zamknij(mqd_t mqdes)
{
    if(mq_close(mqdes) == -1)
    {
        perror("KOLEJKA_ZAMKNIJ ERROR -> MQ_CLOSE()\n");
        exit(EXIT_FAILURE);
    }
}

void kolejka_usun(const char *name)
{
    if(mq_unlink(name) == -1)
    {
        perror("KOLEJKA_USUN ERROR -> MQ_UNLINK()\n");
        exit(EXIT_FAILURE);
    }
}

void kolejka_wyslij(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio)
{
    if(mq_send(mqdes, msg_ptr, msg_len, msg_prio) == -1)
    {
        perror("KOLEJKA_WYSLIJ ERROR -> MQ_SEND()\n");
        exit(EXIT_FAILURE);
    }
}

void kolejka_odbierz(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio)
{
    if(mq_receive(mqdes, msg_ptr, msg_len, msg_prio) == -1)
    {
        perror("KOLEJKA_ODBIERZ ERROR -> MQ_RECEIVE()\n");
        exit(EXIT_FAILURE);
    }
}