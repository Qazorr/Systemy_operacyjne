/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>

extern const char * const sys_siglist[];        //zmienna extern przechowujaca nazwy sygnalow

typedef void (*sighandler_t)(int);
sighandler_t signal(int sig, sighandler_t handler);

//funkcja, ktora zamienia male litery na duze tablicy znakowej, uzywana w custom handlerze
void do_duzych(char *c)
{
    while (*c)
    {
        *c = toupper(*c);
        c++;        
    }
}
//custom handler, ktory wypisuje numer oraz nazwe sygnalu (wielkimi literami)
static void handler(int signal_number)
{
    char nazwa[32];     //stworzenie tablicy znakowej w ktorej bede przechowywal nazwe sygnalu
    strcpy(nazwa, sys_siglist[signal_number]);  //skopiowanie sygnalu o indeksie signal_number to tablicy znakowej "nazwa"
    do_duzych(nazwa);   //przekonwertowanie nazwy sygnalu na wielkie litery
    printf("NUMER SYGNALU = %d\t", signal_number);
    printf("NAZWA SYGNALU: SIG%s\n", nazwa);
}

int main(int argc, char* argv[])    //1 argument funkcji main odpowiada za ilosc wprowadzonych argumentow, drugi odpowiada za przechowanie tych argumentow
{
    if(argc != 3)   //jesli liczba argumentow wywolanych razem z programem executable nie bedzie rowna 3, nastapi blad
    {
        printf("ZA MALO ARGUMENTOW\n");
        exit(EXIT_FAILURE);
    }

    int signal_int, option;

    //przekonwertowanie argumentow na inty
    signal_int = atoi(argv[1]);
    option = atoi(argv[2]);

    if(signal_int == 0 || option == 0)
    {
        printf("ZLE WPROWADZONE ARGUMENTY\n");
        exit(EXIT_FAILURE);
    }

    printf("PID = %d\n", getpid());     //wypisanie PID procesu, po to zeby nie szukac go z poziomu powloki
    switch (option)
    {
    case 1:
        printf("OPERACJA DOMYSLNA\n");
        if(signal(signal_int, SIG_DFL) == SIG_ERR){         //obsluga bledow
            perror("SIGNAL ERROR\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 2:
        printf("IGNOROWANIE\n");    
        if(signal(signal_int, SIG_IGN) == SIG_ERR){         //obsluga bledow
            perror("SIGNAL ERROR\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 3:
        printf("WLASNA OBSLUGA SYGNALU\n");
        if(signal(signal_int, handler) == SIG_ERR){         //obsluga bledow
            perror("SIGNAL ERROR\n");
            exit(EXIT_FAILURE);
        }
        break;
    default:
        printf("PODANO NIEWLASCIWA OPCJE\n");               //jesli zostanie wprowadzona niewlasciwa opcja nastapi blad
        exit(EXIT_FAILURE);
    }

    if(pause() != -1)        //funkcja pause sprawia ze proces czeka na sygnal
    {
        perror("PAUSE ERROR\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
