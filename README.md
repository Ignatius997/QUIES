# Siemano

## UWAGA!!!
**Biblioteka `sqlib` działa TYLKO na Linuxie**, więc jeśli go nie masz: nie czytaj lub zainstaluj Linuxa.  
Testowałem ją jedynie przy użyciu sqlplusa na studentsie: `rlwrap -ir sqlplus` w bazie Oracle,  
i na 99% **NIE BĘDZIE działała poprawnie w jakimkolwiek innym środowisku**.  
Nie jestem nawet pewien, czy strategia (patrz [JAK TO DZIAŁA?](#jak-to-działa)) przeze mnie obrana jest odpowiednia  
do testowania optymalizacji zapytań i sposobu działania sqla.  
**Nie ponoszę żadnej odpowiedzialności itd.**

---

## OPIS
Biblioteka `sqlib` pozwala na szybkie i proste porównywanie wyników, czasów i planów zapytań.  
Została stworzona z myślą o pogłębianiu wiedzy o optymalizacji zapytań i sposobie działania sqla.

---

## JAK TEGO UŻYWAĆ?

### Nazewnictwo plików:
- Jeśli piszesz zapytania do zadania nr. 2 z trzema wariantami i czterema rozwiązaniami (patrz [PRZYKŁAD](#przykład)),  
  nazwij pliki z zapytaniami następująco:
  ```
  2a.sql  2aa.sql  2aaa.sql  2aaaa.sql
  2b.sql  2bb.sql  2bbb.sql  2bbbb.sql
  2c.sql  2cc.sql  2ccc.sql  2cccc.sql
  ```
  Chyba kumasz regułę.

### Odpalanie programu:
1. Wpisz w terminal:
   ```bash
   make exec
   ```
   Komenda ta z plików `sqlib.h` oraz `exec_gen.cpp` tworzy i odpala plik wykonywalny `exec_gen`, który z twoją pomocą (patrz [PRZYKŁAD](#przykład)) tworzy plik `exec.sql`.
2. Odpal sqlplusa:
   ```bash
   rlwrap -ir sqlplus
   ```
   i wklep:
   ```sql
   @exec
   ```

### Dostęp do wyników:
- W odpowiednich plikach `.out` znajdą się wyniki i czasy zapytań.
- W odpowiednich plikach `.plan` znajdą się plany zapytań (patrz [PRZYPISY](#przypisy)).

Więcej szczegółów i funkcjonalności można wywnioskować z `sqlib.h` oraz `makefile'a`.

---

## JAK TO DZIAŁA?

W pliku `exec.sql` znajdują się polecenia, które wykonuje sqlplus.  
Te polecenia to:
- Wykonywanie zapytań użytkownika,
- Usuwanie indeksów i statystyk tabeli,
- Tworzenie i zapisywanie do (nowych) plików.

Przed wykonaniem każdego zapytania usuwane są statystyki oraz indeksy wszystkich tabel podanych przez użytkownika.  
Ma to zapobiec optymalizacji zapytania przez sqla.

Szczegóły można wywnioskować z kodu w `sqlib.h` oraz `exec_gen.cpp`.

---

## PRZYKŁAD

Jeśli chcesz zrobić zadanie o numerze `z` korzystające z `t` tabel o nazwach `n1, ..., nt`,  
które ma `w` wariantów i do każdego wariantu stworzono `r` rozwiązań (patrz [PRZYPISY](#przypisy)):
```bash
make exec
```
Następnie odpowiedz na pytania programu:
```
Tables count: > t
Tables names: > n1, ..., nt
Task number: > z
Subtasks count: > w
Different solves count: > r
```

**Przykład:** Zadanie nr. 3 o trzech wariantach i dowolnie wielu rozwiązaniach:
> "Zadanie 3. Zmodyfikuj zapytanie powyżej (bez wskazówki dla optymalizatora)  
> na trzy sposoby, tak aby zliczało ścieżki długości 5, które:  
> - (a) zaczynają się w wierzchołku 0;  
> - (b) kończą się w wierzchołku o numerze mniejszym niż 50;  
> - (c) spełniają oba powyższe warunki."  
> *(wzięte żywcem z moodle'a)*

---

## PRZYPISY

1. **Plany zapytań**: Są to jedynie plany zapytań generowane przed odpaleniem zapytania,  
   nie są to rzeczywiste plany wykonania zapytań.

2. **Rozwiązania wariantów**: Jeśli masz różną liczbę rozwiązań do różnych wariantów, wystarczy, że podasz tę największą.  
   Nie spowoduje to żadnych zbędnych obliczeń. W plikach wynikowych znajdą się odpowiednie errory wykryte przez sqla.

---

Ignacy Pernach
