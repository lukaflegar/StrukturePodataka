/*
2. Definirati strukturu osoba(ime, prezime, godina roðenja) i napisati program koji :
	A.dinamièki dodaje novi element na poèetak liste,
	B.ispisuje listu,
	C.dinamièki dodaje novi element na kraj liste,
	D.pronalazi element u listi(po prezimenu),
	E.briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 50

struct _osoba;
typedef struct _osoba* Pozicija;

typedef struct _osoba {
	char ime[MAX_SIZE];
	char prezime[MAX_SIZE];
	int god;

	Pozicija Next;

} osoba;

int UnosP(Pozicija P);
int UnosK(Pozicija P);
int Ispis(Pozicija P);
int TraziPrezime(Pozicija P);
int Brisi(Pozicija P);


int main() {
	
	osoba Head;
	Head.Next = NULL;
	int Choice;

    while (1) {
        printf("Unesi 1 (unos na pocetak), 2 (unos na kraj), 3 (ispis), 4 (trazi po prezimenu), 5 (brisi), 6 (izlaz)\n");
        (void)scanf("%d", &Choice);

        switch (Choice) {
        case 1:
            UnosP(&Head);
            break;
        case 2:
            UnosK(&Head);
            break;
        case 3:
            Ispis(Head.Next);
            break;
        case 4:
            TraziPrezime(Head.Next);
            break;
        case 5:
            Brisi(&Head);
            break;
        case 6:
            return 0;
        default:
            printf("Nisi unio ispravan broj, pokusaj ponovno!\n");
            break;
        }
    }
	
	return EXIT_SUCCESS;
}

int UnosP(Pozicija P) {
    
    Pozicija Q;
    Q = (Pozicija)malloc(sizeof(osoba));
    
    if (Q == NULL) {
        printf("Neuspjesna alokacija memorije.\n");
        return 1;
    }

    printf("Unesi ime osobe: ");
    (void)scanf("%s", Q->ime);
    
    printf("Unesi prezime osobe: ");
    (void)scanf("%s", Q->prezime);
    
    printf("Unesi godinu rodenja osobe: ");
    (void)scanf("%d", &Q->god);

    Q->Next = P->Next;           //Pomièemo pokazivaèe
    P->Next = Q;

    return 0;
}

int UnosK(Pozicija P) {

    Pozicija Q;
    Q = (Pozicija)malloc(sizeof(osoba));

    if (Q == NULL) {
        printf("Neuspjesna alokacija memorije.\n");
        return 1;
    }

    while (P->Next != NULL)                                      //Funkcija je ista kao i UnosP samo prvo moramo prošetati do kraja liste
        P = P->Next;

    printf("Unesi ime osobe: ");
    (void)scanf("%s", Q->ime);

    printf("Unesi prezime osobe: ");
    (void)scanf("%s", Q->prezime);

    printf("Unesi godinu rodenja osobe: ");
    (void)scanf("%d", &Q->god);

    Q->Next = P->Next;
    P->Next = Q;

    return 0;
}

int Ispis(Pozicija P) {

    if (P == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    while (P != NULL) {                                          //Dokle god postoji element u listi
        printf("%s %s %d \n", P->ime, P->prezime, P->god);
        P = P->Next;
    }

    return 0;
}

int TraziPrezime(Pozicija P) {
    char prez[30];
    printf("Unesi prezime osobe koju zelis pronaci:\n");
    (void)scanf("%s", prez);

    while (P != NULL && strcmp(prez, P->prezime) != 0) {         // ... != 0 znaèi da nisu isti
        P = P->Next;
    }

    if (P == NULL) {
        printf("Ne postoji osoba pod tim prezimenom.\n");
    }
    else {
        printf("Pronadena osoba: %s %s %d\n", P->ime, P->prezime, P->god);
    }

    return 0; 
}


int Brisi(Pozicija P) {

    char prez[30];
    Pozicija temp = NULL;

    printf("Unesi prezime osobe koju zelis pronaci:\n");
    (void)scanf("%s", prez);

    while (P->Next != NULL && strcmp(prez, P->Next->prezime) != 0) {
        P = P->Next;
    }

    if (P->Next == NULL) {
        printf("Osoba s prezimenom %s ne postoji.\n", prez);
        return 1;
    }

    temp = P->Next;
    P->Next = temp->Next;
    free(temp);

    printf("Osoba s prezimenom %s je uspjesno izbrisana.\n", prez);

    return 0;
}












