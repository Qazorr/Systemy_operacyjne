#include "kolejki.h"

#define SLASH "/"
#define TEST_TIMES 10

char name[CLIENT_SIZE];
mqd_t serwer_des;
mqd_t klient_des;

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
    srand(time(NULL) ^ getpid());

    atexit(ending_sequence);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 8;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;


    char wysylam[MAX_SIZE];
    char dzialanie[CLIENT_SIZE];
    char wynik[MAX_SIZE];
    sleep(2);
    sprintf(name, "%s%d", SLASH, getpid());

    printf("\033[1;5;7;30;40m\t\t\tCLIENT %s IS UP AND WAITING FOR INPUT\033[m\n\n", name);

    serwer_des = kolejka_otworz(SERWER_NAME, O_WRONLY);
    klient_des = kolejka_stworz(name, O_CREAT | O_EXCL | O_RDONLY, 0644, &attr);

    printf("\033[1;30mNAZWA: %s | DESKRYPTOR: %d | flags = %ld | maxmsg = %ld | msgsize = %ld | curmsgs = %ld\033[m\n\n", name, klient_des, attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    int l1 = 0, l2 = 0;
    char O[4] = {'+', '-', '*', '/'};
    char operacja;
    int counter = 0;
    while(1)
    {
        l1 = (rand() % 1000);
        l2 = (rand() % 1000)+1;
        operacja = O[(rand() % 4)];

        sprintf(dzialanie, "%d %c %d", l1, operacja, l2);

        printf("%s\n", dzialanie);

        sprintf(wysylam, "%s %s", name, dzialanie);

        kolejka_wyslij(serwer_des, wysylam, strlen(wysylam)+1, 1);

        sleep(randomNo());

        kolejka_odbierz(klient_des, wynik, MAX_SIZE, NULL);

        printf("OTRZYMALEM ODPOWIEDZ NA NURTUJACE MNIE PYTANIE! \033[1;5;47mODP = %s\033[m. DZIEKUJE!\n", wynik);
        counter++;
        if(counter == TEST_TIMES)
            break;
    }

    exit(EXIT_SUCCESS);
}