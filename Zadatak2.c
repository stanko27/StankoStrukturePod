/*
Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
dinamièki dodaje novi element na poèetak liste,
ispisuje listu,dinamièki dodaje novi element na kraj liste, pronalazi element u listi (po prezimenu), briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable. */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DULJINA 50

typedef struct osoba* Pozicija;
struct osoba {
    char ime[MAX_DULJINA];
    char prezime[MAX_DULJINA];
    int godinaRodenja;
    Pozicija next;
};

int UnosNaPocetak(Pozicija head);
int IspisListe(Pozicija head);
int UnosNaKraj(Pozicija head);
int PronadiOsobu(Pozicija head);
int ObrisiOsobu(Pozicija head);

int main()
{
    struct osoba head;
    head.next = NULL;
    int izbor;

    do {
        printf("\nOdaberite opciju (0-5):\n");
        printf(" 1 - Dodati novu osobu na pocetak liste\n");
        printf(" 2 - Ispisati sve osobe u listi\n");
        printf(" 3 - Dodati novu osobu na kraj liste\n");
        printf(" 4 - Pronaci osobu po prezimenu\n");
        printf(" 5 - Obrisati osobu iz liste\n");
        printf(" 0 - Izlaz iz programa\n");
        printf("Unos: ");
        scanf(" %d", &izbor);

        switch (izbor)
        {
        case 1:
            UnosNaPocetak(&head);
            break;
        case 2:
            IspisListe(head.next);
            break;
        case 3:
            UnosNaKraj(&head);
            break;
        case 4:
            PronadiOsobu(head.next);
            break;
        case 5:
            ObrisiOsobu(&head);
            break;
        case 0:
            printf("Izlaz iz programa...\n");
            break;
        default:
            printf("Pogresan unos! Pokusajte ponovno.\n");
        }

    } while (izbor != 0);

    return EXIT_SUCCESS;
}

int UnosNaPocetak(Pozicija head)
{
    Pozicija novaOsoba = (Pozicija)malloc(sizeof(struct osoba));
    if (!novaOsoba)
    {
        printf("Greska pri alokaciji memorije!\n");
        return EXIT_FAILURE;
    }

    printf("Unesite ime: ");
    scanf(" %s", novaOsoba->ime);

    printf("Unesite prezime: ");
    scanf(" %s", novaOsoba->prezime);

    printf("Unesite godinu rodenja: ");
    scanf(" %d", &novaOsoba->godinaRodenja);

    novaOsoba->next = head->next;
    head->next = novaOsoba;

    printf("Osoba dodana na pocetak liste!\n");

    return EXIT_SUCCESS;
}

int IspisListe(Pozicija head)
{
    if (head == NULL)
    {
        printf("Lista je prazna!\n");
        return EXIT_SUCCESS;
    }

    printf("\n--- Popis osoba ---\n");
    while (head != NULL)
    {
        printf("%s %s (%d)\n", head->ime, head->prezime, head->godinaRodenja);
        head = head->next;
    }

    return EXIT_SUCCESS;
}

int UnosNaKraj(Pozicija head)
{
    while (head->next != NULL)
        head = head->next;

    Pozicija novaOsoba = (Pozicija)malloc(sizeof(struct osoba));
    if (!novaOsoba)
    {
        printf("Greska pri alokaciji memorije!\n");
        return EXIT_FAILURE;
    }

    printf("Unesite ime: ");
    scanf(" %s", novaOsoba->ime);

    printf("Unesite prezime: ");
    scanf(" %s", novaOsoba->prezime);

    printf("Unesite godinu rodenja: ");
    scanf(" %d", &novaOsoba->godinaRodenja);

    novaOsoba->next = NULL;
    head->next = novaOsoba;

    printf("Osoba dodana na kraj liste!\n");

    return EXIT_SUCCESS;
}

int PronadiOsobu(Pozicija head)
{
    char trazenoPrezime[MAX_DULJINA];

    if (head == NULL)
    {
        printf("Lista je prazna!\n");
        return EXIT_SUCCESS;
    }

    printf("Unesite prezime koje zelite pretraziti: ");
    scanf(" %s", trazenoPrezime);

    while (head != NULL && strcmp(trazenoPrezime, head->prezime) != 0)
        head = head->next;

    if (head != NULL)
        printf("\nPronadena osoba: %s %s, godina rodenja %d\n",
            head->ime, head->prezime, head->godinaRodenja);
    else
        printf("\nOsoba s tim prezimenom nije pronadena.\n");

    return EXIT_SUCCESS;
}

int ObrisiOsobu(Pozicija head)
{
    char obrisiPrezime[MAX_DULJINA];
    Pozicija prethodna = head;
    Pozicija trenutna = head->next;

    if (trenutna == NULL)
    {
        printf("Lista je prazna!\n");
        return EXIT_SUCCESS;
    }

    printf("Unesite prezime osobe koju zelite obrisati: ");
    scanf(" %s", obrisiPrezime);

    while (trenutna != NULL && strcmp(obrisiPrezime, trenutna->prezime) != 0)
    {
        prethodna = trenutna;
        trenutna = trenutna->next;
    }
    if (trenutna == NULL)
    {
        printf("Osoba nije pronadena!\n");
        return EXIT_SUCCESS;
    }

    prethodna->next = trenutna->next;
    free(trenutna);

    printf("Osoba uspjesno obrisana!\n");

    return EXIT_SUCCESS;
}