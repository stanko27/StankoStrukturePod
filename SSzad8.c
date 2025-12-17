/*
8. napisati program koji omogucava rad s binarnim stablom pretrazivanja.
program treba omoguciti:
- unosenje novog elementa u stablo
- ispis elemenata (inorder, preorder, postorder i level order)
- brisanje elementa
- pronalazenje elementa u stablu
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define NO_MEMORY_ERROR -1

// struktura cvora binarnog stabla
typedef struct treeNode* Tree;
typedef struct treeNode {
    int value;
    Tree left;
    Tree right;
} treeNode;

// deklaracije funkcija
Tree insert(int, Tree);
Tree createNode(int);
int inorder(Tree);
int preorder(Tree);
int postorder(Tree);
int levelorder(Tree);
int height(Tree);
int printLevel(Tree, int);
Tree deleteNode(int, Tree);
Tree findMin(Tree);
Tree find(int, Tree);

int main()
{
    Tree root = NULL;
    Tree found = NULL;
    int choice = 0;
    int x = 0;

    while (1) {
        printf("----------\n");
        printf("1 - unos novog elementa\n");
        printf("2 - ispis inorder\n");
        printf("3 - ispis postorder\n");
        printf("4 - ispis preorder\n");
        printf("5 - ispis level order\n");
        printf("6 - trazenje elementa\n");
        printf("7 - brisanje elementa\n");
        printf("8 - izlaz\n");
        printf("----------\n> ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("unesi vrijednost: ");
            scanf("%d", &x);
            root = insert(x, root);
            if (!root)
                return NO_MEMORY_ERROR;
            break;

        case 2:
            if (!root)
                printf("stablo je prazno\n");
            else {
                printf("inorder: ");
                inorder(root);
            }
            break;

        case 3:
            if (!root)
                printf("stablo je prazno\n");
            else {
                printf("postorder: ");
                postorder(root);
            }
            break;

        case 4:
            if (!root)
                printf("stablo je prazno\n");
            else {
                printf("preorder: ");
                preorder(root);
            }
            break;

        case 5:
            if (!root)
                printf("stablo je prazno\n");
            else {
                printf("level order: ");
                levelorder(root);
            }
            break;

        case 6:
            printf("unesi element za trazenje: ");
            scanf("%d", &x);
            found = find(x, root);
            if (!found)
                printf("element se ne nalazi u stablu\n");
            else
                printf("element %d pronadjen na adresi %p\n", found->value, found);
            break;

        case 7:
            printf("unesi element za brisanje: ");
            scanf("%d", &x);
            root = deleteNode(x, root);
            break;

        case 8:
            return 0;
        }
        printf("\n");
    }
}

// alokacija i inicijalizacija novog cvora
Tree createNode(int x)
{
    Tree n = (Tree)malloc(sizeof(treeNode));
    if (!n) {
        printf("greska pri alokaciji memorije\n");
        return NULL;
    }
    n->value = x;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// umetanje elementa u bst
Tree insert(int x, Tree root)
{
    if (!root)
        return createNode(x);

    if (x < root->value)
        root->left = insert(x, root->left);
    else
        root->right = insert(x, root->right);

    return root;
}

// inorder ispis (lijevo - korijen - desno)
int inorder(Tree root)
{
    if (!root)
        return 0;

    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);
    return 0;
}

// preorder ispis (korijen - lijevo - desno)
int preorder(Tree root)
{
    if (!root)
        return 0;

    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
    return 0;
}

// postorder ispis (lijevo - desno - korijen)
int postorder(Tree root)
{
    if (!root)
        return 0;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);
    return 0;
}

// ispis po razinama stabla
int levelorder(Tree root)
{
    int h = height(root);
    for (int i = 1; i <= h; i++)
        printLevel(root, i);
    return 0;
}

// ispis elemenata na zadanoj razini
int printLevel(Tree root, int level)
{
    if (!root)
        return 0;

    if (level == 1)
        printf("%d ", root->value);
    else {
        printLevel(root->left, level - 1);
        printLevel(root->right, level - 1);
    }
    return 0;
}

// racunanje visine stabla
int height(Tree root)
{
    if (!root)
        return 0;

    int hl = height(root->left);
    int hr = height(root->right);

    return (hl > hr ? hl : hr) + 1;
}

// brisanje elementa iz bst-a
Tree deleteNode(int x, Tree root)
{
    if (!root)
        return NULL;

    if (x < root->value)
        root->left = deleteNode(x, root->left);
    else if (x > root->value)
        root->right = deleteNode(x, root->right);
    else {
        // slucaj kad cvor ima jedno ili nijedno dijete
        if (!root->left) {
            Tree tmp = root->right;
            free(root);
            return tmp;
        }
        else if (!root->right) {
            Tree tmp = root->left;
            free(root);
            return tmp;
        }
        // slucaj kad cvor ima dva djeteta
        Tree tmp = findMin(root->right);
        root->value = tmp->value;
        root->right = deleteNode(tmp->value, root->right);
    }
    return root;
}

// trazenje najmanjeg elementa u stablu
Tree findMin(Tree root)
{
    while (root && root->left)
        root = root->left;
    return root;
}

// trazenje elementa u bst-u
Tree find(int x, Tree root)
{
    if (!root)
        return NULL;

    if (x == root->value)
        return root;
    else if (x < root->value)
        return find(x, root->left);
    else
        return find(x, root->right);
}
