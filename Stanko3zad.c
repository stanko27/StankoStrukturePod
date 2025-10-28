#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

// Definicija strukture person i typedef za Position 
typedef struct person* Position;
struct person {
    char name[MAX_LENGTH];
    char surname[MAX_LENGTH];
    int birthYear;
    Position next;  // pokazivac na sljedeci element u listi
};

// Deklaracije funkcija
int addFront(Position head);
int showList(Position head);
int addBack(Position head);
int searchPerson(Position head);
int removePerson(Position head);
int insertAfter(Position head);
int insertBefore(Position head);
int sortBySurname(Position head);
int saveList(Position head);
int loadList(Position head);

int main()
{
    struct person head;   // prazni cvor (glava liste)
    head.next = NULL;
    int option = -1;

    do {
        // Meni
        printf("\n---------------------------------\n");
        printf(" 1  Dodaj osobu na pocetak\n");
        printf(" 2  Ispisi listu\n");
        printf(" 3  Dodaj osobu na kraj\n");
        printf(" 4  Pronadi osobu po prezimenu\n");
        printf(" 5  Obrisi osobu\n");
        printf(" 6  Dodaj osobu iza postojece\n");
        printf(" 7  Dodaj osobu ispred postojece\n");
        printf(" 8  Sortiraj listu po prezimenima\n");
        printf(" 9  Spremi listu u datoteku\n");
        printf("10  Ucitaj listu iz datoteke\n");
        printf(" 0  Izlaz\n");
        printf("---------------------------------\n");
        printf("Unos: ");
        scanf("%d", &option);

        // Poziv odgovarajuce funkcije ovisno o izboru korisnika
        switch (option) {
        case 1: addFront(&head); break;
        case 2: showList(head.next); break;
        case 3: addBack(&head); break;
        case 4: searchPerson(head.next); break;
        case 5: removePerson(&head); break;
        case 6: insertAfter(&head); break;
        case 7: insertBefore(&head); break;
        case 8: sortBySurname(&head); break;
        case 9: saveList(head.next); break;
        case 10: loadList(&head); break;
        case 0: printf("Izlazak iz programa...\n"); break;
        default: printf("Neispravan unos, pokusaj ponovno.\n"); break;
        }
    } while (option != 0);

    return EXIT_SUCCESS;
}

// Dodavanje osobe na pocetak liste
int addFront(Position head)
{
    Position newEl = malloc(sizeof(struct person));
    if (!newEl) {
        puts("Greska u alokaciji memorije!");
        return EXIT_FAILURE;
    }

    // Unos podataka
    printf("Ime: "); scanf("%s", newEl->name);
    printf("Prezime: "); scanf("%s", newEl->surname);
    printf("Godina rodenja: "); scanf("%d", &newEl->birthYear);

    // Uvezivanje novog cvora na pocetak
    newEl->next = head->next;
    head->next = newEl;

    return EXIT_SUCCESS;
}

// Ispis cijele liste
int showList(Position head)
{
    if (!head) {
        puts("Lista je prazna!");
        return EXIT_SUCCESS;
    }

    for (; head != NULL; head = head->next)
        printf("%s %s %d\n", head->name, head->surname, head->birthYear);

    return EXIT_SUCCESS;
}

// Dodavanje osobe na kraj liste
int addBack(Position head)
{
    // Trazimo zadnji element
    while (head->next)
        head = head->next;

    Position newEl = malloc(sizeof(struct person));
    if (!newEl) {
        puts("Neuspjela alokacija memorije!");
        return EXIT_FAILURE;
    }

    printf("Ime: "); scanf("%s", newEl->name);
    printf("Prezime: "); scanf("%s", newEl->surname);
    printf("Godina rodenja: "); scanf("%d", &newEl->birthYear);

    // Novi element postaje zadnji
    newEl->next = NULL;
    head->next = newEl;

    return EXIT_SUCCESS;
}

// Pretraga osobe po prezimenu
int searchPerson(Position head)
{
    char key[MAX_LENGTH];
    int found = 0;

    printf("Unesite prezime za pretragu: ");
    scanf("%s", key);

    // Pretrazujemo cijelu listu
    while (head) {
        if (strcmp(head->surname, key) == 0) {
            printf("Pronadena osoba: %s %s %d\n", head->name, head->surname, head->birthYear);
            found = 1;
        }
        head = head->next;
    }

    if (!found)
        puts("Osoba nije pronadena!");

    return EXIT_SUCCESS;
}

// Brisanje osobe iz liste po prezimenu
int removePerson(Position head)
{
    char delSurname[MAX_LENGTH];
    printf("Unesite prezime osobe koju zelite obrisati: ");
    scanf("%s", delSurname);

    Position prev = head;
    Position temp = NULL;

    // Trazimo cvor koji treba izbrisati
    while (prev->next && strcmp(prev->next->surname, delSurname) != 0)
        prev = prev->next;

    if (!prev->next) {
        puts("Osoba nije pronadena!");
        return EXIT_FAILURE;
    }

    // Uklanjanje i oslobadanje memorije
    temp = prev->next;
    prev->next = temp->next;
    free(temp);
    puts("Osoba je uspjesno obrisana.");

    return EXIT_SUCCESS;
}

// Dodavanje nove osobe iza postojece
int insertAfter(Position head)
{
    char target[MAX_LENGTH];
    printf("Unesite prezime osobe iza koje zelite dodati novu: ");
    scanf("%s", target);

    // Trazimo osobu s trazenim prezimenom
    while (head && strcmp(head->surname, target) != 0)
        head = head->next;

    if (!head) {
        puts("Nema osobe s tim prezimenom!");
        return EXIT_FAILURE;
    }

    Position newEl = malloc(sizeof(struct person));
    if (!newEl) {
        puts("Greska u alokaciji memorije!");
        return EXIT_FAILURE;
    }

    printf("Ime: "); scanf("%s", newEl->name);
    printf("Prezime: "); scanf("%s", newEl->surname);
    printf("Godina rodenja: "); scanf("%d", &newEl->birthYear);

    // Umetanje odmah iza pronadene osobe
    newEl->next = head->next;
    head->next = newEl;

    puts("Nova osoba dodana iza trazene.");
    return EXIT_SUCCESS;
}

// Dodavanje nove osobe ispred postojece
int insertBefore(Position head)
{
    char target[MAX_LENGTH];
    printf("Unesite prezime osobe ispred koje zelite dodati novu: ");
    scanf("%s", target);

    // Trazimo element ispred ciljanog
    while (head->next && strcmp(head->next->surname, target) != 0)
        head = head->next;

    if (!head->next) {
        puts("Nema osobe s tim prezimenom!");
        return EXIT_FAILURE;
    }

    Position newEl = malloc(sizeof(struct person));
    if (!newEl) {
        puts("Neuspjela alokacija memorije!");
        return EXIT_FAILURE;
    }

    printf("Ime: "); scanf("%s", newEl->name);
    printf("Prezime: "); scanf("%s", newEl->surname);
    printf("Godina rodenja: "); scanf("%d", &newEl->birthYear);

    // Umetanje prije ciljanog
    newEl->next = head->next;
    head->next = newEl;

    puts("Nova osoba dodana ispred trazene.");
    return EXIT_SUCCESS;
}

// Sortiranje liste po prezimenima (bubble sort)
int sortBySurname(Position head)
{
    int swapped;
    Position end = NULL;

    if (!head->next) {
        puts("Lista je prazna, nema sto sortirati.");
        return EXIT_SUCCESS;
    }

    do {
        swapped = 0;
        Position prev = head;
        Position curr = head->next;

        // Klasicni bubble sort s pokazivacima
        while (curr->next != end) {
            if (strcmp(curr->surname, curr->next->surname) > 0) {
                // Zamjena cvorova
                Position tmp = curr->next;
                curr->next = tmp->next;
                tmp->next = curr;
                prev->next = tmp;
                swapped = 1;
            }
            prev = prev->next;
            curr = prev->next;
        }
        end = curr;
    } while (swapped);

    puts("Lista sortirana po prezimenima.");
    return EXIT_SUCCESS;
}

// Spremanje liste u tekstualnu datoteku
int saveList(Position head)
{
    char filename[MAX_LENGTH];
    printf("Unesite ime datoteke: ");
    scanf("%s", filename);

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        puts("Greska pri otvaranju datoteke!");
        return EXIT_FAILURE;
    }

    // Upis svakog cvora u datoteku
    for (; head; head = head->next)
        fprintf(fp, "%s %s %d\n", head->name, head->surname, head->birthYear);

    fclose(fp);
    puts("Lista je uspjesno spremljena.");
    return EXIT_SUCCESS;
}

// Ucitavanje liste iz datoteke
int loadList(Position head)
{
    char filename[MAX_LENGTH];
    printf("Unesite ime datoteke: ");
    scanf("%s", filename);

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        puts("Ne mogu otvoriti datoteku!");
        return EXIT_FAILURE;
    }

    // Brisanje prethodne liste i ucitavanje nove
    head->next = NULL;
    Position tail = head;
    struct person temp;

    // Ucitavanje liniju po liniju
    while (fscanf(fp, "%s %s %d", temp.name, temp.surname, &temp.birthYear) == 3) {
        Position newEl = malloc(sizeof(struct person));
        if (!newEl) {
            puts("Greska u alokaciji!");
            fclose(fp);
            return EXIT_FAILURE;
        }
        strcpy(newEl->name, temp.name);
        strcpy(newEl->surname, temp.surname);
        newEl->birthYear = temp.birthYear;
        newEl->next = NULL;
        tail->next = newEl;
        tail = newEl;
    }

    fclose(fp);
    puts("Lista uspjesno ucitana iz datoteke.");
    return EXIT_SUCCESS;
}
