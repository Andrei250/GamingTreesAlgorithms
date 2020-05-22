#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//2 structuri pentru taskul1
typedef struct ListTree {
	struct tree_node *nod;
	struct ListTree *next;
} ListTree;

typedef struct tree_node {
	char **configuration;
	int freePlaces;
	int N, M, isLeaf;
	ListTree *headConfigs;
} tree_node;

//2 structuri pentru taskul 2 si 3
typedef struct ListNode {
	struct Minimax *nod;
	struct ListNode *next;
} ListNode;

typedef struct Minimax {
	int value;
	int level;
	ListNode *head;
} Minimax;

//2 structuri pentru taskul4
typedef struct listParanoic {
	struct Paranoic *node;
	struct listParanoic *next;
} listParanoic;

typedef struct Paranoic {
	int *values;
	int players;
	char *elements;
	listParanoic *head;
} Paranoic;

//aloca memorie pentru un nod de la taskul 1
void alloc_tree_node(tree_node **root, char **matrix, int N, int M,
	int freePlaces, int isLeaf)
{
	*root = malloc(sizeof(tree_node));
	(*root)->N = N;
	(*root)->M = M;
	(*root)->headConfigs = NULL;
	(*root)->configuration = malloc(sizeof(char *) * N);
	(*root)->freePlaces = freePlaces;
	(*root)->isLeaf = isLeaf;

	for (int i = 0; i < N; ++i)
		(*root)->configuration[i] = strdup(matrix[i]);
}

//caut ultima linie libera de pe coloana j in matricea de joc
//de la taskul 1
int findPos(char **matrix, int N, int col)
{
	int j = 0;

	while (j < N && matrix[j][2 * col] == '-')
		j++;

	return j - 1;
}

// verific daca este valida configuratia initiala
int validate(char **matrix, int N, int M)
{
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j) {
			if (i + 3 < N &&
				matrix[i][2 * j] == matrix[i + 1][2 * j] &&
				matrix[i][2 * j] == matrix[i + 2][2 * j] &&
				matrix[i][2 * j] == matrix[i + 3][2 * j] &&
				matrix[i][2 * j] != '-')
				return 1;

			if (j + 3 < M &&
				matrix[i][2 * j] == matrix[i][2 * (j + 1)] &&
				matrix[i][2 * j] == matrix[i][2 * (j + 2)] &&
				matrix[i][2 * j] == matrix[i][2 * (j + 3)] &&
				matrix[i][2 * j] != '-')
				return 1;

			if (i + 3 < N && j + 3 < M &&
				matrix[i][2 * j] == matrix[i + 1][2 * (j + 1)] &&
				matrix[i][2 * j] == matrix[i + 2][2 * (j + 2)] &&
				matrix[i][2 * j] == matrix[i + 3][2 * (j + 3)] &&
				matrix[i][2 * j] != '-')
				return 1;

			if (i + 3 < N && j - 3 >= 0 &&
				matrix[i][2 * j] == matrix[i + 1][2 * (j - 1)] &&
				matrix[i][2 * j] == matrix[i + 2][2 * (j - 2)] &&
				matrix[i][2 * j] == matrix[i + 3][2 * (j - 3)] &&
				matrix[i][2 * j] != '-')
				return 1;
		}

	return 0;
}

//verific daca noul nod este frunza sau nu
int isOk(char **matrix, int i, int j, int N, int M)
{
	int counter = 0;
	int savedI = i, savedJ = j;

    //verificare pe coloana
    //verific doar in joc, deoarece presupun ca acolo il pun deci
    //is sus nu se afla nimic
	while (savedI < N) {
		if (matrix[savedI][2 * j] != matrix[i][2 * j])
			break;
		counter++;
		savedI++;
	}

	if (counter >= 4)
		return 1;

	counter = 0;
	savedI = i;

    //verificat linie
    //verific si in stanga si in dreapta
	while (savedJ < M) {
		if (matrix[i][2 * savedJ] != matrix[i][2 * j])
			break;
		counter++;
		savedJ++;
	}

	savedJ = j - 1;

	while (savedJ >= 0) {
		if (matrix[i][2 * savedJ] != matrix[i][2 * j])
			break;
		counter++;
		savedJ--;
	}

	if (counter >= 4)
		return 1;

    //diagonala principala
	counter = 0;
	savedI = i;
	savedJ = j;

	while (savedJ < M && savedI < N) {
		if (matrix[savedI][2 * savedJ] != matrix[i][2 * j])
			break;
		counter++;
		savedJ++;
		savedI++;
	}

	savedI = i - 1;
	savedJ = j - 1;

	while (savedJ >= 0 && savedI >= 0) {
		if (matrix[savedI][2 * savedJ] != matrix[i][2 * j])
			break;
		counter++;
		savedJ--;
		savedI--;
	}

	if (counter >= 4)
		return 1;

    //diagonala secundara
	counter = 0;
	savedI = i;
	savedJ = j;

	while (savedJ < M && savedI >= 0) {
		if (matrix[savedI][2 * savedJ] != matrix[i][2 * j])
			break;
		counter++;
		savedJ++;
		savedI--;
	}

	savedI = i + 1;
	savedJ = j - 1;

	while (savedJ >= 0 && savedI < N) {
		if (matrix[savedI][2 * savedJ] != matrix[i][2 * j])
			break;
		counter++;
		savedJ--;
		savedI++;
	}

	if (counter >= 4)
		return 1;

	return 0;
}

//construiesc arborele de la taskul 1
void createTree(tree_node *root, char NextPlayer)
{
	if (root->freePlaces && root->isLeaf == 0) { //daca nu e frunza continui
		for (int i = root->M - 1; i >= 0; --i)
			if (root->configuration[0][2 * i] == '-') {
				int position = findPos(root->configuration, root->N, i);

				root->configuration[position][2 * i] = NextPlayer;
				tree_node *newNode;

				ListTree *ListNode = malloc(sizeof(ListTree));

				int isLeaf = isOk(root->configuration, position, i, root->N,
						root->M);
				//daca se afla pe prima linie atunci se scade un spatiu liber
				if (position == 0)
					alloc_tree_node(&newNode, root->configuration, root->N,
						root->M, root->freePlaces - 1, isLeaf);
				else
					alloc_tree_node(&newNode, root->configuration, root->N,
						root->M, root->freePlaces, isLeaf);

				ListNode->next = NULL;
				ListNode->nod = newNode;
				root->configuration[position][2 * i] = '-';

				if (root->headConfigs == NULL)
					root->headConfigs = ListNode;
				else {
					ListNode->next = root->headConfigs;
					root->headConfigs = ListNode;
				}
			}

		//schimb jucatorul
		char Player = (NextPlayer == 'R') ? 'B' : 'R';
		ListTree *headList = root->headConfigs;

		//continui creearea arborelui apeland functia pentru fiecare copil
		while (headList) {
			createTree(headList->nod, Player);
			headList = headList->next;
		}
	}
}

//afisez arborele de la taskul 1 in fisierul de iesire
void printTree(tree_node *root, FILE *fila, int numberOfTabs)
{
	for (int i = 0; i < root->N; ++i) {
		for (int k = 0; k < numberOfTabs; ++k)
			fputs("\t", fila);

		fputs(root->configuration[i], fila);
		fputc('\n', fila);
	}

	fputc('\n', fila);
	ListTree *headList = root->headConfigs;

	while (headList) {
		printTree(headList->nod, fila, numberOfTabs + 1);
		headList = headList->next;
	}
}

//functie care adauga un element la finalul unei liste
void push_back(ListNode **root, ListNode **end, Minimax *node)
{
	ListNode *new = malloc(sizeof(ListNode));

	new->nod = node;
	new->next = NULL;

	if (*root == NULL) {
		*root = new;
		*end = new;
	} else {
		(*end)->next = new;
		(*end) = (*end)->next;
	}
}

//construiesc arborele de la taskul 2 si 3
void buildTree(ListNode **root, int H, char **matrix)
{
	char *p;
	ListNode *newList = NULL, *oldList = NULL, *end = NULL;

	p = strtok(matrix[H - 1], " ");

	//iau elementele de pe ultima linie si le pun intr-o lista de noduri
	while (p) {
		p = p + 1;
		p[strlen(p) - 1] = '\0';
		int number = atoi(p);
		Minimax *newNode = malloc(sizeof(Minimax));

		newNode->level = H - 1;
		newNode->value = number;
		newNode->head = NULL;
		push_back(&oldList, &end, newNode);
		p = strtok(NULL, " ");
	}

	if (end)
		end = end->next;

	//parcurg de jos in sus matricea citita, transform informatia in noduri
	//si apoi de fiecare data le bag intr-o noua lista, iar la final lista
	//noua o pun in cea veche si repet asta pana termin toata matricea
	//in lista finala se va afla doar nodul root
	for (int i = H - 2; i >= 0; --i) {
		newList = NULL;
		end = NULL;
		p = strtok(matrix[i], " ");

		while (p) {
			if (p[0] == '[') {
				p = p + 1;
				p[strlen(p) - 1] = '\0';
				int number = atoi(p);
				Minimax *newNode = malloc(sizeof(Minimax));

				newNode->level = i;
				newNode->value = number;
				newNode->head = NULL;
				push_back(&newList, &end, newNode);
			} else {
				p = p + 1;
				p[strlen(p) - 1] = '\0';
				int number = atoi(p);

				int counter = 0;
				ListNode *newhead = oldList;
				//initializez un minim sau un maxim in functie de nivel
				int value = ((i + 1) % 2 == 0) ? 99999999 : -99999999;

				while (oldList && counter < number - 1) {
					counter++;
					if ((i + 1) % 2 == 0)
						value = fmin(value, oldList->nod->value);
					else
						value = fmax(value, oldList->nod->value);

					oldList = oldList->next;
				}

				ListNode *last = oldList;

				if (oldList) {
					if ((i + 1) % 2 == 0)
						value = fmin(value, oldList->nod->value);
					else
						value = fmax(value, oldList->nod->value);
					oldList = oldList->next;
					last->next = NULL;
				}

				Minimax *newNode = malloc(sizeof(Minimax));

				newNode->level = i;
				newNode->value = value;
				newNode->head = newhead;
				push_back(&newList, &end, newNode);
			}

			p = strtok(NULL, " ");
		}
		oldList = newList;
	}

	*root = newList;
}

//afisez arborele de la taskurile 2 si 3
void printMinimaxTree(Minimax *root, FILE *fila, int nubmerOfTabs)
{
	for (int k = 0; k < nubmerOfTabs; ++k)
		fputs("\t", fila);

	char str[12];

	sprintf(str, "%d", root->value);
	fputs(str, fila);
	fputc('\n', fila);

	ListNode *headList = root->head;

	while (headList) {
		printMinimaxTree(headList->nod, fila, nubmerOfTabs + 1);
		headList = headList->next;
	}
}

//dealoc elementele unu arbore de la taskurile 2 si 3
void deallocMinimaxTree(Minimax *root)
{
	if (root == NULL)
		return;

	ListNode *head;

	while (root->head) {
		deallocMinimaxTree(root->head->nod);
		head = root->head;
		root->head = root->head->next;
		free(head);
	}

	free(root);
}

//reconstruiesc arborele de a taskul 3 folosind algoritmul alfa-beta prunig
int rebuildTree(Minimax *root, int Level, int alfa, int beta)
{
	if (root->head == NULL)
		return root->value;

	if (Level % 2 == 0) {
		int value = 99999999;

		ListNode *headList = root->head;

		while (headList) {
			//caut valori mai bune in sub-arbori
			//daca nu gasesc, conditia beta <= alfa va oprii cautaera
			value = fmin(value, rebuildTree(headList->nod,
								Level + 1, alfa, beta));

			beta = fmin(beta, value);

			//daca beta devine <= alfa atunci nu mai are rost sa caut
			//deoarece nivelul Max de deasupra il va anula oricum
			//deci dealoc restul subarborilor acestui nod
			//exceptand ce am cautat pana acum
			if (beta <= alfa) {
				ListNode *anotherList = headList->next;

				while (anotherList) {
					deallocMinimaxTree(anotherList->nod);
					ListNode *ANL = anotherList;

					anotherList = anotherList->next;
					free(ANL);
				}
				break;
			}

			headList = headList->next;
		}

		//nodul curent ia o noua valoare
		root->value = value;
		if (headList)
			headList->next = NULL;
		return value;
	} else {
		int value = -99999999;

		ListNode *headList = root->head;

		while (headList) {
			//caut valori mai bune in sub-arbori
			//daca nu gasesc, conditia beta <= alfa va oprii cautaera
			value = fmax(value, rebuildTree(headList->nod,
								Level + 1, alfa, beta));

			alfa = fmax(alfa, value);

			//daca beta devine <= alfa atunci nu mai are rost sa caut
			//deoarece nivelul Min de deasupra il va anula oricum
			//deci dealoc restul subarborilor acestui nod
			//exceptand ce am cautat pana acum
			if (beta <= alfa) {
				ListNode *anotherList = headList->next;

				while (anotherList) {
					deallocMinimaxTree(anotherList->nod);
					ListNode *ANL = anotherList;

					anotherList = anotherList->next;
					free(ANL);
				}
				break;
			}
			headList = headList->next;
		}

		//nodul curent ia o noua valoare
		root->value = value;
		if (headList)
			headList->next = NULL;
		return value;
	}
}

//adaug un element la finalul unei liste de tip listParanoic
void push_back_paranoic(listParanoic **root, Paranoic *node)
{
	listParanoic *head = *root;
	listParanoic *new = malloc(sizeof(listParanoic));

	new->node = node;
	new->next = NULL;

	if (*root == NULL)
		*root = new;
	else {
		while (head->next != NULL)
			head = head->next;

		head->next = new;
	}
}
//dealoc un arbore de la taskul 4
void deallocParanoic(Paranoic *root)
{
	if (root == NULL)
		return;

	listParanoic *head;

	while (root->head) {
		deallocParanoic(root->head->node);
		head = root->head;
		root->head = root->head->next;
		free(head);
	}

	free(root->elements);
	free(root->values);
	free(root);
}

//construiesc arborele de la taskul 4 pe aceeasi idee ca la taskul 2
//doar ca aici tin cont ca sunt Players - 1 nivele Min si 1 nivel Max
//De asemenea, retin fiecare nod ca string pentru a afisa mai usor
void paranoicTree(listParanoic **root, int H, int Players,
	char **matrix)
{
	char *p;

	p = strtok(matrix[H - 1], " ");

	listParanoic *newList = NULL, *oldList = NULL;

	//formez o lista cu nodurile d epe ultimul nivel (frunze)
	while (p) {
		Paranoic *newNode = malloc(sizeof(Paranoic));

		newNode->elements = strdup(p);
		p = p + 1;
		p[strlen(p) - 1] = '\0';
		newNode->players = Players;
		newNode->values = malloc(sizeof(int) * Players);

		int i = strlen(p) - 1, count = 0;
		char number[9];
		int *elm = malloc(sizeof(int) * Players);

		while (i >= 0) {
			while (i > 0 && p[i] != ',')
				i--;
			if (i == 0) {
				strcpy(number, p + i);
				elm[count++] = atoi(number);
			} else {
				strcpy(number, p + i + 1);
				elm[count++] = atoi(number);
				p[i] = '\0';
			}

			i--;
		}

		for (int i = Players - 1; i >= 0; --i)
			newNode->values[Players - i - 1] = elm[i];
		newNode->head = NULL;
		push_back_paranoic(&oldList, newNode);
		free(elm);

		p = strtok(NULL, " ");
	}

	//pentru fiecare nivel creez alta lista ca la taskul 2 cu nodurile
	//de pe acel nivel, iar la final noua lista o retin in alta lista
	//si repet asta pana cand ajung la radacina
	for (int k = H - 2; k >= 0; --k) {
		p = strtok(matrix[k], " ");
		newList = NULL;

		while (p) {
			if (p[0] == '[') { // frunza
				Paranoic *newNode = malloc(sizeof(Paranoic));

				newNode->elements = strdup(p);
				p = p + 1;
				p[strlen(p) - 1] = '\0';
				newNode->players = Players;
				newNode->values = malloc(sizeof(int) * Players);

				int i = strlen(p) - 1, count = 0;
				char number[9];
				int *elm = malloc(sizeof(int) * Players);

				while (i >= 0) {
					while (i > 0 && p[i] != ',')
						i--;
					if (i == 0) {
						strcpy(number, p + i);
						elm[count++] = atoi(number);
					} else {
						strcpy(number, p + i + 1);
						elm[count++] = atoi(number);
						p[i] = '\0';
					}

					i--;
				}

				for (int i = Players - 1; i >= 0; --i)
					newNode->values[Players - i - 1] = elm[i];
				newNode->head = NULL;
				push_back_paranoic(&newList, newNode);
				free(elm);
			} else { // nod intern
				p = p + 1;
				p[strlen(p) - 1] = '\0';
				int number = atoi(p);

				int counter = 0;
				listParanoic *newhead = oldList;
				int value = ((k + 1) % Players == 1) ? -99999 : 99999;
				int *vals = malloc(sizeof(int) * Players);
				char *elements = NULL;

				//parcurg lista veche si fac nodurile copii pentru fiecare
				//nod intern
				while (oldList && counter < number - 1) {
					counter++;
					if ((k + 1) % Players != 1) {
						if (value > oldList->node->values[0]) {
							value = oldList->node->values[0];

							for (int i = 0; i < Players; ++i)
								vals[i] = oldList->node->values[i];

							if (elements)
								free(elements);

							elements = strdup(oldList->node->elements);
						}
					} else {
						if (value < oldList->node->values[0]) {
							value = oldList->node->values[0];

							for (int i = 0; i < Players; ++i)
								vals[i] = oldList->node->values[i];

							if (elements)
								free(elements);
							elements = strdup(oldList->node->elements);
						}
					}

					oldList = oldList->next;
				}

				listParanoic *last = oldList;

				if (oldList) {
					if ((k + 1) % Players != 1) {
						if (value > oldList->node->values[0]) {
							value = oldList->node->values[0];

							for (int i = 0; i < Players; ++i)
								vals[i] = oldList->node->values[i];

							if (elements)
								free(elements);

							elements = strdup(oldList->node->elements);
						}
					} else {
						if (value < oldList->node->values[0]) {
							value = oldList->node->values[0];

							for (int i = 0; i < Players; ++i)
								vals[i] = oldList->node->values[i];
							if (elements)
								free(elements);

							elements = strdup(oldList->node->elements);
						}
					}
					oldList = oldList->next;
					last->next = NULL;
				}

				Paranoic *newNode = malloc(sizeof(Paranoic));

				newNode->players = Players;
				newNode->values = malloc(sizeof(int) * Players);
				newNode->elements = strdup(elements);
				for (int i = 0; i < Players; ++i)
					newNode->values[i] = vals[i];
				newNode->head = newhead;
				push_back_paranoic(&newList, newNode);
				free(elements);
				free(vals);
			}

			p = strtok(NULL, " ");
		}
		oldList = newList;
	}

	*root = newList;
}

//afisez un arbore de la taskul 4
void printParanoic(Paranoic *root, FILE *fila, int nubmerOfTabs)
{
	for (int k = 0; k < nubmerOfTabs; ++k)
		fputs("\t", fila);
	fputs(root->elements, fila);
	fputc('\n', fila);

	listParanoic *headList = root->head;

	while (headList) {
		printParanoic(headList->node, fila, nubmerOfTabs + 1);
		headList = headList->next;
	}
}

//dealoc memoria unui arbore de la taskul 1
void deallocTreeNode(tree_node *root)
{
	if (root == NULL)
		return;

	ListTree *head;

	while (root->headConfigs) {
		deallocTreeNode(root->headConfigs->nod);
		head = root->headConfigs;
		root->headConfigs = root->headConfigs->next;
		free(head);
	}

	for (int i = 0; i < root->N; ++i)
		free(root->configuration[i]);
	free(root->configuration);
	free(root);
}

//pentru fiecare task citesc input-ul intr-o matrice
int main(int argc, char **argv)
{

	if (strcmp(argv[1], "-c1") == 0) {
		char **config;
		char NextPlayer;
		char *pointer, *line;
		int N, M;
		tree_node *root;
		FILE *fila = fopen(argv[2], "r");
		char string[50];

		fgets(string, 50, fila);
		string[strlen(string) - 1] = '\0';
		pointer = strtok(string, " ");
		N = atoi(pointer);
		pointer = strtok(NULL, " ");
		M = atoi(pointer);
		pointer = strtok(NULL, " ");
		NextPlayer = *pointer;
		int length = 2 * (M + 1) + 1;

		line = malloc(sizeof(char) * length);
		config = malloc(sizeof(char *) * N);

		for (int i = 0; i < N; ++i) {
			fgets(line, length, fila);
			if (line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';
			config[i] = strdup(line);
		}
		fclose(fila);
		int freePlaces = 0;

		for (int i = 0; i < M; ++i) {
			int j = 0;

			while (j < N && config[j][2 * i] == '-')
				j++;

			if (j > 0)
				freePlaces++;
		}

		int isLeaf = validate(config, N, M);

		alloc_tree_node(&root, config, N, M, freePlaces, isLeaf);

		if (isLeaf == 0)
			createTree(root, NextPlayer);
	FILE *fisier = fopen(argv[3], "w");

		printTree(root, fisier, 0);
		fclose(fisier);

		free(line);

		for (int i = 0; i < N; ++i)
			free(config[i]);

		free(config);
		deallocTreeNode(root);
	} else if (strcmp(argv[1], "-c2") == 0) {
		FILE *fila = fopen(argv[2], "r");
		int H;
		char string[500000];
		char **config;

		fgets(string, 500000, fila);
		if (string[strlen(string) - 1] == '\n')
			string[strlen(string) - 1] = '\0';
		H = atoi(string);
		config = malloc(sizeof(char *) * H);

		for (int i = 0; i < H; ++i) {
			fgets(string, 500000, fila);
			if (string[strlen(string) - 1] == '\n')
				string[strlen(string) - 1] = '\0';
			config[i] = strdup(string);
		}

		fclose(fila);

		ListNode *head = NULL;

		buildTree(&head, H, config);
		Minimax *tree = head->nod;

		fila = fopen(argv[3], "w");
		printMinimaxTree(tree, fila, 0);
		fclose(fila);

		for (int i = 0; i < H; ++i)
			free(config[i]);

		free(config);
		deallocMinimaxTree(tree);
		free(head);
	} else if (strcmp(argv[1], "-c3") == 0) {
		FILE *fila = fopen(argv[2], "r");
		int H;
		char string[500000];
		char **config;

		fgets(string, 500000, fila);
		if (string[strlen(string) - 1] == '\n')
			string[strlen(string) - 1] = '\0';
		H = atoi(string);
		config = malloc(sizeof(char *) * H);

		for (int i = 0; i < H; ++i) {
			fgets(string, 500000, fila);
			if (string[strlen(string) - 1] == '\n')
				string[strlen(string) - 1] = '\0';
			config[i] = strdup(string);
		}

		fclose(fila);

		ListNode *head = NULL;

		buildTree(&head, H, config);
		Minimax *tree = head->nod;

		rebuildTree(tree, 1, -99999, 99999);

		fila = fopen(argv[3], "w");
		printMinimaxTree(tree, fila, 0);
		fclose(fila);

		for (int i = 0; i < H; ++i)
			free(config[i]);

		free(config);
		deallocMinimaxTree(tree);
		free(head);
	} else if (strcmp(argv[1], "-b") == 0) {
		FILE *fila = fopen(argv[2], "r");
		int H, Players;
		char string[300000];
		char **config;

		fgets(string, 300000, fila);
		if (string[strlen(string) - 1] == '\n')
			string[strlen(string) - 1] = '\0';
		char *point = strtok(string, " ");

		H = atoi(point);
		point = strtok(NULL, " ");
		Players = atoi(point);

		config = malloc(sizeof(char *) * H);

		for (int i = 0; i < H; ++i) {
			fgets(string, 300000, fila);
			if (string[strlen(string) - 1] == '\n')
				string[strlen(string) - 1] = '\0';
			config[i] = strdup(string);
		}

		fclose(fila);
		listParanoic *head = NULL;

		paranoicTree(&head, H, Players, config);
		Paranoic *root = head->node;

		fila = fopen(argv[3], "w");
		printParanoic(root, fila, 0);
		fclose(fila);

		for (int i = 0; i < H; ++i)
			free(config[i]);

		free(config);
		deallocParanoic(root);
		free(head);
	}

	return 0;
}
