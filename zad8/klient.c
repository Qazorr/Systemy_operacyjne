/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


//biblioteka z funkcjami obslugujacymi kolejki
#include "kolejki.h"

//makro do utworzenia nazwy kolejki klienta (/PID)
#define SLASH "/"

char name[CLIENT_SIZE];  //nazwa kolejki klientas
mqd_t serwer_des;   //deskryptor kolejki serwera
mqd_t klient_des;   //deskryptor kolejki klienta

//funkcja do atexit()
void ending_sequence()
{
    printf("KLIENT SIE ZAKONCZYL...\nNastapilo zamkniecie kolejek oraz ich usuniecie.\n");
    kolejka_zamknij(serwer_des);
    kolejka_zamknij(klient_des);
    kolejka_usun(name);

    sleep(3);
}

int main()
{
    if(atexit(ending_sequence) != 0)
    {
        perror("ATEXIT EROR -> KLIENT -> LINE 31\n");
        exit(EXIT_FAILURE);
    }

    struct mq_attr attr;
    attr.mq_flags = 0;      // Opcje: 0 lub O_NONBLOCK
    attr.mq_maxmsg = 8;     // Maksymalna liczba komunikatow w kolejce
    attr.mq_msgsize = MAX_SIZE;  // Maksymalny rozmiar komunikatu (w bajtach)    
    attr.mq_curmsgs = 0;    // Liczba komunikatow aktualnie w kolejce    

    char wysylam[MAX_SIZE];  //zawiera wiadomosc wysylana do serwera
    char dzialanie[CLIENT_SIZE]; //zawiera dzialanie na dwoch liczbach
    char wynik[MAX_SIZE];    //zawiera wynik pobrany z serwera
    sleep(2);

    //stworzenie nazwy kolejki
    sprintf(name, "%s%d", SLASH, getpid());

    //otworzenie kolejki serwera oraz stworzenie kolejki klienta
    serwer_des = kolejka_otworz(SERWER_NAME, O_WRONLY);
    klient_des = kolejka_stworz(name, O_CREAT | O_EXCL | O_RDONLY, 0644, &attr);

    //wypisanie komunikatow startowych
    printf("\033[1;5;7;30;40m\t\t\tCLIENT %s IS UP AND WAITING FOR INPUT\033[m\n\n", name);
    printf("\033[1;30mNAZWA: %s | DESKRYPTOR: %d | flags = %ld | maxmsg = %ld | msgsize = %ld | curmsgs = %ld\033[m\n\n", name, klient_des, attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    while(fgets(dzialanie, CLIENT_SIZE, stdin))
    {
        //sklejenie wiadomosci do wyslania do serwera
        sprintf(wysylam, "%s %s", name, dzialanie);

        //wyslanie do serwera
        kolejka_wyslij(serwer_des, wysylam, strlen(wysylam)+1, 1);

        sleep(randomNo());

        //odebranie wiadomosci od serwera
        kolejka_odbierz(klient_des, wynik, MAX_SIZE, NULL);

        //wypisanie komunikatu
        printf("OTRZYMALEM ODPOWIEDZ NA NURTUJACE MNIE PYTANIE! \033[1;5;47mODP = %s\033[m. DZIEKUJE!\n", wynik);
    }

    exit(EXIT_SUCCESS);
}