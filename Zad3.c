#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

typedef struct person* Position;
struct person {
    char name[MAX_LENGTH];
    char surname[MAX_LENGTH];
    int birthYear;
    Position next;
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
    struct person head;
    head.next = NULL;
    int option = -1;

    do {
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

int addFront(Position head)
{
    Position newEl = malloc(sizeof(struct person));
    if (!newEl) {
        puts("Greska u alokaciji memorije!");
        return EXIT_FAILURE;
    }

    printf("Ime: "); scanf("%s", newEl->name);
    printf("Prezime: "); scanf("%s", newEl->surname);
    printf("Godina rodenja: "); scanf("%d", &newEl->birthYear);

    newEl->next = head->next;
    head->next = newEl;

    return EXIT_SUCCESS;
}

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

int addBack(Position head)
{
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

    newEl->next = NULL;
    head->next = newEl;

    return EXIT_SUCCESS;
}

int searchPerson(Position head)
{
    char key[MAX_LENGTH];
    int found = 0;

    printf("Unesite prezime za pretragu: ");
    scanf("%s", key);

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

int removePerson(Position head)
{
    char delSurname[MAX_LENGTH];
    printf("Unesite prezime osobe koju zelite obrisati: ");
    scanf("%s", delSurname);

    Position prev = head;
    Position temp = NULL;

    while (prev->next && strcmp(prev->next->surname, delSurname) != 0)
        prev = prev->next;

    if (!prev->next) {
        puts("Osoba nije pronadena!");
        return EXIT_FAILURE;
    }

    temp = prev->next;
    prev->next = temp->next;
    free(temp);
    puts("Osoba je uspjesno obrisana.");

    return EXIT_SUCCESS;
}

int insertAfter(Position head)
{
    char target[MAX_LENGTH];
    printf("Unesite prezime osobe iza koje zelite dodati novu: ");
    scanf("%s", target);

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

    newEl->next = head->next;
    head->next = newEl;

    puts("Nova osoba dodana iza trazene.");
    return EXIT_SUCCESS;
}

int insertBefore(Position head)
{
    char target[MAX_LENGTH];
    printf("Unesite prezime osobe ispred koje zelite dodati novu: ");
    scanf("%s", target);

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

    newEl->next = head->next;
    head->next = newEl;

    puts("Nova osoba dodana ispred trazene.");
    return EXIT_SUCCESS;
}

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

        while (curr->next != end) {
            if (strcmp(curr->surname, curr->next->surname) > 0) {
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

    for (; head; head = head->next)
        fprintf(fp, "%s %s %d\n", head->name, head->surname, head->birthYear);

    fclose(fp);
    puts("Lista je uspjesno spremljena.");
    return EXIT_SUCCESS;
}

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

    head->next = NULL;
    Position tail = head;
    struct person temp;

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
