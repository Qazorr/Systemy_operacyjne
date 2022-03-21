/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

//! W pliku znajduja sie funkcje operujace na pamieci dzielonej

//* plik naglowkowy
#include "shared_mem.h"


int shmemory_otworz(const char *name, int flags, mode_t mode)
{
    int descryptor;

    if((descryptor = shm_open(name, flags, mode)) == -1){
        perror("SHM_OPEN ERROR -> SHMEMORY_OPEN()\n");
        _exit(EXIT_FAILURE);
    }

    return descryptor;
}

void shmemory_ustaw(int fd, off_t lenght)
{
    if(ftruncate(fd, lenght) == -1){
        perror("FTRUNCATE ERROR -> USTAW()\n");
        exit(EXIT_FAILURE);
    }
}

void shmemory_zamknij(int fd)
{
    if(close(fd) == -1){
        perror("CLOSE ERROR -> SHMEMORY_ZAMKNIJ()\n");
        exit(EXIT_FAILURE);
    }
}

void shmemory_unlink(const char *name)
{
    if(shm_unlink(name) == -1)
    {
        perror("SHM_UNLINK ERROR -> SHMEMORY_UNLINK()\n");
        exit(EXIT_FAILURE);
    }
}

void *memory_map(void *addr, size_t lenght, int prot, int flags, int fd, off_t offset)
{
    void *ret;

    if((ret = mmap(addr, lenght, prot, flags, fd, offset)) == MAP_FAILED)
    {
        perror("MMAP ERROR -> MEMORY_MAP()\n");
        exit(EXIT_FAILURE);
    }

    return ret;
}

void memory_unlink_map(void *addr, size_t lenght)
{
    if(munmap(addr, lenght) == -1){
        perror("MUNMAP ERROR -> MEMORY_UNLINK_MAP()\n");
        exit(EXIT_FAILURE);
    }
}

