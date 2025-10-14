#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAKS_BODOVA 50

typedef struct {
    char prvo_ime[30];
    char prezime_osobe[30];
    int osvojeni_bodovi;
} Osoba;

int brojLinija(FILE* dat);
void ucitajPodatke(FILE* dat, Osoba* lista, int broj);
void prikaziPodatke(Osoba* lista, int broj);

int main() {
    FILE* ulaznaDatoteka = fopen("studenti.txt", "r"); //"r" - read, otvaranje datoteke za citanje
    if (!ulaznaDatoteka) {
        printf("Greska, datoteka se ne moze otvoriti.\n"); //ako se ovo ispise, datoteka se nije uspjesno 
                                                           //otvorila(datoteka ne postoji, nije dobra 
                                                           // putanja do datoteke, krivo ime datoteke, ...)
        return 1; //znaci da je program zavrsio sa greskom
    }

    int ukupnoOsoba = brojLinija(ulaznaDatoteka); //pozivamo funkciju kojom racunamo broj linija u student.txt

    Osoba* nizOsoba = (Osoba*)malloc(ukupnoOsoba * sizeof(Osoba)); //dinamièka alokacija memorije
    if (!nizOsoba) { //provjera je li alokacija uspijela
        printf("Greska, problem sa memorijom.\n");
        fclose(ulaznaDatoteka);
        return 1;
    }

    ucitajPodatke(ulaznaDatoteka, nizOsoba, ukupnoOsoba); //funkcija koja ucitava podatke iz datoteka studenti.txt
    prikaziPodatke(nizOsoba, ukupnoOsoba); //poziv funkcije koja ispisuje podatke

    free(nizOsoba); //ovime cemo osloboditi memoriju
    fclose(ulaznaDatoteka); //zatvaramo datoteku
    return 0;
}

// Funkcija za brojanje linija u datoteci
int brojLinija(FILE* dat) {
    int brojac = 0;
    char red[100];
    while (fgets(red, sizeof(red), dat)) {
        brojac++;
    }
    rewind(dat);
    return brojac;
}

// Uèitavanje osoba iz datoteke
void ucitajPodatke(FILE* dat, Osoba* lista, int broj) {
    for (int i = 0; i < broj; i++) {
        fscanf(dat, "%s %s %d", lista[i].prvo_ime, lista[i].prezime_osobe, &lista[i].osvojeni_bodovi);
    }
}

// Ispis osoba s relativnim bodovima
void prikaziPodatke(Osoba* lista, int broj) {
    printf("\nIme          Prezime       Bodovi   Postotak\n\n");
    for (int i = 0; i < broj; i++) {
        float postotak = (lista[i].osvojeni_bodovi / (float)MAKS_BODOVA) * 100;
        printf("%-12s %-13s %3d      %.2f%%\n",
            lista[i].prvo_ime, lista[i].prezime_osobe,
            lista[i].osvojeni_bodovi, postotak);
    }
}