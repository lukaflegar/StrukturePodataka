#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct _Polinom;
typedef struct _Polinom* Position;
typedef struct _Polinom {
    int Coeff;
    int Exp;
    Position Next;
} Polinom;

int addPoly(Position P, int Coeff, int Exp);
int LoadPoly(const char* filename, Position P);
int PrintPoly(Position P);
Position SumPoly(Position poly1, Position poly2);
Position MultiplyPoly(Position poly1, Position poly2);
int FreePoly(Position P);

int main() {
    Polinom poly1 = { .Exp = 0, .Coeff = 0, .Next = NULL };
    Polinom poly2 = { .Exp = 0, .Coeff = 0, .Next = NULL };


    LoadPoly("polinom1.txt", &poly1);
    LoadPoly("polinom2.txt", &poly2);

    printf("Polinom 1: ");
    PrintPoly(&poly1);

    printf("Polinom 2: ");
    PrintPoly(&poly2);


    Position sum = SumPoly(&poly1, &poly2);
    printf("Zbroj polinoma: ");
    PrintPoly(sum);

   
    Position product = MultiplyPoly(&poly1, &poly2);
    printf("Umnozak polinoma: ");
    PrintPoly(product);

    FreePoly(poly1.Next);
    FreePoly(poly2.Next);
    FreePoly(sum);
    FreePoly(product);

    return 0;
}

int addPoly(Position P, int Coeff, int Exp) {
    Position newEl = (Position)malloc(sizeof(Polinom));

    if (newEl == NULL) {
        printf("Greska pri alociranju memorije!\n");
        return -1;
    }

    newEl->Coeff = Coeff;
    newEl->Exp = Exp;
    newEl->Next = NULL;

    // Pronalaženje pozicije za umetanje novog elementa
    while (P->Next != NULL && P->Next->Exp > Exp) {
        P = P->Next;
    }

    // Provjera da li je eksponent isti i ažuriranje koeficijenta
    if (P->Next != NULL && P->Next->Exp == Exp) {
        P->Next->Coeff += Coeff;
        free(newEl);
    }
    else {
        newEl->Next = P->Next;
        P->Next = newEl;
    }

    return 0;
}

int LoadPoly(const char* filename, Position P) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Greska prilikom otvaranja datoteke");
        return -1;
    }

    int Coeff, Exp;
    while (fscanf(file, "%d %d", &Coeff, &Exp) == 2) {
        addPoly(P, Coeff, Exp);
    }

    fclose(file);

    return 0;
}

int PrintPoly(Position P) {
    Position temp = P->Next;
    while (temp != NULL) {
        printf("%dx^%d", temp->Coeff, temp->Exp);
        temp = temp->Next;
        if (temp != NULL)
            printf(" + ");
    }
    printf("\n");
    return 0;
}

Position SumPoly(Position poly1, Position poly2) {
    Position result = NULL;
    result = (Position)malloc(sizeof(Polinom));

    if (result == NULL) {
        printf("Greska pri alociranju memorije!\n");
        return -1;
    }

    result->Exp = 0;
    result->Coeff = 0;
    result->Next = NULL;

    Position temp1 = poly1->Next; // Prvi èlan polinoma 1
    Position temp2 = poly2->Next; // Prvi èlan polinoma 2

    while (temp1 != NULL || temp2 != NULL) {
        if (temp1 == NULL) {
            addPoly(result, temp2->Coeff, temp2->Exp);
            temp2 = temp2->Next;
        }
        else if (temp2 == NULL) {
            addPoly(result, temp1->Coeff, temp1->Exp);
            temp1 = temp1->Next;
        }
        else if (temp1->Exp > temp2->Exp) {
            addPoly(result, temp1->Coeff, temp1->Exp);
            temp1 = temp1->Next;
        }
        else if (temp1->Exp < temp2->Exp) {
            addPoly(result, temp2->Coeff, temp2->Exp);
            temp2 = temp2->Next;
        }
        else {
            addPoly(result, temp1->Coeff + temp2->Coeff, temp1->Exp);
            temp1 = temp1->Next;
            temp2 = temp2->Next;
        }
    }

    return result;
}

Position MultiplyPoly(Position poly1, Position poly2) {
    Position result = NULL;
    result = (Position)malloc(sizeof(Polinom));
    
    if (result == NULL) {
        printf("Greska pri alociranju memorije!\n");
        return -1;
    }

    result->Exp = 0;
    result->Coeff = 0;
    result->Next = NULL;

    for (Position temp1 = poly1->Next; temp1 != NULL; temp1 = temp1->Next) {
        for (Position temp2 = poly2->Next; temp2 != NULL; temp2 = temp2->Next) {
            addPoly(result, temp1->Coeff * temp2->Coeff, temp1->Exp + temp2->Exp);
        }
    }

    return result;
}

int FreePoly(Position P) {
    Position temp = NULL;

    while (P != NULL) {
        temp = P;
        P = P->Next;
        free(temp);
    }
    return 0;
}
