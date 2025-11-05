//5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa rezultat. Stog je potrebno realizirati preko vezane liste.//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// svaki cvor sadrzi jedan broj (value) i pokazivac na sljedeci element stoga
typedef struct Node* Ptr;
typedef struct Node {
    int value;
    Ptr next;
} Node;

// deklaracije funkcija
int readPostfix(Ptr top);
int pushStack(Ptr top, int number);
int popStack(Ptr top, int* number);
void clearStack(Ptr top);

int main(void)
{
    // glava stoga (prazni cvor koji olaksava rad)
    Node head;
    head.next = NULL;

    // poziv funkcije koja cita postfix izraz iz datoteke i racuna rezultat
    if (readPostfix(&head) == EXIT_SUCCESS && head.next != NULL)
        printf("\nrezultat izraza: %d\n", head.next->value);
    else
        printf("\nneuspjelo racunanje izraza!\n");

    // brisanje svih elemenata stoga (ciscenje memorije)
    clearStack(&head);
    return EXIT_SUCCESS;
}


// funkcija koja cita postfix izraz iz datoteke
// i racuna rezultat koristeci stog

int readPostfix(Ptr top)
{
    FILE* fp = fopen("postfix.txt", "r");
    if (!fp) {
        printf("greska: datoteka se ne moze otvoriti.\n");
        return EXIT_FAILURE;
    }

    char token[20];   // varijabla za citanje operatora ili operanda
    int a, b;          // operandi koji se skidaju sa stoga

    // cita sve rijeci (brojeve i operatore) iz datoteke dok ima podataka
    while (fscanf(fp, "%s", token) == 1) {
        printf("procitano: %s\n", token);

        // ako je operator, skinu se dva broja sa stoga i izvrsi operacija
        if (strcmp(token, "+") == 0) {
            popStack(top, &a);
            popStack(top, &b);
            pushStack(top, b + a);
        }
        else if (strcmp(token, "-") == 0) {
            popStack(top, &a);
            popStack(top, &b);
            pushStack(top, b - a);
        }
        else if (strcmp(token, "*") == 0) {
            popStack(top, &a);
            popStack(top, &b);
            pushStack(top, b * a);
        }
        else if (strcmp(token, "/") == 0) {
            popStack(top, &a);
            popStack(top, &b);
            if (a == 0)
                printf("upozorenje: dijeljenje s nulom nije dozvoljeno!\n");
            else
                pushStack(top, b / a);
        }
        // ako nije operator, pretvara string u broj i sprema ga na stog
        else {
            pushStack(top, atoi(token));
        }
    }

    fclose(fp);
    return EXIT_SUCCESS;
}


// funkcija koja gura novi element na vrh stoga

int pushStack(Ptr top, int number)
{
    Ptr newNode = (Ptr)malloc(sizeof(Node));
    if (!newNode) {
        printf("greska: nema dovoljno memorije.\n");
        return EXIT_FAILURE;
    }

    // novi cvor pokazuje na prethodni vrh stoga
    newNode->value = number;
    newNode->next = top->next;
    top->next = newNode;

    return EXIT_SUCCESS;
}


// funkcija koja skida element s vrha stoga

int popStack(Ptr top, int* number)
{
    // ako je stog prazan, vraca se greska
    if (top->next == NULL) {
        printf("stog je prazan!\n");
        return EXIT_FAILURE;
    }

    // sprema vrijednost vršnog elementa i uklanja ga
    Ptr temp = top->next;
    *number = temp->value;
    top->next = temp->next;
    free(temp);

    return EXIT_SUCCESS;
}


// funkcija koja brise sve elemente stoga
// (koristi pop u petlji)

void clearStack(Ptr top)
{
    int tempVal;
    while (top->next != NULL)
        popStack(top, &tempVal);
}
