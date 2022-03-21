=========================================================================
Autor: Kacper Piątkowski                          
=========================================================================

Zawartosc:
============

Katalog zad1 zawiera 5 programow napisanych w jezyku C
-------------------------------------------------------------------------

Kazdy z programow korzysta z pliku funkcje.c i funkcje.h,
w ktorym znajduja sie dwie funkcje pomocnicze wypisujace dane procesu.

-------------------------------------------------------------------------

Dodatkowo w folderze znajduje sie plik drzewo_genealogiczne.txt,
wymagany w zadaniu 1.d)

Przy kompilacji programu c.c stworzy sie plik drzewo.txt, ktory
po wykonaniu programu zostanie nadpisany.

Przy wykonywaniu programu c.c nadpisze sie plik drzewo.txt, do ktorego
przekierowana zostala zawartosc po uzyciu komendy pstree -p.

W pliku odpowiedz.txt znajduje się odpowiedź na pytanie spod zadań.

-------------------------------------------------------------------------

* Jak uruchomic programy:
=========================

Katalog zawiera program Makefile do kompilacji, linkowania
i uruchamiania powyzszych programow, a takze czyszczenia katalogu.

-> Aby uruchomic 1-szy program, nalezy wykonac komende:
       make runA
-> Aby uruchomic 2-gi program, nalezy wykonac komende:
       make runB
-> Aby uruchomic 3-ci program, nalezy wykonac komende:
       make runC
-> Aby uruchomic 4-ty program, nalezy wykonac komende:
       make runD
-> Aby uruchomic 5-ty program, nalezy wykonac komende:
       make runE

-> Aby skompilowac wszystkie programy do plikow obiektowych
    oraz executable nalezy wykonac komende:
        make all

-> Aby wyczyscic zawartosc katalogu (usunac zbedne pliki), nalezy wykonac:
       make clean

-> Aby spakowac wszystkie rzeczy nalezy wywolac komende:
       make all
   Po czym uzyc komendy:
       make tar

==========================================================================