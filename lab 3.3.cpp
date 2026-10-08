#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct snode {
	char          inf[256];
	struct snode *next;
};

struct snode *top = NULL;   /* вершина стека */

/* ---------- push: в голову ---------- */
void push(void)
{
	struct snode *p = NULL;
	char s[256];

	if ((p = (struct snode*)malloc(sizeof(struct snode))) == NULL) {
		printf("Ошибка при распределении памяти\n");
		exit(1);
	}

	printf("Введите название объекта: \n");
	scanf("%255s", s);
	if (*s == 0) {
		printf("Запись не была произведена\n");
		free(p);
		return;
	}
	strcpy(p->inf, s);

	p->next = top;
	top = p;
	printf("Элемент добавлен на вершину стека\n");
}

/* ---------- pop: из головы ---------- */
int pop(char *out)
{
	if (top == NULL) {
		printf("Стек пуст\n");
		return 0;
	}
	struct snode *tmp = top;
	strcpy(out, tmp->inf);
	top = tmp->next;
	free(tmp);
	return 1;
}

/* ---------- Просмотр ---------- */
void review(void)
{
	struct snode *struc = top;
	if (top == NULL) {
		printf("Стек пуст\n");
		return;
	}
	printf("Содержимое стека (вершина сверху):\n");
	while (struc) {
		printf("Имя - %s\n", struc->inf);
		struc = struc->next;
	}
}

/* ---------- Поиск по имени ---------- */
struct snode *find(char *name)
{
	struct snode *struc = top;
	if (top == NULL) {
		printf("Стек пуст\n");
		return NULL;
	}
	while (struc) {
		if (strcmp(name, struc->inf) == 0) return struc;
		struc = struc->next;
	}
	printf("Элемент не найден\n");
	return NULL;
}

/* ---------- Удаление по имени ---------- */
void del(char *name)
{
	struct snode *struc = top;
	struct snode *prev = NULL;
	int flag = 0;

	if (top == NULL) {
		printf("Стек пуст\n");
		return;
	}

	while (struc) {
		if (strcmp(name, struc->inf) == 0) {
			flag = 1;
			if (prev == NULL) top = struc->next;
			else              prev->next = struc->next;

			free(struc);
			struc = (prev == NULL) ? top : prev->next;
			continue;
		}
		prev = struc;
		struc = struc->next;
	}

	if (flag == 0) printf("Элемент не найден\n");
}

/* ---------- Освобождение ---------- */
void free_all(void)
{
	struct snode *n = top;
	while (n) {
		struct snode *nx = n->next;
		free(n);
		n = nx;
	}
	top = NULL;
}

/* ---------- main ---------- */
int main(void)
{
	setlocale(LC_ALL,"rus");
	int  choice;
	char name[256];

	for (;;) {
		printf("\n=== СТЕК (LIFO) ===\n"
			"1 - на вершину\n"
			"2 - с вершины\n"
			"3 - просмотр\n"
			"4 - найти\n"
			"5 - удалить по имени\n"
			"0 - выход\n> ");
		if (scanf("%d", &choice) != 1) break;

		switch (choice) {
		case 1: push(); break;
		case 2:
			if (pop(name))
				printf("Извлечён: %s\n", name);
			break;
		case 3: review(); break;
		case 4: {
					printf("Имя для поиска: ");
					scanf("%255s", name);
					struct snode *f = find(name);
					if (f) printf("Найден: %s\n", f->inf);
					break;
		}
		case 5:
			printf("Имя для удаления: ");
			scanf("%255s", name);
			del(name);
			break;
		case 0:
			free_all();
			return 0;
		default: printf("Нет такого пункта\n");
		}
	}
	free_all();
	return 0;
}