/*
2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

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

int insertAtFront(Position head);
int printList(Position head);
int insertAtEnd(Position head);
int findPerson(Position head);
int deletePerson(Position head);

int main()
{
	struct person head;
	head.next = NULL;
	int number;
	while (1)
	{
		printf("Unesite broj(0-5):\n 1 - Dinamicki dodati novi element na pocetak liste\n 2 - Ispisati listu\n 3 - Dinamicki dodati novi element na kraj liste\n 4 - Pronaci element u listi (po prezimenu)\n 5 - Obrisati odredeni element iz liste\n 0 - Izlazak iz programa\n");
		scanf(" %d", &number);

		if (number == 1)
			insertAtFront(&head);
		else if (number == 2)
			printList(head.next);
		else if (number == 3)
			insertAtEnd(&head);
		else if (number == 4)
			findPerson(head.next);
		else if (number == 5)
			deletePerson(&head);
		else if (number == 0)
			break;
		else
			printf("Greska, pogresan unos znamenke!\n");
	}

	return EXIT_SUCCESS;
}

int insertAtFront(Position head)
{
	Position newPerson = NULL;

	newPerson = (Position)malloc(sizeof(struct person));

	if (!newPerson)
	{
		printf("Neuspjesna alokacija memorije!\n");
		return EXIT_FAILURE;
	}

	printf("Unesite ime osobe: ");
	scanf("%s", newPerson->name);

	printf("Unesite prezime osobe: ");
	scanf("%s", newPerson->surname);

	printf("Unesite godinu rodenja osobe: ");
	scanf("%d", &newPerson->birthYear);

	newPerson->next = head->next;
	head->next = newPerson;

	return EXIT_SUCCESS;
}

int printList(Position head)
{
	while (head != NULL)
	{
		printf("%s %s %d \n", head->name, head->surname, head->birthYear);
		head = head->next;
	}
	
	return EXIT_SUCCESS;
}

int insertAtEnd(Position head)
{
	while (head->next != NULL)
	{
		head = head->next;
	}

	Position newPerson = NULL;

	newPerson = (Position)malloc(sizeof(struct person));

	if (!newPerson)
	{
		printf("Neuspjesna alokacija memorije!\n");
		return EXIT_FAILURE;
	}

	printf("Unesite ime osobe: ");
	scanf("%s", newPerson->name);

	printf("Unesite prezime osobe: ");
	scanf("%s", newPerson->surname);

	printf("Unesite godinu rodenja osobe: ");
	scanf("%d", &newPerson->birthYear);

	newPerson->next = head->next;
	head->next = newPerson;

	return EXIT_SUCCESS;
}

int findPerson(Position head) 
{
	char surname[MAX_LENGTH] = { 0 };

	if (head == NULL) {
		printf("Greska!");
		return NULL;
	}

	printf("\nUnesi prezime:");
	scanf(" %s", &surname);

	while (head->next != NULL && strcmp(surname, head->surname) != 0) 
	{
		head = head->next;
	}

	if (!strcmp(surname, head->surname))
		printf("\nOsoba pronadena ->\t ime: %s\t prezime: %s\t godina rodenja: %d\n", head->name, head->surname, head->birthYear);
	else
		printf("\nOsoba ne postoji!\n");

	return EXIT_SUCCESS;
}

int deletePerson(Position head)
{
	Position previous = NULL;
	char deleteSurname[MAX_LENGTH];

	printf("Unesite prezime osobe koju zelite izbrisati: ");
	scanf("%s", deleteSurname);

	if (!deleteSurname)
	{
		printf("Osoba ne postoji!\n");
	}

	while (head->next != NULL && strcmp(deleteSurname, head->surname!= 0))
	{
		previous = head;
		head = head->next;
	}

	if (previous != NULL && strcmp(deleteSurname, head->surname) == 0)
	{
		previous->next = head->next;
		free(head);
	}
	
	return EXIT_SUCCESS;
}
