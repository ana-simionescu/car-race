// Copyright 2019 Simionescu Ana-Maria
#include <iostream>
#include <algorithm>
#include <cstring>
#include <tuple>
#include "SkipList.h"
#define N 1001

// Functie pentru ordonarea crescatoare a timpilor unei curse si
// pentru departajarea in functie de ultimul clasament
bool cresc(std::tuple<int, int, int> A, std::tuple<int, int, int> B) {
	if(std::get<1>(A) == 0) {
		return 0;
	}
	if(std::get<1>(B) == 0) {
		return 1;
	}
	if(std::get<1>(A) == std::get<1>(B)) {
		if(std::get<2>(A) == std::get<2>(B)) {
			return std::get<0>(A) < std::get<0>(B);
		} else {
			return std::get<2>(A) < std::get<2>(B);
		}
	} else {
		return std::get<1>(A) < std::get<1>(B);
	}
}


// Functie pentru ordonarea descrescatoare  a punctajelor
// si obtinerea clasamentului
bool descresc(std::tuple<int, int, int> A, std::tuple<int, int, int> B) {
	if(std::get<1>(A) == std::get<1>(B)) {
			return std::get<0>(A) < std::get<0>(B);
		} else {
			return std::get<1>(A) > std::get<1>(B);
		}
}

int main() {
	int n, m, k, i, j, l, p, oldrank[N], last_print_rank[N];
	std::tuple<int, int, int> rank[N], race[N];
	Node<int> *aux;
	Node<int> *current;
	char s[10];

	freopen("races.in", "r", stdin);
	freopen("races.out", "w", stdout);
	scanf("%d%d%d", &n, &m, &k);

	// Initializez clasamentul pe cursa si clasamentul general

	for (j = 1; j <= n; j++) {
		std::get<0>(race[j]) = j;
		std::get<1>(race[j]) = 0;
		std::get<2>(race[j]) = j;
	}
	for (j = 1; j <= n; j++) {
		std::get<0>(rank[j]) = j;
		std::get<1>(rank[j]) = 0;
		std::get<2>(rank[j]) = j;
	}
	// Inserez in SkipList id-urile jucatorilor cu punctajul initial 0
	SkipList<int> *ids = new SkipList<int>;
	for (i = 1; i <= n; i++) {
		ids->insert(i, 0);
	}
	int ok = 0;
	for (i = 1; i <= m+k; i++) {
		scanf("%s", s);
		if (strcmp(s, "print") == 0) {
			if (ok == 1) {
				for (j = 1; j <= n; j++) {
					printf("%d %d %d\n", std::get<0>(rank[j]),
						std::get<1>(rank[j]),
						last_print_rank[std::get<0>(rank[j])]-j);
				}
			} else {
				for (j = 1; j <= n; j++) {
					printf("%d %d %d\n", std::get<0>(rank[j]),
 		 				std::get<1>(rank[j]), 0);
					ok = 1;
				}
			}
			// Actualizez ultimul clasament printat;
			for (j = 1; j <= n; j++) {
				for (l = 1; l <= n; l++) {
					if (std::get<0>(rank[l]) == j) {
						last_print_rank[j] = std::get<2>(rank[l]);
					}
				}
			}
			printf("\n");
		} else {
			p = 0;
			// Salvez ultimul clasament
			for (j = 1; j <= n; j++) {
				for (l = 1; l <=n; l++) {
					if(std::get<0>(rank[l]) == j) {
						oldrank[j] = std::get<2>(rank[l]);
					}
				}
			}
			// Actualizez timpii pentru noua cursa
			int t = atoi(s);
			std::get<0>(race[1]) = 1;
			std::get<1>(race[1]) = t;
			std::get<2>(race[1]) = oldrank[1];
			if(t != 0) {
				p++;
			}
			for (j = 2; j <= n; j++) {
				scanf("%d", &t);
				if(t != 0) {
					p++;
				}
				std::get<0>(race[j]) = j;
				std::get<1>(race[j]) = t;
				std::get<2>(race[j]) = oldrank[j];
			}
			int cont = 1;
			sort(race+1, race+n+1, cresc);
			// Acord punctajele corespunzatoare, actualizand valoarea
			// din SkipList
			for (j = 1; j <= n; j++) {
				aux = ids->search(std::get<0>(race[j]));
				aux = aux->next;
				if (std::get<1>(race[j]) != 0) {
					if(p/2-cont+1 == 0 && p%2 == 0) {
						cont++;
					}
					aux->value += p/2 - cont + 1;
					cont++;
				}
			}
			// Salvez ultimul clasament
			current = ids->getHead();
			while (current->below) {
				current = current->below;
			}
			current = current->next;
			for (j = 1; j <= n; j++) {
				for (l = 1; l <= n; l++) {
					if(std::get<0>(rank[l]) == j) {
						oldrank[j] = std::get<2>(rank[l]);
					}
				}
			}
			// Actualizez datele pentru noul clasament
			while(current->next) {
				std::get<0>(rank[current->key]) = current->key;
				std::get<1>(rank[current->key]) = current->value;
				std::get<2>(rank[current->key]) = oldrank[current->key];
				current = current->next;
			}
			// Determin clasametul
			sort(rank + 1, rank + n + 1, descresc);
			// Actualizez pozitiile in noul clasament
	 		for (j = 1; j <= n; j++) {
			 	std::get<2>(rank[j]) = j;
			}
		}
	}
	delete ids;

	return 0;
}
