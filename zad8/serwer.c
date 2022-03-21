/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


//biblioteka z funkcjami do obslugi kolejek
#include "kolejki.h"

//@param des deskryptor do kolejki
mqd_t des;

//handler do sygnalu sigint
static void handler(int sigNum)
{
    printf("\nWYWOLANIE SIGINT...\nKonczenie programu w toku...\n");
    sleep(3);
    exit(EXIT_SUCCESS);
}

//funkcja do atexit
void ending_sequence()
{   
    kolejka_zamknij(des);
    kolejka_usun(SERWER_NAME);
    printf("=====================DONE========================\n");
}

//sprawdz jaki jest znak i wykonaj dzialanie
double oblicz(double a, char o, double b)
{
    switch(o)
    {
        case '+':
            return a+b;
        case '-':
            return a-b;
        case '*':
            return a*b;
        case '/':
            return a/b;
        default:
            printf("INVALID OPERATION\n");
            return 0;
    }
}

int main()
{   

    struct mq_attr attr;
    attr.mq_flags = 0;        // Opcje: 0 lub O_NONBLOCK
    attr.mq_maxmsg = 8;       // Maksymalna liczba komunikatow w kolejce
    attr.mq_msgsize = MAX_SIZE;    // Maksymalny rozmiar komunikatu (w bajtach)
    attr.mq_curmsgs = 0;      // Liczba komunikatow aktualnie w kolejce

    //ustawienie obslugi sygnalu na signal interrupt
    if(signal(SIGINT, handler) == SIG_ERR)
    {
        perror("SIGNAL ERROR -> LINE 59\n");
        exit(EXIT_FAILURE);
    }

    //wywolanie atexit z funkcja ending_sequence() -> line 23
    if(atexit(ending_sequence) != 0)
    {
        perror("ATEXIT ERROR -> SERWER -> LINE 66\n");
        exit(EXIT_FAILURE);
    }

    char klient[CLIENT_SIZE];    //przechowuje nazwe klienta /PID
    char otrzymane[MAX_SIZE];    //przechowuje wiadomosc od klienta
    char dostarczam[MAX_SIZE];    //przechowuje wiadomosc wysylana do klienta
    char operacja;      //znak operacji (dzialania) jaka ma zostac uzyta
    int klient_des;     //deskryptor do kolejki klienta
    double pierwsza,druga;     //@param pierwsza pierwsza liczba   @param druga druga liczba
    double wynik;     //przechowuje wynik dzialania operacji na dwoch liczbach

    //@param des deskryptor kolejki serwera
    des = kolejka_stworz(SERWER_NAME, O_CREAT | O_EXCL | O_RDONLY, 0644, &attr);

    //komunikaty poczatkowe
    printf("\033[1;5;7;30;40m\t\t\t\tSERVER IS UP AND WAITING FOR MESSAGES\n\n");
    printf("\033[m");

    printf("\033[1;37mNAZWA KOLEJKI: %s | DESKRYPTOR: %d | flags = %ld | maxmsg = %ld | msgsize = %ld | curmsgs = %ld\033[m\n\n", SERWER_NAME, des, attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
    
    while(1)
    {
        //pobierz z kolejki wiadomosc od klienta
        kolejka_odbierz(des, otrzymane, MAX_SIZE, NULL);

        //wypisanie wiadomosci
        printf("ODEBRALEM WIADOMOSC: \033[1;47m%s\033[m", otrzymane);

        //rozbicie wiadomosci na 4 wartosci
        sscanf(otrzymane, "%s %lg %c %lg", klient, &pierwsza, &operacja, &druga);

        //obliczenie dzialania na dwoch liczbach
        wynik = oblicz(pierwsza,operacja,druga);
        sleep(randomNo());

        //wypisanie wyniku
        printf("ZWRACAM: \033[1;47m%g\033[m DO KLIENTA: %s | PROSZE BARDZO!\n", wynik, klient);

        //sklejenie wyniku do tablicy znakowej
        sprintf(dostarczam, "%.5g", wynik);

        //otworz kolejke konsumenta, wyslij wiadomosc, zamknij kolejke
        klient_des = kolejka_otworz(klient, O_WRONLY);
        kolejka_wyslij(klient_des, dostarczam, strlen(dostarczam)+1, 1);
        kolejka_zamknij(klient_des);
    }

    exit(EXIT_SUCCESS);
}