/*
1. napisati program koji prvo proèita koliko redaka ima datoteka, tj.koliko ima studenata
zapisanih u datoteci.nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata(ime, prezime, bodovi) i uèitati iz datoteke sve zapise.na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
napomena: svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova / max_br_bodova * 100
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE (64)
#define MAX_POINTS (50)

typedef struct _student {
	char name [MAX_SIZE];
	char surname [MAX_SIZE];
	double points;
}Student;

int readNoRowsInFile();
Student* allocateMemoryAndReadStudents(int);
int showStudents(int, Student*);
double calculateRelativePoints(double);


int main() {
	int noRows = 0;
	Student* students = NULL;

	noRows = readNoRowsInFile();
	if (noRows > 0) {
		students = allocateMemoryAndReadStudents(noRows);
		showStudents(noRows, students);

		free(students);
	}

	return EXIT_SUCCESS;
}

int readNoRowsInFile() {
	int counter = 0;
	FILE* filePointer = NULL;
	char name[MAX_SIZE], surname[MAX_SIZE];
	double points;

	filePointer = fopen("Students.txt", "r");
	if (!filePointer) {
		printf("Neuspjesno otvaranje \n");
		return -1;
	}

	while (fscanf(filePointer, " %s %s %lf", name, surname, &points) == 3) {
		counter++;
	}

	fclose(filePointer);

	return counter;
}

Student* allocateMemoryAndReadStudents(int noStudents) {
	int counter = 0;
	FILE* filePointer = NULL;
	Student* students = NULL;

	students = (Student*)malloc(noStudents * sizeof(Student));
	if (!students) {
		printf("Neuspjesna alokacija memorije \n");
		return NULL;
	}

	filePointer = fopen("Students.txt", "r");
	if (!filePointer) {
		printf("Neuspjesno otvaranje datoteke \n");
		free(students);
		return NULL;
	}

	while (fscanf(filePointer, " %s %s %lf", students[counter].name, students[counter].surname, &students[counter].points) == 3) {
		counter++;
	}

	fclose(filePointer);
	return students;

}

int showStudents(int noStudents, Student* students) {
	for (int counter = 0; counter < noStudents; counter++) {
		double relPoints = calculateRelativePoints(students[counter].points);
		printf("Ime: %s \t Prezime: %s \t Bodovi: %lf \t Relativni bodovi: %lf%% \n", students[counter].name, students[counter].surname, students[counter].points, relPoints);

	}
	return EXIT_SUCCESS;
}

double calculateRelativePoints(double points) {
	return (points / MAX_POINTS) * 100.0;
}































































