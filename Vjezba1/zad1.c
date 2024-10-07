#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE (64)
#define MAX_POINTS (50)

typedef struct _student {
	char ime [MAX_SIZE];
	char prezime [MAX_SIZE];
	double bodovi [MAX_POINTS];
}Student;

int ReadNoRowsInFile();
Student* allocateMemoryAndReadStudents(int);
int showStudents(int, Student*);
double calculateRelativePoints(double);


int main() {

}































































