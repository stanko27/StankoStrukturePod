/*
10. Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.
                a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki čvor vezane liste
                   sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
                b) Potrebno je formirati stablo država sortirano po nazivu. Svaki čvor stabla sadrži vezanu
                   listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati države i gradove te omogućiti korisniku putem
tastature pretragu gradova određene države koji imaju broj stanovnika veći od unosa na
tastaturi.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

 

/* stablo gradova */
typedef struct cityTree* cityTreePos;
struct cityTree {

	char cityName[MAX];
	int population;
	cityTreePos left;
	cityTreePos right;
};

/* lista gradova */
typedef struct cityList* cityListPos;
struct cityList {

	char cityName[MAX];
	int population;
	cityListPos next;
};



/* lista drzava (a) */
typedef struct stateList* stateListPos;
struct stateList {

	char stateName[MAX];
	cityTreePos cityTree;
	stateListPos next;
};

/* stablo drzava (b) */
typedef struct stateTree* stateTreePos;
struct stateTree {

	char stateName[MAX];
	cityListPos cityList;
	stateTreePos left;
	stateTreePos right;
};



/* gradovi */
cityTreePos InsertCityTree(cityTreePos, char[], int);
cityListPos InsertCityList(cityListPos, char[], int);

/* drzave */
stateListPos CreateStateListEl(char[]);
stateTreePos InsertStateTree(stateTreePos, char[]);
int InsertStateList(stateListPos, stateListPos);

/* citanje */
int ReadStates(char[], stateListPos, stateTreePos*);
int ReadCitiesForBoth(char[], stateListPos, stateTreePos);

/* ispis */
int PrintCityTree(cityTreePos);
int PrintCityList(cityListPos);
int PrintStateList(stateListPos);
int PrintStateTree(stateTreePos);

/* pomocne */
stateListPos FindStateInList(stateListPos, char[]);
stateTreePos FindStateInTree(stateTreePos, char[]);

/* izbornik */
int Menu(stateListPos, stateTreePos);



int main() {

	stateListPos listHead = malloc(sizeof(struct stateList));
	stateTreePos stateRoot = NULL;

	strcpy(listHead->stateName, "");
	listHead->cityTree = NULL;
	listHead->next = NULL;

	ReadStates("drzave.txt", listHead, &stateRoot);

	Menu(listHead, stateRoot);

	return 0;
}



/* ubacivanje grada u BST */
cityTreePos InsertCityTree(cityTreePos root, char name[], int pop) {

	if (!root) {
		root = malloc(sizeof(struct cityTree));
		strcpy(root->cityName, name);
		root->population = pop;
		root->left = root->right = NULL;
		return root;
	}

	if (pop < root->population)
		root->left = InsertCityTree(root->left, name, pop);
	else if (pop > root->population)
		root->right = InsertCityTree(root->right, name, pop);
	else if (strcmp(name, root->cityName) < 0)
		root->left = InsertCityTree(root->left, name, pop);
	else
		root->right = InsertCityTree(root->right, name, pop);

	return root;
}

/* ubacivanje grada u listu */
cityListPos InsertCityList(cityListPos head, char name[], int pop) {

	cityListPos newEl = malloc(sizeof(struct cityList));
	strcpy(newEl->cityName, name);
	newEl->population = pop;
	newEl->next = NULL;

	if (!head || pop < head->population ||
		(pop == head->population && strcmp(name, head->cityName) < 0)) {
		newEl->next = head;
		return newEl;
	}

	cityListPos temp = head;
	while (temp->next &&
		(temp->next->population < pop ||
		(temp->next->population == pop &&
		 strcmp(temp->next->cityName, name) < 0)))
		temp = temp->next;

	newEl->next = temp->next;
	temp->next = newEl;

	return head;
}

/* stvaranje drzave za listu */
stateListPos CreateStateListEl(char name[]) {

	stateListPos el = malloc(sizeof(struct stateList));
	strcpy(el->stateName, name);
	el->cityTree = NULL;
	el->next = NULL;
	return el;
}

/* ubacivanje drzave u listu */
int InsertStateList(stateListPos head, stateListPos el) {

	while (head->next && strcmp(head->next->stateName, el->stateName) < 0)
		head = head->next;

	el->next = head->next;
	head->next = el;
	return 0;
}

/* ubacivanje drzave u stablo */
stateTreePos InsertStateTree(stateTreePos root, char name[]) {

	if (!root) {
		root = malloc(sizeof(struct stateTree));
		strcpy(root->stateName, name);
		root->cityList = NULL;
		root->left = root->right = NULL;
		return root;
	}

	if (strcmp(name, root->stateName) < 0)
		root->left = InsertStateTree(root->left, name);
	else if (strcmp(name, root->stateName) > 0)
		root->right = InsertStateTree(root->right, name);

	return root;
}

/* citanje drzava */
int ReadStates(char file[], stateListPos listHead, stateTreePos* treeRoot) {

	FILE* fp = fopen(file, "r");
	char state[MAX], cityFile[MAX];

	while (fscanf(fp, " %[^,],%s", state, cityFile) == 2) {

		stateListPos newListEl = CreateStateListEl(state);
		InsertStateList(listHead, newListEl);

		*treeRoot = InsertStateTree(*treeRoot, state);

		ReadCitiesForBoth(cityFile, newListEl,
			FindStateInTree(*treeRoot, state));
	}

	fclose(fp);
	return 0;
}

/* citanje gradova za listu i stablo */
int ReadCitiesForBoth(char file[], stateListPos listEl, stateTreePos treeEl) {

	FILE* fp = fopen(file, "r");
	char city[MAX];
	int pop;

	while (fscanf(fp, " %[^,],%d", city, &pop) == 2) {
		listEl->cityTree = InsertCityTree(listEl->cityTree, city, pop);
		treeEl->cityList = InsertCityList(treeEl->cityList, city, pop);
	}

	fclose(fp);
	return 0;
}

/* ispis BST gradova */
int PrintCityTree(cityTreePos root) {

	if (!root) return 0;
	PrintCityTree(root->left);
	printf("    %s (%d)\n", root->cityName, root->population);
	PrintCityTree(root->right);
	return 0;
}

/* ispis liste gradova */
int PrintCityList(cityListPos head) {

	while (head) {
		printf("    %s (%d)\n", head->cityName, head->population);
		head = head->next;
	}
	return 0;
}

/* ispis liste drzava */
int PrintStateList(stateListPos head) {

	head = head->next;
	while (head) {
		printf("\nDrzava: %s\n", head->stateName);
		PrintCityTree(head->cityTree);
		head = head->next;
	}
	return 0;
}

/* ispis stabla drzava */
int PrintStateTree(stateTreePos root) {

	if (!root) return 0;
	PrintStateTree(root->left);
	printf("\nDrzava: %s\n", root->stateName);
	PrintCityList(root->cityList);
	PrintStateTree(root->right);
	return 0;
}

/* trazenje drzave u listi */
stateListPos FindStateInList(stateListPos head, char name[]) {

	head = head->next;
	while (head) {
		if (strcmp(head->stateName, name) == 0)
			return head;
		head = head->next;
	}
	return NULL;
}

/* trazenje drzave u stablu */
stateTreePos FindStateInTree(stateTreePos root, char name[]) {

	if (!root) return NULL;
	if (strcmp(name, root->stateName) == 0)
		return root;
	if (strcmp(name, root->stateName) < 0)
		return FindStateInTree(root->left, name);
	return FindStateInTree(root->right, name);
}


int Menu(stateListPos listHead, stateTreePos treeRoot) {

	int choice;
	char name[MAX];

	while (1) {

		printf("\n1 - ispis (a)\n");
		printf("2 - ispis (b)\n");
		printf("3 - izlaz\n");
		scanf("%d", &choice);

		if (choice == 3) break;

		if (choice == 1)
			PrintStateList(listHead);

		if (choice == 2)
			PrintStateTree(treeRoot);
	}

	return 0;
}