/*
11. Prepraviti zadatak 10 na način da se formira hash tablica država. Tablica ima 11 mjesta, a
funkcija za preslikavanje ključ računa da se zbraja ASCII vrijednost prvih pet slova države zatim
računa ostatak cjelobrojnog dijeljenja te vrijednosti s veličinom tablice. Države s istim ključem se
pohranjuju u vezanu listu sortiranu po nazivu države. Svaki čvor vezane liste sadrži stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024
#define N 11

typedef struct treeNode* treePointer;
typedef struct statelist* position;

/* strukture */

struct treeNode {
    char cityName[MAX];
    int population;
    treePointer left;
    treePointer right;
};

struct statelist {
    char stateName[MAX];
    treePointer nextTree;
    position next;
};

/* prototipi */

int hash(char*);
position CreateHeadList();
position CreateListEl(char[]);
int InsertionInList(position, position);
treePointer CreateTreeEl(char[], int);
treePointer InsertionInTree(treePointer, char[], int);
int ReadFromStateFILE(char[], struct statelist*);
int ReadFromCityFILE(position, char[]);
int PrintInorder(treePointer);
position FindState(struct statelist*, char[]);
int PrintHashtable(struct statelist*);
int PrintList(struct statelist);
int PrintCitiesLargerThan(treePointer, int);
int Menu(struct statelist*);

/* main */

int main()
{
    struct statelist hashTable[N];
    int i;

    for (i = 0; i < N; i++) {
        hashTable[i].next = CreateHeadList();
        hashTable[i].nextTree = NULL;
    }

    ReadFromStateFILE("drzave.txt", hashTable);
    PrintHashtable(hashTable);
    Menu(hashTable);

    return 0;
}

/* hash funkcija */

int hash(char* stateName)
{
    int sum = 0, i = 0;

    while (stateName[i] != '\0' && i < 5) {
        sum += stateName[i];
        i++;
    }

    return sum % N;
}

/* lista drzava */

position CreateHeadList()
{
    position head = malloc(sizeof(struct statelist));
    if (!head)
        return NULL;

    head->stateName[0] = '\0';
    head->next = NULL;
    head->nextTree = NULL;

    return head;
}

position CreateListEl(char stateName[])
{
    position el = malloc(sizeof(struct statelist));
    if (!el)
        return NULL;

    strcpy(el->stateName, stateName);
    el->next = NULL;
    el->nextTree = NULL;

    return el;
}

int InsertionInList(position head, position newEl)
{
    position current = head;

    while (current->next != NULL &&
           strcmp(current->next->stateName, newEl->stateName) < 0)
        current = current->next;

    newEl->next = current->next;
    current->next = newEl;

    return 0;
}

/* stablo gradova */

treePointer CreateTreeEl(char cityName[], int population)
{
    treePointer el = malloc(sizeof(struct treeNode));
    if (!el)
        return NULL;

    strcpy(el->cityName, cityName);
    el->population = population;
    el->left = NULL;
    el->right = NULL;

    return el;
}

treePointer InsertionInTree(treePointer root, char cityName[], int population)
{
    if (root == NULL)
        return CreateTreeEl(cityName, population);

    if (population < root->population)
        root->left = InsertionInTree(root->left, cityName, population);
    else if (population > root->population)
        root->right = InsertionInTree(root->right, cityName, population);
    else
        if (strcmp(cityName, root->cityName) < 0)
            root->left = InsertionInTree(root->left, cityName, population);
        else
            root->right = InsertionInTree(root->right, cityName, population);

    return root;
}

/* citanje datoteka */

int ReadFromStateFILE(char fileName[], struct statelist* table)
{
    FILE* fp = fopen(fileName, "r");
    char state[MAX], cityFile[MAX];
    position el;
    int idx;

    if (!fp)
        return -1;

    while (fscanf(fp, "%[^,],%s\n", state, cityFile) == 2) {
        idx = hash(state);
        el = CreateListEl(state);
        InsertionInList(table[idx].next, el);
        ReadFromCityFILE(el, cityFile);
    }

    fclose(fp);
    return 0;
}

int ReadFromCityFILE(position state, char fileName[])
{
    FILE* fp = fopen(fileName, "r");
    char city[MAX];
    int population;

    if (!fp)
        return -1;

    while (fscanf(fp, "%[^,],%d\n", city, &population) == 2) {
        state->nextTree = InsertionInTree(state->nextTree, city, population);
    }

    fclose(fp);
    return 0;
}

/* ispisi */

int PrintInorder(treePointer root)
{
    if (!root)
        return 0;

    PrintInorder(root->left);
    printf("    %s (%d)\n", root->cityName, root->population);
    PrintInorder(root->right);

    return 0;
}

int PrintHashtable(struct statelist* table)
{
    int i;
    for (i = 0; i < N; i++) {
        printf("Bucket %d\n", i);
        PrintList(table[i]);
    }
    return 0;
}

int PrintList(struct statelist bucket)
{
    position current = bucket.next->next;

    while (current) {
        printf("State: %s\n", current->stateName);
        PrintInorder(current->nextTree);
        printf("\n");
        current = current->next;
    }

    return 0;
}

/* pretraga */

position FindState(struct statelist* table, char stateName[])
{
    int idx = hash(stateName);
    position current = table[idx].next->next;

    while (current) {
        if (strcmp(current->stateName, stateName) == 0)
            return current;
        current = current->next;
    }

    return NULL;
}

int PrintCitiesLargerThan(treePointer root, int population)
{
    if (!root)
        return 0;

    PrintCitiesLargerThan(root->left, population);
    if (root->population >= population)
        printf("    %s (%d)\n", root->cityName, root->population);
    PrintCitiesLargerThan(root->right, population);

    return 0;
}

/* izbornik */

int Menu(struct statelist* table)
{
    char state[MAX];
    int pop;
    position found;

    while (1) {
        printf("1 - pretraga\n2 - izlaz\n");
        scanf("%d", &pop);

        if (pop == 2)
            break;

        printf("Unesi drzavu: ");
        scanf("%s", state);

        found = FindState(table, state);

        if (!found) {
            printf("Drzava ne postoji!\n");
        } else {
            PrintInorder(found->nextTree);
            printf("Minimalni broj stanovnika: ");
            scanf("%d", &pop);
            PrintCitiesLargerThan(found->nextTree, pop);
        }
    }

    return 0;
}

