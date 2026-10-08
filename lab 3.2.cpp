#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct qnode {
	char          inf[256];
	struct qnode *next;
};

struct qnode *q_head = NULL;   /* откуда берём (front) */
struct qnode *q_tail = NULL;   /* куда кладём (back)  */

/* ---------- enqueue: в конец ---------- */
void enqueue(void)
{
	struct qnode *p = NULL;
	char s[256];

	if ((p = (struct qnode*)malloc(sizeof(struct qnode))) == NULL) {
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
	p->next = NULL;

	if (q_tail == NULL) {          /* очередь пуста */
		q_head = q_tail = p;
	}
	else {
		q_tail->next = p;
		q_tail = p;
	}
	printf("Элемент добавлен в конец очереди\n");
}

/* ---------- dequeue: из головы ---------- */
int dequeue(char *out)
{
	if (q_head == NULL) {
		printf("Очередь пуста\n");
		return 0;
	}
	struct qnode *tmp = q_head;
	strcpy(out, tmp->inf);
	q_head = tmp->next;
	if (q_head == NULL) q_tail = NULL;
	free(tmp);
	return 1;
}

/* ---------- Просмотр ---------- */
void review(void)
{
	struct qnode *struc = q_head;
	if (q_head == NULL) {
		printf("Очередь пуста\n");
		return;
	}
	printf("Содержимое очереди (от головы к хвосту):\n");
	while (struc) {
		printf("Имя - %s\n", struc->inf);
		struc = struc->next;
	}
}

/* ---------- Поиск по имени ---------- */
struct qnode *find(char *name)
{
	struct qnode *struc = q_head;
	if (q_head == NULL) {
		printf("Очередь пуста\n");
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
	struct qnode *struc = q_head;
	struct qnode *prev = NULL;
	int flag = 0;

	if (q_head == NULL) {
		printf("Очередь пуста\n");
		return;
	}

	while (struc) {
		if (strcmp(name, struc->inf) == 0) {
			flag = 1;
			if (prev == NULL) q_head = struc->next;
			else              prev->next = struc->next;

			if (struc == q_tail) q_tail = prev;
			free(struc);

			struc = (prev == NULL) ? q_head : prev->next;
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
	struct qnode *n = q_head;
	while (n) {
		struct qnode *nx = n->next;
		free(n);
		n = nx;
	}
	q_head = q_tail = NULL;
}

/* ---------- main ---------- */
int main(void)
{
	setlocale(LC_ALL,"rus");
	int  choice;
	char name[256];

	for (;;) {
		printf("\n=== ОЧЕРЕДЬ (FIFO) ===\n"
			"1 - в конец\n"
			"2 - из головы\n"
			"3 - просмотр\n"
			"4 - найти\n"
			"5 - удалить по имени\n"
			"0 - выход\n> ");
		if (scanf("%d", &choice) != 1) break;

		switch (choice) {
		case 1: enqueue(); break;
		case 2:
			if (dequeue(name))
				printf("Извлечён: %s\n", name);
			break;
		case 3: review(); break;
		case 4: {
					printf("Имя для поиска: ");
					scanf("%255s", name);
					struct qnode *f = find(name);
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