/*
4. Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _poly* position;

typedef struct _poly {
    int coefficient;
    int exponent;
    position next;
} poly;

position createElement(int coefficient, int exponent);
int addToList(position poly, position newElement);
int readPolyFromFile(char filename[], position poly1, position poly2);
int printList(position poly);
int sumPoly(position poly1, position poly2, position sumResult);
int multiplyPoly(position poly1, position poly2, position multiplyResult);

int main() {
    poly poly1 = {.coefficient = 0, .exponent = 0, .next = NULL};
    poly poly2 = {.coefficient = 0, .exponent = 0, .next = NULL};
    poly sum = {.coefficient = 0, .exponent = 0, .next = NULL};
    poly multiply = {.coefficient = 0, .exponent = 0, .next = NULL};
    char filename[50];

    printf("unesi ime datoteke: ");
    scanf(" %[^\n]", filename);

    readPolyFromFile(filename, &poly1, &poly2);

    printf("\nprvi polinom: ");
    printList(&poly1);
    printf("\ndrugi polinom: ");
    printList(&poly2);

    sumPoly(&poly1, &poly2, &sum);
    multiplyPoly(&poly1, &poly2, &multiply);

    printf("\n\n--- zbroj polinoma ---\n");
    printList(&sum);
    printf("\n\n--- umnozak polinoma ---\n");
    printList(&multiply);
    printf("\n");

    return EXIT_SUCCESS;
}

/* funkcija cita dva polinoma iz datoteke */
int readPolyFromFile(char filename[], position poly1, position poly2) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("datoteka se ne moze otvoriti!\n");
        return EXIT_FAILURE;
    }

    char line[1024];
    int coefficient = 0, exponent = 0, n = 0;
    position newEl = NULL;

    /* prvi polinom */
    fgets(line, 1024, fp);
    char* ptr = line;
    while (strlen(ptr) > 0) {
        sscanf(ptr, "%d %d %n", &coefficient, &exponent, &n);
        if (coefficient != 0) {
            newEl = createElement(coefficient, exponent);
            addToList(poly1, newEl);
        }
        ptr += n;
    }

    /* drugi polinom */
    fgets(line, 1024, fp);
    ptr = line;
    while (strlen(ptr) > 0) {
        sscanf(ptr, "%d %d %n", &coefficient, &exponent, &n);
        if (coefficient != 0) {
            newEl = createElement(coefficient, exponent);
            addToList(poly2, newEl);
        }
        ptr += n;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

/* stvara novi element liste */
position createElement(int coefficient, int exponent) {
    position newEl = (position)malloc(sizeof(poly));
    if (!newEl) {
        printf("greska kod alokacije memorije!\n");
        return NULL;
    }
    newEl->coefficient = coefficient;
    newEl->exponent = exponent;
    newEl->next = NULL;
    return newEl;
}

/* dodaje element u listu po eksponentu */
int addToList(position poly, position newElement) {
    while (poly->next != NULL && poly->next->exponent > newElement->exponent)
        poly = poly->next;

    if (poly->next != NULL && poly->next->exponent == newElement->exponent) {
        poly->next->coefficient += newElement->coefficient;
        free(newElement);
    } else {
        newElement->next = poly->next;
        poly->next = newElement;
    }
    return EXIT_SUCCESS;
}

/* ispisuje polinom u standardnom obliku */
int printList(position poly) {
    while (poly->next != NULL) {
        poly = poly->next;
        if (poly->next == NULL)
            printf("%dx^%d", poly->coefficient, poly->exponent);
        else
            printf("%dx^%d + ", poly->coefficient, poly->exponent);
    }
    return EXIT_SUCCESS;
}

/* sabira dva polinoma */
int sumPoly(position poly1, position poly2, position sumResult) {
    position temp = poly1;
    while (temp->next != NULL) {
        addToList(sumResult, createElement(temp->next->coefficient, temp->next->exponent));
        temp = temp->next;
    }
    temp = poly2;
    while (temp->next != NULL) {
        addToList(sumResult, createElement(temp->next->coefficient, temp->next->exponent));
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

/* mnozi dva polinoma */
int multiplyPoly(position poly1, position poly2, position multiplyResult) {
    position p1 = poly1->next;
    position p2 = NULL;
    position newEl = NULL;

    while (p1 != NULL) {
        p2 = poly2->next;
        while (p2 != NULL) {
            newEl = createElement(p1->coefficient * p2->coefficient, p1->exponent + p2->exponent);
            addToList(multiplyResult, newEl);
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    return EXIT_SUCCESS;
}