/* 
 ----------------------------------------------------------------------
 Autor: Kacper Piatkowski
 ----------------------------------------------------------------------
*/ 

//* biblioteki
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//* wlasna biblioteka
#include "funkcje.h"

/*
    * SEMAPHORE_NAME -> nazwa semaforas
    * SEMAPHORE_INITIAL_VALUE -> wartosc semafora przy inicjalizacji
    * FILE_NAME -> nazwa pliku z numerem
    * FILE_INITIAL_VALUE -> liczba wpisana defaultowo do pliku
    * PATH -> uzywane w sprintfie do robienia executable do execl
*/
#define SEMAPHORE_NAME "test_semaphore"
#define SEMAPHORE_INITIAL_VALUE 1
#define FILE_NAME "numer.txt"
#define FILE_INITIAL_VALUE 0
#define PATH "./"

//* globalny pointer, który potem posluży do wskazania na semafor
sem_t *ptr = NULL;

//* handler do sygnalu SIGINT
static void handler(int sigNum)
{
    exit(EXIT_SUCCESS);
}

//* funkcja wywolywana przy atexit()
//* zamyka semafor, usuwa go oraz wypisuje komunikat
void ending_sequence()
{
    sem_zamknij(ptr);
    sem_usun(SEMAPHORE_NAME);
    printf("\n-------------------------------------------------\n");
    printf("\t\tSEMAFOR USUNIETY");
    printf("\n-------------------------------------------------\n");
    _exit(EXIT_SUCCESS);
}

/*
  * argv[1] -  nazwa programu do inicjalizowania procesow
  * argv[2] - liczba procesow
  * argv[3] - liczba sekcji
*/
int main(int argc, char *argv[])
{
    //* obsluga bledów dla zlej ilosci arugmentow przy wywolaniu
    if(argc != 4)
    {
        printf("NIEPOPRAWNE WYWOLANIE -> ZA MALA ILOSC ARGUMENTOW (WYMAGANA ILOSC -> 3)\n");
        printf("PRZYKLADOWE WYWOLANIE: %s NAZWA.x ILOSC_POTOMKOW ILOSC_SEKCJI\n", argv[0]);
        printf("\tNP. %s wykluczajacy.x 10 2\n", argv[0]);
        _exit(EXIT_FAILURE);
    }

    int child_number;   //* ilosc potomków, 
    int sections;   //* ilosc sekcji

    //* zmiana argumentow z tablic znakowych na inty
    if(((child_number = atoi(argv[2])) == 0) || (sections = atoi(argv[3])) == 0)
    {
        printf("ZLE WPROWADZONE ARGUMENTY -> argv[2], argv[3] powinny byc liczbami!\n");
        exit(EXIT_FAILURE);
    }

    char executable[32];    //* tablica znakowa przechowujaca PATH + nazwa pliku

    //* sklejenie sprintfem PATH, argv[1] to tablicy executable
    sprintf(executable, "%s%s", PATH, argv[1]);

    //* signal handling dla SIGINTA -> CTRL-C
    if(signal(SIGINT, handler) == SIG_ERR)
    {
        perror("SIGNAL INTERRUPT ERROR\n");
        exit(EXIT_FAILURE);
    }

    //* tworzymy semafor, ustawiamy pointer na ten semafor
    sem_t *Semafor = sem_stworz(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, SEMAPHORE_INITIAL_VALUE);
    ptr = Semafor;
    
    int val;    //* zmienna do ktorej pobieramy wartosc semafora
    sem_pobierz_wartosc(Semafor, &val);

    //* wypisanie komunikatu poczatkowego do terminala
    printf("\nPID = %d | Nazwa semafora: %s | o adresie: %p | wartosci poczatkowej: %d\n\n", getpid(), SEMAPHORE_NAME, (void *)Semafor, val);
    
    //* ustawienie funkcji atexit() -> patrz wyzej do funkcji ending_sequence() (line 40)
    if(atexit(ending_sequence)!=0)
    {
        perror("ATEXIT ERROR\n");
        exit(EXIT_FAILURE);
    }

    int child_PID;  //* child_PID -> PID potomka
    int final_value = -1; //* final_value -> zmienna przechowujaca ostateczna wartosc z pliku FILENAME (default: numer.txt)

    //* otwarcie pliku FILENAME z WRITE-MODE, wpisanie do niego FILE_INITIAL_VALUE (default: 0) oraz zamkniecie pliku + obsluga bledów
    FILE *numer;
    if((numer = fopen(FILE_NAME, "w")) == NULL)
    {
        perror("FOPEN ERROR -> MACIERZYSTY -> WRITE-MODE\n");
        exit(EXIT_FAILURE);
    }

    if(fprintf(numer, "%d", FILE_INITIAL_VALUE) < 0)
    {
        if(ferror(numer));
        printf("FPRINTF ERROR -> MACIERZYSTY -> WRITE-MODE\n");
        exit(EXIT_FAILURE);
    }

    if(fclose(numer) == EOF)
    {
        perror("FCLOSE ERROR -> MACIERZYSTY -> WRITE-MODE\n");
        exit(EXIT_FAILURE);
    }

    int i;
    for(i = 0; i<child_number; i++)
    {
        child_PID = fork();
        switch (child_PID)
        {
            //* case error -> obsluga bledów fork()
            case -1: 
                perror("FORK ERROR\n");         
                exit(EXIT_FAILURE);
                break;

            //* case potomek -> uruchomienie programu funkcja execl() (default: wykluczajacy.x) + obsluga bledow
            case 0:
                execl(executable, argv[1], SEMAPHORE_NAME, FILE_NAME, argv[3], NULL);    
                perror("EXECL ERROR -> POTOMEK -> CASE 0\n");
                _exit(EXIT_FAILURE);
            
            //* case macierzysty -> w tym programie macierzysty przy forku() macierzysty nic nie wykonuje
            default:
                break;
        }
    }

    //* proces macierzysty czeka na "child_number" procesow potomnych + obsluga bledow
    for(i = 0; i < child_number; i++)
    {
        if(wait(NULL) == -1)
        {
            perror("WAIT ERROR -> MACIERZYSTY\n");
            _exit(EXIT_FAILURE);
        }
    }

    //* otwarcie pliku FILENAME z READ-MODE, zczytanie z niego final_value (patrz line 105) oraz zamkniecie pliku + obsluga bledów
    if((numer = fopen(FILE_NAME, "r")) == NULL)
    {
        perror("FOPEN ERROR -> MACIERZYSTY -> READ-MODE\n");
        exit(EXIT_FAILURE);
    }

    if(fscanf(numer, "%d", &final_value) == EOF)
    {
        perror("FSCANF ERROR -> MACIERZYSTY -> READ-MODE\n");
        exit(EXIT_FAILURE);
    }
    if(fclose(numer) == EOF)
    {
        perror("FCLOSE ERROR -> MACIERZYSTY -> READ-MODE\n");
        exit(EXIT_FAILURE);
    }

    //* pobranie wartosci semafora + wypisanie komunikatu koncowego
    sem_pobierz_wartosc(Semafor, &val);
    printf("\nILOSC POTOMKÓW: %d | WARTOSC POCZATKOWA SEMAFORU: %d | ILOSC SEKCJI %d | WARTOSC PLIKU %s = %d\n", child_number, val, sections, FILE_NAME, final_value);

    //* sprawdzenie czy wszystko przebieglo pomyslnie
    if(child_number*sections == final_value){
        printf("\nESTYMOWANA WARTOSC PLIKU %s ZOSTALA OSIAGNIETA -> %d\n", FILE_NAME, final_value);
        exit(EXIT_SUCCESS);
    }
    else{
        printf("COS POSZLO NIE TAK! WARTOSC OCZEKIWANA -> %d != %d <- WARTOSC UZYSKANA\n", child_number*sections, final_value);
        exit(EXIT_FAILURE);
    }
}