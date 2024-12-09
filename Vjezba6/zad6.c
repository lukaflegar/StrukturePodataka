/*

ZADATAK 6	Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
			predstavljaju pojedini raèun.Na poèetku svake datoteke je zapisan datum u kojem vremenu je
			raèun izdat u formatu YYYY - MM - DD.Svaki sljedeæi red u datoteci predstavlja artikl u formatu
			naziv, kolièina, cijena.Potrebno je formirati vezanu listu raèuna sortiranu po datumu.Svaki èvor
			vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla.Nakon toga potrebno je
			omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni
			artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.

*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH (50)
#define MAX_DATE_LENGTH (11)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)


typedef struct _article* articlePosition;
typedef struct _article {
	char name[MAX_LENGTH];
	int quantity;
	int price;
	articlePosition next;
}article;

typedef struct _receipt* receiptPosition;
typedef struct _receipt {
	char receiptDate[MAX_DATE_LENGTH];
	article articleHead;
	receiptPosition next;

}receipt;


int loadReceipts(receiptPosition receiptHead);
receiptPosition createReceipt(char* receiptName);
articlePosition createArticle(char* articleName, int articleQuantity, int articlePrice);
int addArticleSorted(articlePosition articleHead, articlePosition newArticleNode);
int addReceiptSorted(receiptPosition receiptHead, receiptPosition newReceiptNode);
int compareDates(char* date1, char* date2);
int mergeReceipts(receiptPosition newReceiptNode, receiptPosition currentReceiptInList);
int findArticlesInTimeFrame(receiptPosition receiptHead, char* wantedArticleName, char* dateFrom, char* dateTo);
int printArticlesInTimeFrame(receiptPosition firstReceipt, receiptPosition lastReceipt, char* wantedArticleName);
int printReceiptsAndArticles(receiptPosition receiptHead);
int freeArticle(articlePosition articleHead);
int freeReceipt(receiptPosition receiptHead);


int main() {
	receipt receiptHead = { {0},{{0},0,0,NULL},NULL };
	char dateFrom[MAX_LENGTH] = { '\0' };
	char dateTo[MAX_LENGTH] = { '\0' };
	char articleName[MAX_LENGTH] = { '\0' };

	loadReceipts(&receiptHead);

	printf("\nReceipts and articles:\n");
	printReceiptsAndArticles(&receiptHead);


	printf("\nEnter the name of the article and the time period in the format 'article YYYY-MM-DD YYYY-MM-DD' to find out the total quantity and price of that article: ");
	scanf("%s %s %s", articleName, dateFrom, dateTo);

	findArticlesInTimeFrame(&receiptHead, articleName, dateFrom, dateTo);

	freeReceipt(&receiptHead);

	return EXIT_SUCCESS;
}

//Funkcija za citanje imena racuna iz datoteke racuni.txt
int loadReceipts(receiptPosition receiptHead) {
	FILE* filename = NULL;
	char receiptName[MAX_LENGTH] = { '\0' };
	receiptPosition newReceiptNode = NULL;

	filename = fopen("racuni.txt", "r");

	if (!filename) {
		printf("\nError opening file!\n");
		return FILE_NOT_OPEN;
	}

	while (!feof(filename)) {
		fscanf(filename, " %s ", receiptName);
		newReceiptNode = createReceipt(receiptName);
		addReceiptSorted(receiptHead, newReceiptNode);
	}

	fclose(filename);

	return EXIT_SUCCESS;
}

//Funkcija za stvaranje cvora racuna s podatcima iz datoteke
receiptPosition createReceipt(char* receiptName) {
	FILE* filename = NULL;
	char receiptDate[MAX_LENGTH] = { 0 }, articleName[MAX_LENGTH];
	int articleQuantity = 0, articlePrice = 0;
	receiptPosition newReceiptNode = NULL;
	articlePosition newArticleNode = NULL;

	filename = fopen(receiptName, "r");

	if (!filename) {
		printf("Error opening file: %s\n", receiptName);
		return NULL;
	}

	fscanf(filename, "%s ", receiptDate);
	
	newReceiptNode = malloc(sizeof(receipt));
	if (newReceiptNode == NULL) {
		printf("Memory allocation failed\n");
		return FAILED_MEMORY_ALLOCATION;
	}

	strcpy(newReceiptNode->receiptDate, receiptDate);
	newReceiptNode->next = NULL;
	newReceiptNode->articleHead.next = NULL;

	while (!feof(filename)) {
		fscanf(filename, " %s %d %d ", articleName, &articleQuantity, &articlePrice);
		newArticleNode = createArticle(articleName, articleQuantity, articlePrice);
		addArticleSorted(&newReceiptNode->articleHead, newArticleNode);
	}


	fclose(filename);

	return newReceiptNode;
}

//Funkcija za stvaranje cvora artikala s podatcima iz datoteke, poziva se u funkciji createReceipt
articlePosition createArticle(char* articleName, int articleQuantity, int articlePrice) {
	articlePosition newArticleNode = NULL;

	newArticleNode = malloc(sizeof(article));
	if (newArticleNode == NULL) {
		printf("Memory allocation failed\n");
		return FAILED_MEMORY_ALLOCATION;
	}

	strcpy(newArticleNode->name, articleName);
	newArticleNode->quantity = articleQuantity;
	newArticleNode->price = articlePrice;
	newArticleNode->next = NULL;

	return newArticleNode;
}

//Funkcija za sortirani unos artikala
int addArticleSorted(articlePosition articleHead, articlePosition newArticleNode) {
	articlePosition current = articleHead;

	while (current->next != NULL && strcmp(newArticleNode->name, current->next->name) > 0)
		current = current->next;

	if (current->next != NULL && strcmp(newArticleNode->name, current->next->name) == 0) {
		current->next->quantity += newArticleNode->quantity;
		current->next->price += newArticleNode->price * newArticleNode->quantity;
	}

	else {
		newArticleNode->next = current->next;
		current->next = newArticleNode;
	}

	return EXIT_SUCCESS;
}

//Funkcija za sortirani unos racuna
int addReceiptSorted(receiptPosition receiptHead, receiptPosition newReceiptNode) {
	char receiptDate[MAX_LENGTH] = { '\0' };
	receiptPosition current = receiptHead;

	while (current->next != NULL && compareDates(newReceiptNode->receiptDate, current->next->receiptDate) == 1)
		current = current->next;


	if (current->next != NULL && compareDates(newReceiptNode->receiptDate, current->next->receiptDate) == 0) {
		mergeReceipts(newReceiptNode, current->next);
		free(newReceiptNode);
	}

	else {
		newReceiptNode->next = current->next;
		current->next = newReceiptNode;
	}

	return EXIT_SUCCESS;
}

//Funkcija za usporedbu datuma, potrebna nam je za sortirani unos
int compareDates(char* date1, char* date2) {
	int year1 = 0, year2 = 0, month1 = 0, month2 = 0, day1 = 0, day2 = 0;

	sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
	if (sscanf(date1, "%d-%d-%d", &year1, &month1, &day1) != 3) {
		printf("Error: Invalid date format for date1: %s\n", date1);
		return -2;
	}

	sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);
	if (sscanf(date2, "%d-%d-%d", &year2, &month2, &day2) != 3) {
		printf("Error: Invalid date format for date2: %s\n", date2);
		return -2;
	}

	if (year1 > year2)
		return 1; //ako je drugi raniji
	else if (year1 < year2)
		return -1; //ako je drugi kasniji

	if (month1 > month2)
		return 1;
	else if (month1 < month2)
		return -1;

	if (day1 > day2)
		return 1;
	else if (day1 < day2)
		return -1;

	return 0; //ako su isti
}

//Funkcija za spajanje racuna istih datuma
int mergeReceipts(receiptPosition newReceiptNode, receiptPosition currentReceiptInList) {
	articlePosition current = &newReceiptNode->articleHead, temp = NULL;

	while (current->next != NULL) {
		temp = createArticle(current->next->name, current->next->quantity, current->next->price);

		addArticleSorted(&currentReceiptInList->articleHead, temp);

		current = current->next;
	}

	return EXIT_SUCCESS;
}

//Funkcija za pronalazenje racuna i artikla u odredenom vremenskom periodu
int findArticlesInTimeFrame(receiptPosition receiptHead, char* wantedArticleName, char* dateFrom, char* dateTo) {
	receiptPosition current = receiptHead, firstReceipt = NULL, lastReceipt = NULL;

	while (current->next != NULL) {
		
		if (compareDates(dateFrom, current->next->receiptDate) <= 0) {
			firstReceipt = current->next;
			break;
		}
		current = current->next;
	}

	current = receiptHead;

	while (current->next != NULL) {
	
		if (compareDates(dateTo, current->next->receiptDate) >= 0) {
			lastReceipt = current->next;
		}
		current = current->next;
	}

	if (firstReceipt == NULL || lastReceipt == NULL)
		printf("\nThere are no receipts in the requested time period!\n");
	else
		printArticlesInTimeFrame(firstReceipt, lastReceipt, wantedArticleName);

	return EXIT_SUCCESS;
}

//Funkcija za ispis racuna i artikala u odredenom vremenskom periodu, poziva se u funkciji findArticlesInTimeFrame
int printArticlesInTimeFrame(receiptPosition firstReceipt, receiptPosition lastReceipt, char* wantedArticleName) {
	receiptPosition receiptCurrent = firstReceipt;
	articlePosition articleCurrent = NULL;
	int totalQuantity = 0, totalPrice = 0;

	if (firstReceipt == lastReceipt) {
		articleCurrent = &receiptCurrent->articleHead;

		while (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName) != 0)
			articleCurrent = articleCurrent->next;

		if (articleCurrent->next != NULL) {
			articleCurrent->next->price = articleCurrent->next->price * articleCurrent->next->quantity;
			printf("\nTotal quantity is %d and it costs %d euros!\n", articleCurrent->next->quantity, articleCurrent->next->price);
		}

		else
			printf("\nThe article was not purchased in the requested time period!\n");
	}

	else {

		while (receiptCurrent != lastReceipt->next) {

			articleCurrent = &receiptCurrent->articleHead;

			while (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName) != 0)
				articleCurrent = articleCurrent->next;


			if (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName) == 0) {
				totalQuantity += articleCurrent->next->quantity;
				totalPrice += articleCurrent->next->price * articleCurrent->next->quantity;
			}

			receiptCurrent = receiptCurrent->next;
		}

		if (totalQuantity != 0)
			printf("\nTotal quantity is %d and it costs %d euros!\n", totalQuantity, totalPrice);


		else
			printf("\nThe article was not purchased in the requested time period!\n");
	}

	return EXIT_SUCCESS;
}

//Funkcija za ispis sortiranih racuna i artikala
int printReceiptsAndArticles(receiptPosition receiptHead) {
	receiptPosition currentReceipt = receiptHead->next;
	articlePosition currentArticle = NULL;

	while (currentReceipt) {
		printf("Receipt date: %s\n", currentReceipt->receiptDate);

		currentArticle = currentReceipt->articleHead.next;
		if (!currentArticle) {
			printf("There are no articles in this receipt.\n");
		}

		while (currentArticle) {
			printf("  Article: %s, Quantity: %d, Price: %d EUR\n",
				currentArticle->name, currentArticle->quantity, currentArticle->price);
			currentArticle = currentArticle->next;
		}

		printf("\n");
		currentReceipt = currentReceipt->next;
	}
	return 0;
}

//Funkcija za oslobadanje memorije artikala, zasebno se poziva u funkciji za oslobadanje memorije racuna
int freeArticle(articlePosition articleHead) { 
	articlePosition current = articleHead->next;
	articlePosition temp = NULL;

	while (current != NULL) {
		temp = current;
		current = current->next;
		free(temp); 
	}

	return 0;
}

//Funkcija za oslobadanje memorija racuna nakon što se oslobode svi artikli u racunu
int freeReceipt(receiptPosition receiptHead) {
	receiptPosition current = receiptHead->next;
	receiptPosition temp = NULL;

	while (current != NULL) {
		freeArticle(&current->articleHead);
		temp = current;
		current = current->next;
		free(temp); 
	}

	return 0;
}

