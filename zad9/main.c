//wymagany define
#define  _REENTRANT

//biblioteki
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define PRINT_HELPER 4 //zmienna pomagajaca w kosmetycznych rzeczach

int global = 0;     //licznik globalny
int ilosc_sekcji = 0;   //ilosc sekcji podawanych jako argument
int ilosc_watkow = 0;   //ilosc watkow podawanych jako argument

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   //mutex do synchronizacji watkow

//zwraca losowa liczbe (uzywane do sleepa)
int randomNo(){
    return rand()%2+1;
}

//przejdz do wiersza y, kolumny x i wyczysc wiersz
void gotoxy(unsigned x, unsigned y)
{
    printf("\033[%d;%dH\033[2K", y, x);
}

//wypisz index (uzywane przy wypisywaniu watkow)
void print_index(int index)
{
    printf("[\033[32m%d\033[m] ", index);
}

//wypisywanie licznika, najpierw przechodzimy do linii w ktorej znajduje sie licznik, czyscimy ja, zapisujemy nowy licznik
void print_licznik()
{
    gotoxy(30, ilosc_watkow + 3);
    printf("\n----------------\033[38;5;135mLICZNIK = %d\033[m----------------\n", global);
}

//ekran startowy programu
void start()
{
    gotoxy(0,0);
    printf("\033[5;7m");     // Wlacz migotanie i odwrotny obraz
    printf("Uruchomiono program z iloscia sekcji = %d | iloscia watkow = %d\n", ilosc_sekcji, ilosc_watkow);
    printf("    ---- Nacisnij klawisz [Enter], aby wystartowac! ----   ");
    printf("\033[0m");       // Wroc do domyslnego trybu wyswietlania
    getchar();
}

//funckja do atexit
void end()
{
    int error;
    if((error = pthread_mutex_destroy(&mutex)) != 0){
        printf("ERROR CODE : %d", error);
        printf("DESTROY ERROR -> LINE 58\n");
        _exit(EXIT_FAILURE);
    }
    printf("\n------------------\033[38;5;87mMUTEX ZOSTAL USUNIETY\033[m------------------\n");
}

//funkcja wykluczajaca
void* wykluczanie(void *arg)
{
    int licznik = 0;    //wewnetrzny licznik do ktorego bedziemy przypisywac globalny licznik
    int i;  //iterator do petli
    int index_watku = *(int*)arg;   //index watku, wykorzystywany do wypisania watku

    sleep(2);

    for(i = 1; i<=ilosc_sekcji; i++)
    {
        //!SEKCJA PRYWATNA
        gotoxy(0, ilosc_watkow+index_watku+PRINT_HELPER+2); //przejdz do linii w ktorej ma byc wypisana sekcja prywatna watku
        print_index(index_watku);   //wypisz index watku
        printf("\033[38;5;154mSekcja prywatna nr\033[m \033[38;5;118m%d\033[m\n", i);   //wypisz sekcje prywatna i jej numer
        sleep(randomNo());

        // if(pthread_mutex_lock(&mutex) != 0){    //synchronizacja watkow, wpuszczenie watku do sekcji krytycznej
        //     printf("MUTEX_LOCK() ERROR -> LINE 84\n");
        //     exit(EXIT_FAILURE);
        // }     

        //!SEKCJA KRYTYCZNA
        gotoxy(30, ilosc_watkow+index_watku+PRINT_HELPER+2);    //przejdz do linii oraz kolumny w ktorej ma zostac wypisana sekcja
        print_index(index_watku);   //wypisz index watku
        printf("\033[38;5;198mSekcja krytyczna nr\033[m \033[38;5;118m%d\033[m\n", i);  //wypisz sekcje krytyczna i jej numer

        licznik = global;   //przypisujemy wartosc licznika globalnego
        licznik++;  //iterujemy 
        sleep(randomNo());  
        global = licznik;   //po randomowym czasie przypisujemy do wartosci globalnej licznik

        print_licznik();    //wypisujemy nowy licznik
    
        // if(pthread_mutex_unlock(&mutex) != 0){   //wychodzimy z sekcji krytycznej
        //     printf("MUTEX_UNLOCK() ERROR -> LINE 101\n");
        //     exit(EXIT_FAILURE);
        // }
        fflush(stdout);
    }

    gotoxy(0, ilosc_watkow+index_watku+PRINT_HELPER+2); //przejdz do linii w ktorej ma zostac wypisany watek
    print_index(index_watku);   //wypisz index watku
    printf("\033[5;31mSkonczylem prace\033[m");     //wypisz komunikat

    // fflush(stdout);

    pthread_exit(NULL);
}

/**
 *  @param argv[1] ilosc sekcji
 *  @param argv[2] ilosc watkow
*/
int main(int argc, char *argv[])
{
    //sprawdzanie ilosci argumentow
    if(argc != 3){
        printf("ZLE WYWOLANIE: Wywolaj poprzez: %s liczba_sekcji liczba_watkow\nPrzykladowe wywolanie: %s 2 4\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    //sprawdzam czy argument jest liczba
    if((ilosc_sekcji = atoi(argv[1])) == 0){
        printf("ATOI ERROR -> ILOSC SEKCJI -> LINE 132\n");
        exit(EXIT_FAILURE);
    }

    //sprawdzam czy argument jest liczba    
    if((ilosc_watkow = atoi(argv[2])) == 0){
        printf("ATOI ERROR -> ILOSC WATKOW -> LINE 138\n");
        exit(EXIT_FAILURE);
    }
        //ustawiam atexit na funkcje end (line 55)
    if(atexit(end) != 0){
        perror("ATEXIT ERROR -> LINE 143\n");
        _exit(EXIT_FAILURE);
    }

    if(system("clear") == -1){
        printf("SYSTEM() ERROR -> LINE 148\n");
        exit(EXIT_FAILURE);
    }

    //* wywolanie poczatkowego ekranu
    start();

    int i;

    gotoxy(0,0);    //przechodze na poczatek terminala
    printf("Adres mutexu -> \033[93m%p\033[m\n", (void *)&mutex);   //wypisuje adres mutexu

    pthread_t *watki = (pthread_t*)malloc(sizeof(pthread_t)*ilosc_watkow);  //tablica przechowujaca id watkow

    //* tworzenie watkow i wypisywanie ich ID
    for(i = 1; i<=ilosc_watkow; i++){
        if(pthread_create(&watki[i-1], NULL, wykluczanie, (void*)&i)){
            printf("ERROR PTHREAD_CREATE() -> LINE 165\n");
            exit(EXIT_FAILURE);
        }
        else{
            gotoxy(0,i+1);
            print_index(i);
            printf("Utworzono watek o ID -> \033[36m%ld\033[m", watki[i-1]);
        }

        usleep(200);
    }

    //* wypisuje poczatkowy licznik
    print_licznik();

    gotoxy(0, ilosc_watkow+5);

    //* laczenie watkow
    for(i = 0; i<ilosc_watkow; i++)
    {
        if(pthread_join(watki[i], NULL)){
            printf("JOIN ERROR -> LINE 186\n");
            exit(EXIT_FAILURE);
        }
    }

    gotoxy(0, 2*(ilosc_watkow+PRINT_HELPER));    //przechodze do linii ostatecznych komunikatow

    //* wypisanie komunikatu koncowego
    printf("Oczekiwana wartosc = \033[32m%d\033[m porownujac do otrzymanej wartosci = \033[32m%d\033[m\n", ilosc_sekcji*ilosc_watkow, global);
    

    //* sprawdzenie czy estymowana wartosc zgadza sie z uzyskana
    if(ilosc_sekcji*ilosc_watkow == global){
        printf("------------------------\033[38;5;87mSUKCES!\033[m---------------------------\n");
        exit(EXIT_SUCCESS);
    }

    printf("------------------\033[38;5;87mCOS POSZLO NIE TAK!\033[m------------------\n");
    printf("\n");
    exit(EXIT_FAILURE);
}