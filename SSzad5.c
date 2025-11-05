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
int clearStack(Ptr top);   // promijenjeno iz void u int

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
    if (clearStack(&head) == EXIT_SUCCESS)
        printf("memorija uspjesno oslobodjena.\n");
    else
        printf("greska prilikom oslobadjanja memorije.\n");

    return EXIT_SUCCESS;
}

// funkcija koja cita postfix izraz iz datoteke i racuna rezultat koristeci stog
int readPostfix(Ptr top)
{
    FILE* fp = fopen("postfix.txt", "r");
    if (!fp) {
        printf("greska: datoteka se ne moze otvoriti.\n");
        return EXIT_FAILURE;
    }

    char token[20];
    int a, b;

    while (fscanf(fp, "%s", token) == 1) {
        printf("procitano: %s\n", token);

        if (strcmp(token, "+") == 0) {
            if (popStack(top, &a) == EXIT_FAILURE || popStack(top, &b) == EXIT_FAILURE)
                return EXIT_FAILURE;
            pushStack(top, b + a);
        }
        else if (strcmp(token, "-") == 0) {
            if (popStack(top, &a) == EXIT_FAILURE || popStack(top, &b) == EXIT_FAILURE)
                return EXIT_FAILURE;
            pushStack(top, b - a);
        }
        else if (strcmp(token, "*") == 0) {
            if (popStack(top, &a) == EXIT_FAILURE || popStack(top, &b) == EXIT_FAILURE)
                return EXIT_FAILURE;
            pushStack(top, b * a);
        }
        else if (strcmp(token, "/") == 0) {
            if (popStack(top, &a) == EXIT_FAILURE || popStack(top, &b) == EXIT_FAILURE)
                return EXIT_FAILURE;
            if (a == 0) {
                printf("upozorenje: dijeljenje s nulom nije dozvoljeno!\n");
                return EXIT_FAILURE;
            }
            pushStack(top, b / a);
        }
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

    newNode->value = number;
    newNode->next = top->next;
    top->next = newNode;

    return EXIT_SUCCESS;
}

// funkcija koja skida element s vrha stoga
int popStack(Ptr top, int* number)
{
    if (top->next == NULL) {
        printf("stog je prazan!\n");
        return EXIT_FAILURE;
    }

    Ptr temp = top->next;
    *number = temp->value;
    top->next = temp->next;
    free(temp);

    return EXIT_SUCCESS;
}

// funkcija koja brise sve elemente stoga
int clearStack(Ptr top)
{
    int tempVal;
    while (top->next != NULL) {
        if (popStack(top, &tempVal) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

