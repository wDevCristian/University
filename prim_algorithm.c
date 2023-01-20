#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <limits.h>
#include <time.h>

// --Macrodefinitii
#define USINT unsigned short     
#define RED printf("\033[1;31m")
#define GREEN printf("\033[1;32m");
#define RESET_COLOR printf("\033[0m")
#define BOLD printf("\033[1;37m")
#define VERTICES 3

// --Variabile globale 
char color[VERTICES] = { 'a', 'a', 'a' };
int sp[VERTICES] = { 0, 0, 0 };
USINT timeP;


typedef struct prim {
	int node;
	USINT isVisited;
	int cost;
	int path;
} primMatrix;

// --Functii 
int **createMatrix(USINT nrNods) {
	int **mx = (int**)malloc(sizeof(int*) * nrNods);
	USINT i;

	for (i = 0; i < nrNods; i++) {
		mx[i] = (int*)malloc(sizeof(int) * nrNods);
	}

	return mx;
}

void initializeMatrix(int **matrix, USINT nrNods) {
	USINT i;
	USINT j;

	for (i = 0; i < nrNods; i++) {
		for (j = 0; j < nrNods; j++) {
			matrix[i][j] = -1;
		}
	}
}

void freeMatrix(int **matrix, USINT nrNods) {
	USINT i;

	for (i = 0; i < nrNods; i++) {
		free(matrix[i]);
		matrix[i] = NULL;
	}

	free(matrix);
	matrix = NULL;
}


void createNode(int node, int **matrix, USINT nrNods) {
	USINT i;

	for (i = 0; i < nrNods; i++) {
		matrix[node][i] = 0;
	}
}


void createArch(int sNode, int eNode, int **matrix, USINT nrNods, int num) {

	if (matrix[sNode][eNode] == 1) {
		RED;
		printf("[ ERROR ] ");
		BOLD;
		printf("Arcul deja a fost introdus.\n");
		return;
	}
	else {
		matrix[sNode][eNode] = num;
		matrix[eNode][sNode] = num;
	}
}

void showMatrix(int **matrix, USINT nrNods) {
	USINT i;
	USINT j;

	printf("\n   ");

	for (i = 0; i < nrNods; i++) {
		printf("[%hu]", i);
	}

	for (i = 0; i < nrNods; i++) {
		printf("\n[%hu]", i);
		for (j = 0; j < nrNods; j++) {
			if (matrix[i][j] < 0) {
				printf("%d ", matrix[i][j]);
				continue;
			}
			printf(" %d ", matrix[i][j]);
		}
	}
}

void delEdge(int sNode, int eNode, int **matrix) {
	matrix[sNode][eNode] = 0;
	matrix[eNode][sNode] = 0;
}

void delNode(int node, int **matrix, USINT nrNods) {
	USINT i;

	for (i = 0; i < nrNods; i++) {
		if (matrix[node][i] == 1) {
			delEdge(node, i, matrix);
		}

		matrix[node][i] = -1;
	}

}

void searchInDepth(int node, int **matrix, USINT nrNods) {
	USINT i;
	USINT initTime[VERTICES];
	USINT finalTime[VERTICES];

	color[node] = 'g';
	timeP++;
	initTime[node] = timeP;

	for (i = 0; i < nrNods; i++) {
		if (matrix[node][i] && (color[i] == 'a')) {
			sp[i] = node;
			searchInDepth(i, matrix, nrNods);
		}
	}

	color[node] = 'n';
	printf("%d ", node);
	timeP++;
	finalTime[node] = timeP;
}

void traverseInDepth(int **matrix, USINT nrNods) {
	USINT i;

	timeP = 0;
	for (i = 0; i < nrNods; i++) {
		if (color[i] == 'a') {
			searchInDepth(i, matrix, nrNods);
		}
	}
}

void initPrimMatrix(primMatrix *prim) {
	USINT i;

	for (i = 0; i < VERTICES; i++) {
		(prim + i)->node = i;
		(prim + i)->isVisited = 0;
		(prim + i)->cost = INT_MAX;
		(prim + i)->path = -1;
	}

}


void printPrimVector(primMatrix *prim) {
	USINT i;

	printf("\n[N] [V] [C] [P]\n");
	for (i = 0; i < VERTICES; i++) {
		printf(" %d | %hu | %d | %d\n", (prim + i)->node, (prim + i)->isVisited, (prim + i)->cost, (prim + i)->path);
	}
}


void primAlgorithm(int cNode, int **matrix) {
	USINT i;
	int minCost;
	int minCostIndex = cNode;
	primMatrix prim[VERTICES];

	initPrimMatrix(prim);

	prim[cNode].cost = 0;
	prim[cNode].isVisited = 1;
	prim[cNode].path = cNode;

	do {

		cNode = minCostIndex;
		minCost = INT_MAX;

		for (i = 0; i < VERTICES; i++) {
			if (matrix[cNode][i] > 0 && prim[i].isVisited != 1) {
				if (prim[i].cost > matrix[cNode][i]) {
					prim[i].cost = matrix[cNode][i];
					prim[i].path = cNode;
				}
			}
		}

		for (i = 0; i < VERTICES; i++) {
			if (prim[i].isVisited) continue;

			if (prim[i].cost < minCost) {
				minCost = prim[i].cost;
				minCostIndex = i;
			}
		}

		prim[minCostIndex].isVisited = 1;

	} while (minCostIndex != cNode);

	printPrimVector(prim);
}


int main() {
	USINT opt;
	USINT nrNods;
	USINT i;
	int **matrix = NULL;
	int nod = 0;
	int startNode = 0;
	int endNode = 0;

	// srand(time(0));

	while (1) {
		printf("\nAlegeti una din cele 7 optiuni: \n\n");

		printf("1. Initializare\n");
		printf("2. Introduceti nod\n");
		printf("3. Introduceti arc\n");
		printf("4. Afisare matrice\n");
		printf("5. Stergere nod\n");
		printf("6. Stergere arc\n");
		printf("7. Traversare graf in adancime dupa metoda CRL.\n");
		printf("8. Iesire\n");
		printf("9. Algoritmul lui Prim de identificare a minimului unui graf.\n");
		BOLD;
		printf("Optiune: ");
		RESET_COLOR;
		scanf("%hu", &opt);

		switch (opt)
		{
		case 1:
			nrNods = VERTICES;
			/*printf("Dati nr. de noduri ale unui graf: ");

			do {
				scanf("%hu", &nrNods);
			} while (nrNods > 50);
			*/
			matrix = createMatrix(nrNods);

			if (matrix == NULL) {
				RED;
				printf("[ ERROR ]");
				BOLD;
				printf(" Nu a putut fi alocata memorie pentru matrice.\n");
				RESET_COLOR;
			}
			else {
				initializeMatrix(matrix, nrNods);
				GREEN;
				printf("[ OK ]");
				BOLD;
				printf(" Matricea adiacenta grafului a fost initializata.\n");
				RESET_COLOR;
			}

			for (i = 0; i < nrNods; i++) {
				createNode(i, matrix, nrNods);
			}

			createArch(0, 1, matrix, nrNods, 60);
			createArch(0, 2, matrix, nrNods, 640);
			createArch(1, 2, matrix, nrNods, 600);

			break;
		case 2:
			/*do {
				printf("Introduceti un nod: ");
				scanf("%d", &nod);
			} while (nod >= nrNods || nod < 0);*/

			RED;
			printf("Aceasta optiune a fost dezactivata.\n");
			RESET_COLOR;

			//createNode(matrix, nod, nrNods);

			break;
		case 3:
			/*do {
				printf("Introduceti un arc: '(a, b)' : ");
				scanf("%d", &startNode);
				scanf("%d", &endNode);

			} while (startNode > nrNods || endNode > nrNods || endNode < 0 || startNode < 0);

			createArch(startNode, endNode, matrix, nrNods);*/

			RED;
			printf("Aceasta optiune a fost dezactivata.\n");
			RESET_COLOR;


			break;
		case 4:
			showMatrix(matrix, nrNods);
			break;
		case 5:
			printf("Dati nodul pe care doriti sa il stergeti: ");
			scanf("%d", &nod);

			delNode(nod, matrix, nrNods);
			break;
		case 6:
			printf("Dati arcul pe care doriti sa il stergeti: \nNodul de start: ");
			scanf("%d", &startNode);
			printf("Nodul final: ");
			scanf("%d", &endNode);

			delEdge(startNode, endNode, matrix);
			break;

		case 7:
			traverseInDepth(matrix, nrNods);
			break;

		case 8:
			if (matrix) {
				freeMatrix(matrix, nrNods);
				GREEN;
				printf("[ OK ]");
				BOLD;
				printf(" Zona de memorie cu matricea adiacenta a fost eliberata.\n");
				RESET_COLOR;
				exit(1);
			}
			else {
				exit(1);
			}

		case 9:
			// In cazul dat testez exemplul din laborator, distanta dintre Arad, Timisoara, Bucuresti
			primAlgorithm(1, matrix);
			break;

		default:
			RED;
			printf("[ ERROR ] ");
			BOLD;
			printf("A fost aleasa o optiune inexistenta.\n\n");
			RESET_COLOR;
			break;
		}
	}



	return 0;
}