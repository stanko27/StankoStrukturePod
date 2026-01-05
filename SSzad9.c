/*
9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraća
pokazivač na korijen stabla.
b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat će se u stablo na
slici Slika 2.
c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira slučajne brojeve u rasponu <10, 90>. Također, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN 10
#define MAX 90

typedef struct treeNode* treePointer;

struct treeNode {
    int value;
    treePointer left;
    treePointer right;
};

/* deklaracije */
treePointer InsertElement(int, treePointer);
treePointer AllocateNew(int);
void InOrder(treePointer, FILE*);
int Replace(treePointer);
int* GenerateRandomArray(int);
void FreeTree(treePointer);

int main()
{
    int i, choice, m;

    FILE* fp1 = fopen("dat1.txt", "w");
    FILE* fp2 = fopen("dat2.txt", "w");

    if (!fp1 || !fp2) {
        printf("Greska pri otvaranju datoteka!\n");
        return 1;
    }

    srand((unsigned)time(NULL));

    while (1) {
        printf("\n-------\n");
        printf("0 - izlaz\n");
        printf("1 - zadani niz\n");
        printf("2 - random niz\n");
        printf("--------\n> ");
        scanf("%d", &choice);

        if (choice == 0)
            break;

        if (choice == 1) {
            int arr[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
            int n = sizeof(arr) / sizeof(arr[0]);
            treePointer root = NULL;

            for (i = 0; i < n; i++)
                root = InsertElement(arr[i], root);

            fprintf(fp1, "Prije replace:\n");
            InOrder(root, fp1);

            Replace(root);

            fprintf(fp1, "\nNakon replace:\n");
            InOrder(root, fp1);
            fprintf(fp1, "\n\n");

            FreeTree(root);
        }
        else if (choice == 2) {
            printf("Koliko elemenata? ");
            scanf("%d", &m);

            int* arr = GenerateRandomArray(m);
            treePointer root = NULL;

            for (i = 0; i < m; i++)
                root = InsertElement(arr[i], root);

            fprintf(fp2, "Prije replace:\n");
            InOrder(root, fp2);

            Replace(root);

            fprintf(fp2, "\nNakon replace:\n");
            InOrder(root, fp2);
            fprintf(fp2, "\n\n");

            free(arr);
            FreeTree(root);
        }
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}

/* insert prema slici: veci -> lijevo, manji ili isti -> desno */
treePointer InsertElement(int value, treePointer root)
{
    if (!root)
        return AllocateNew(value);

    if (value > root->value)
        root->left = InsertElement(value, root->left);
    else
        root->right = InsertElement(value, root->right);

    return root;
}

treePointer AllocateNew(int value)
{
    treePointer node = (treePointer)malloc(sizeof(struct treeNode));
    if (!node)
        return NULL;

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void InOrder(treePointer root, FILE* fp)
{
    if (!root)
        return;

    InOrder(root->left, fp);
    printf("%d ", root->value);
    fprintf(fp, "%d ", root->value);
    InOrder(root->right, fp);
}

 /*
   replace: vraća ukupnu sumu starog podstabla, u cvor sprema sumu njegovih potomaka
 */
int Replace(treePointer root)
{
    if (!root)
        return 0;

    int leftSum = Replace(root->left);
    int rightSum = Replace(root->right);

    int oldValue = root->value;
    root->value = leftSum + rightSum;

    return oldValue + root->value;
}

int* GenerateRandomArray(int n)
{
    int i;
    int* arr = (int*)malloc(n * sizeof(int));
    if (!arr)
        return NULL;

    printf("Generirani niz:\n");
    for (i = 0; i < n; i++) {
        arr[i] = MIN + rand() % (MAX - MIN + 1);
        printf("%d ", arr[i]);
    }
    printf("\n");

    return arr;
}

void FreeTree(treePointer root)
{
    if (!root)
        return;

    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}
