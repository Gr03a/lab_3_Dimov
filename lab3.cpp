#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


struct node {
	char         inf[256];   /* полезная информация */
	int          priority;   /* чем больше — тем важнее */
	struct node *next;
};

struct node *head = NULL;    /* голова = максимальный приоритет */
struct node *last = NULL;    /* хвост (для обновления при вставке в конец) */

/* ---------- Создание элемента ---------- */
struct node *get_struct(void)
{
	struct node *p = NULL;
	char s[256];
	int  pr;

	if ((p = (struct node*)malloc(sizeof(struct node))) == NULL) {
		printf("Ошибка при распределении памяти\n");
		exit(1);
	}

	printf("Введите название объекта: \n");
	scanf("%255s", s);
	if (*s == 0) {
		printf("Запись не была произведена\n");
		free(p);
		return NULL;
	}
	strcpy(p->inf, s);

	printf("Введите приоритет (целое, больше = важнее): \n");
	scanf("%d", &pr);
	p->priority = pr;

	p->next = NULL;
	return p;
}

/* ---------- Вставка с учётом приоритета ---------- */
void spstore(void)
{
	struct node *p = get_struct();
	if (p == NULL) return;

	/* Пустой список или новый приоритет выше головы */
	if (head == NULL || p->priority > head->priority) {
		p->next = head;
		head = p;
		if (last == NULL) last = p;
		return;
	}

	/* Ищем первый узел с приоритетом < нового */
	struct node *cur = head;
	while (cur->next != NULL && cur->next->priority >= p->priority)
		cur = cur->next;

	p->next = cur->next;
	cur->next = p;

	if (p->next == NULL) last = p;
}

/* ---------- Извлечение максимума ---------- */
int extract(char *out_name, int *out_prio)
{
	if (head == NULL) {
		printf("Очередь пуста\n");
		return 0;
	}
	struct node *tmp = head;
	if (out_name) strcpy(out_name, tmp->inf);
	if (out_prio) *out_prio = tmp->priority;

	head = tmp->next;
	if (head == NULL) last = NULL;
	free(tmp);
	return 1;
}

/* ---------- Просмотр ---------- */
void review(void)
{
	struct node *struc = head;
	if (head == NULL) {
		printf("Очередь пуста\n");
		return;
	}
	printf("Содержимое очереди (голова = максимальный приоритет):\n");
	while (struc) {
		printf("Имя - %s, приоритет - %d\n", struc->inf, struc->priority);
		struc = struc->next;
	}
}

/* ---------- Поиск по имени ---------- */
struct node *find(char *name)
{
	struct node *struc = head;
	if (head == NULL) {
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
	struct node *struc = head;
	struct node *prev = NULL;
	int flag = 0;

	if (head == NULL) {
		printf("Очередь пуста\n");
		return;
	}

	while (struc) {
		if (strcmp(name, struc->inf) == 0) {
			flag = 1;
			if (prev == NULL) head = struc->next;
			else              prev->next = struc->next;

			if (struc == last) last = prev;
			free(struc);

			struc = (prev == NULL) ? head : prev->next;
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
	struct node *n = head;
	while (n) {
		struct node *nx = n->next;
		free(n);
		n = nx;
	}
	head = last = NULL;
}

/* ---------- main ---------- */
int main(void)
{
	setlocale(LC_ALL,"rus");
	int  choice;
	char name[256];

	for (;;) {
		printf("\n=== ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ===\n"
			"1 - добавить\n"
			"2 - извлечь максимум\n"
			"3 - просмотр\n"
			"4 - найти\n"
			"5 - удалить по имени\n"
			"0 - выход\n> ");
		if (scanf("%d", &choice) != 1) break;

		switch (choice) {
		case 1: spstore(); break;
		case 2: {
					int pr;
					if (extract(name, &pr))
						printf("Извлечён: %s (приоритет %d)\n", name, pr);
					break;
		}
		case 3: review(); break;
		case 4: {
					printf("Имя для поиска: ");
					scanf("%255s", name);
					struct node *f = find(name);
					if (f) printf("Найден: %s, приоритет %d\n", f->inf, f->priority);
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