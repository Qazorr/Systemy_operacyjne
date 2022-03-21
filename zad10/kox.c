/*
    ALGORYTM PIEKARNI
    Na poczatku tworze 2 tablice globalne
    NUM - tablica ktora przechowuje numer watku / miejsce w kolejce - wartosci startowe to 0
    ACCESS - tablica ktora mowi o tym czy dany watek otrzymal swoj numer - wartosci startowe to 0 (0 - false, 1 - true)

    =================FUNKCJA BLOKUJACA====================
    ARGUMENTY : ID WATKU
    Na poczatku ustawiamy wartosc ACCESS naszego watku na 1 (true). Nastepnie przydzielamy mu kolejny numer w kolejce
    czyli szukamy maksymalnej wartosci w naszej tablicy NUM a nastepnie inkrementujemy ta wartosc o 1 i przypisujemy ja 
    naszemu watkowi. Nastepnie iterujemy przez wszystkie watki programu:
    j = 0 .. n - 1 gdzie n to liczba watkow
    while(ACCESS[j]); - czekaj dopoki watek j otrzyma swoj numer
    while(NUM[j] != 0 && (NUM[j], j) < (NUM[i], i)) - poczekaj dopoki wszystkie watki z mniejszym numerem albo z 
    takim samym numerem i mniejszym indeksem zakoncza swoja prace

    =================FUNKCJA ODBLOKUJACA==================
    ARGUMENTY : ID WATKU
    NUM[ID WATKU] = 0 - ustawiamy wartosc watku na 0, czyli przerywamy dzialanie funkcji blokujacej tzn.
    petla while(NUM[j]..... przeskoczy na kolejny indeks
*/




// Biblioteki
#define _REENTRANT
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// Zmienne preprocesora do drukowania informacji o watkach
#define DRUKOWANIE_UTWORZONE_WATKI 4
#define DRUKOWANIE 7
#define DRUKOWANIE_PRACA 3
#define DRUKOWANIE_LICZNIK 1

// Licznik globalny
int licznikGlobalny = 0;

// Ilosc sekcji krytycznych zdefiniowanych przez uzytkownika
int iloscSekcji, iloscWatkow;

// Zmienne dzielone wykluczajace watki
volatile int *NUM, *ACCESS;

// Funkcja realizujaca wykluczanie watkow
void * funkcjaWykluczajaca(void *);

// Funkcja czyszczaca ekran i witajaca uzytkownika
void powitanie(void);

// Funkcja zmieniajaca biezaca linie w terminalu
void gotoxy(int k, int w);

// Funkcja tworzaca template dla watkow
void template(void);

// Zablokuj dostep do sekcji krytycznej
void zablokujDostep(int WATEKID);

// Odblokuj dostep do sekcji krytycznej
void odblokujDostep(int WATEKID);

// Zwroc maksymalna wartosc watkow
int max();

int main(int argc, char *argv[]) {
    // Sprawdzenie poprawnosci argumentow podanych przez uzytkownika
    if(argc != 3 || atoi(argv[1]) == 0 || atoi(argv[1]) == 0) {
        printf("Error: Sposob uruchomienia: %s liczba_watkow liczba_sekcji\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Ilosc sekcji krytycznych podana przez uzytkownika
    iloscSekcji = atoi(argv[1]);

    // Ilosc watkow podana przez uzytkownika
    iloscWatkow = atoi(argv[2]);

    // Ustaw zmienne dzielone do wykluczania
    NUM = (volatile int *)calloc(iloscWatkow, sizeof(volatile int));
    ACCESS = (volatile int *)calloc(iloscWatkow, sizeof(volatile int));

    // Powitaj uzytkownika
    powitanie();

    // Tworzenie watkow
    int i, errCode;

    // Tworzenie tablicy do przechowywania indentyfikatorow watkow
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*iloscWatkow);

    for(i=1; i<=iloscWatkow; i++) {
        // Stworz watek, jezeli wystapil blad, zakoncz dzialanie programu
        if((errCode = pthread_create(&threads[i-1], NULL, funkcjaWykluczajaca, (void*)&i))) {
            printf("Error: blad funkcji pthread_create %d [MAIN]\n", errCode);
            exit(EXIT_FAILURE);
        }
        else {
            // Idz w miejsce do wiersza i+DRUKOWANIE_UTWORZONE_WATKI i wyswietl informacje o utworzonym watku
            gotoxy(i+DRUKOWANIE_UTWORZONE_WATKI, 0);
            printf(" [\033[0;32m%d\033[0m] Utworzono watek -> jego ID: \033[0;31m%ld\033[0m\n", i, threads[i-1]);
            usleep(100);
        }
    }

    // Idz do iloscWatkow+DRUKOWANIE-1 i wydrukuj licznik
    gotoxy(iloscWatkow+DRUKOWANIE-1, 0);
    printf(" =============================================== \n");
    printf("                   Licznik: 0\n");
    printf(" =============================================== ");

    // Wyswietl template dla watkow z sekcjami ktytycznymi i prywatnymi
    template();

    for(i=0; i<iloscWatkow; i++) {
        // Poczekaj na thread[i], w przypadku bledu zakoncz dzialanie programu
        if((errCode = pthread_join(threads[i], NULL))) {
            printf("Error: blad funkcji pthread_join %d [MAIN]\n", errCode);
            exit(EXIT_FAILURE);
        }
        
    }

    // Po zakonczeniu dzialania wszystkich watkow, idz do ostantiego wiersza i wyswietl informacje o poprawnosci licznika globalnego
    gotoxy(iloscWatkow+DRUKOWANIE+iloscWatkow+DRUKOWANIE_UTWORZONE_WATKI+1, 0);
    printf(" Ilosc watkow * ilosc sekcji: %d,  Licznik: %d\n", iloscSekcji*iloscWatkow, licznikGlobalny);
    return EXIT_SUCCESS;
}

// Implementacja funkcji do blokowania sekcji krytycznej
void zablokujDostep(int WATEKID) {
    int j;
    ACCESS[WATEKID] = 1;
    NUM[WATEKID] = max() + 1;
    ACCESS[WATEKID] = 0;

    for(j=0; j<iloscWatkow; j++) {
        while(ACCESS[j]);
        // dopoki num[j] != 0 i (num[j], j) < (num[i], i)
        while(NUM[j] != 0 && ( NUM[j] < NUM[WATEKID] || (NUM[j] == NUM[WATEKID] && j < WATEKID)));
    }
}

// Implementacja funkcji do odblokowania sekcji krytycznej
void odblokujDostep(int WATEKID) {
    NUM[WATEKID] = 0;
}

int max() {
    int i;
    volatile int maxV = 0;
    for(i=0; i<iloscWatkow; i++) {
        if(NUM[i] > maxV)
            maxV = NUM[i];
    }
    return maxV;
}

// Implementacja funkcji zmieniajacej linie
void gotoxy(int w, int k) {
    printf("\033[%d;%dH\033[2K", w, k);
}

// Implementacja funkcji witajacej
void powitanie(void) {
    gotoxy(0,0);
    printf("\033[2J");
    printf("\033[5;7m");     // Wlacz migotanie i odwrotny obraz
    printf(" Wzajemne wykluczanie z wykorzystaniem algorytmu piekarni\n");
    printf(" Ilosc watkow: %d, ilosc sekcji: %d  \n", iloscWatkow, iloscSekcji);
    printf(" Nacisnij klawisz [Enter], aby wystartowac! ");
    getchar();
    printf("\033[0m");       // Wroc do domyslnego trybu wyswietlania
}

// Implementacja funkcji tworzacej template
/*
    STREFA PRYWATNA     STREFA KRYTYCZNA    STAN
    WATEK ID            WATEK ID            W TRAKCIE / ZAKONCZONY
*/
void template(void) {
    gotoxy(iloscWatkow+DRUKOWANIE+3, 0);
    printf("\tSEKCJA PRYWATNA\t\t\tSEKCJA KRYTYCZNA\n");
}

// Implementacja funkcji wykluczajacej
void * funkcjaWykluczajaca(void * arg) {
    // Dla kazdego procesu wypisz n sekcji krytycznych i prywatnych zdefiniowanych przez uzytkownika
    int i;
    srand(123);
    int licznik = 0;
    int argV = *(int*)arg;
    sleep(1);
    for(i=0; i<iloscSekcji; i++) {
        // SEKCJA PRYWATNA
            gotoxy(iloscWatkow+DRUKOWANIE+argV+DRUKOWANIE_PRACA, 0);
            printf("   \033[0;31mWatek nr. %d, sekcja nr. %d \033[0m\n", argV, i);
            sleep(rand()%2+1);
        // SEKCJA PRYWATNA
       
        // Zablokuj mutex, tylko jeden watek ma dostep do sekcji krytycznej
        
        zablokujDostep(argV-1);
        
        // SEKCJA KRYTYCZNA
            // Drukowanie informacji o stanie sesji krytycznej watku
            gotoxy(iloscWatkow+DRUKOWANIE+argV+DRUKOWANIE_PRACA, 37);
            printf("\033[0;7mWatek nr. %d, sekcja nr. %d\033[0m\n", argV, i);
            // Inkrementacja licznika
            licznik = licznikGlobalny;
            licznik ++; 

            // Losowe uspienie watku
            sleep(rand()%2+1);
            
            // Ustawienie globalnego licznika
            licznikGlobalny = licznik;

            printf("\033[%d;%dH\033[2K", iloscWatkow+DRUKOWANIE, 0);
            printf("                   Licznik: %d", licznikGlobalny);
            fflush(stdout);
        // SEKCJA KRYTYCZNA
        odblokujDostep(argV-1);
    }
    printf("\033[%d;%dH\033[2K", iloscWatkow+argV+DRUKOWANIE+DRUKOWANIE_PRACA, 0);
    printf("   \033[0;32mWatek nr. %d, ZAKONCZONO \033[0m\n", argV);
    fflush(stdout);
    pthread_exit(NULL);
    return NULL;
}