#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


struct node {
	char         inf[256];   
	int          priority;   
	struct node* next;
};

struct node* head = NULL;    /* голова = максимальный приоритет */
struct node* last = NULL;    /* хвост (для обновления при вставке в конец) */

/* ---------- Создание элемента ---------- */
struct node* get_struct(void)
{
	struct node* p = NULL;
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
	struct node* p = get_struct();
	if (p == NULL) return;

	/* Пустой список или новый приоритет выше головы */
	if (head == NULL || p->priority > head->priority) {
		p->next = head;
		head = p;
		if (last == NULL) last = p;
		return;
	}

	/* Ищем первый узел с приоритетом < нового */
	struct node* cur = head;
	while (cur->next != NULL && cur->next->priority >= p->priority)
		cur = cur->next;

	p->next = cur->next;
	cur->next = p;

	if (p->next == NULL) last = p;
}

/* ---------- Извлечение максимума ---------- */
int extract(char* out_name, int* out_prio)
{
	if (head == NULL) {
		printf("Очередь пуста\n");
		return 0;
	}
	struct node* tmp = head;
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
	struct node* struc = head;
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
struct node* find(char* name)
{
	struct node* struc = head;
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
void del(char* name)
{
	struct node* struc = head;
	struct node* prev = NULL;
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
/*----------- Изменение приоритета по имени -----------*/

int change_priority(char* name, int new_prio)
{
	
	struct node* matched = NULL;  
	struct node* matched_tail = NULL;
	struct node* prev = NULL;
	struct node* cur = head;

	while (cur != NULL) {
		if (strcmp(name, cur->inf) == 0) {
			struct node* next = cur->next;
			if (prev == NULL)
				head = next;            
			else
				prev->next = next;      

			if (cur == last)
				last = prev;

			cur->next = NULL;
			if (matched == NULL)
				matched = matched_tail = cur;
			else {
				matched_tail->next = cur;
				matched_tail = cur;
			}

			cur = next;
		}
		else {
			
			prev = cur;
			cur = cur->next;
		}
	}

	if (matched == NULL) {
		printf("Элемент не найден\n");
		return 0;
	}

	int count = 0;
	cur = matched;
	while (cur != NULL) {
		struct node* next = cur->next;   

		cur->priority = new_prio;
		cur->next = NULL;

		if (head == NULL || cur->priority > head->priority) {
			cur->next = head;
			head = cur;
			if (last == NULL) last = cur;
		}
		else {
			struct node* p = head;
			while (p->next != NULL && p->next->priority >= cur->priority)
				p = p->next;

			cur->next = p->next;
			p->next = cur;

			if (cur->next == NULL) last = cur;
		}

		count++;
		cur = next;
	}

	return count;
}
/* ---------- Освобождение ---------- */
void free_all(void)
{
	struct node* n = head;
	while (n) {
		struct node* nx = n->next;
		free(n);
		n = nx;
	}
	head = last = NULL;
}

/* ---------- main ---------- */
int main(void)
{
	SetConsoleOutputCP(65001); 
	SetConsoleCP(65001);       
	setlocale(LC_ALL, "ru-RU.UTF-8"); 
	
	int  choice;
	char name[256];

	for (;;) {
		printf("\n=== ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ===\n"
			"1 - добавить\n"
			"2 - извлечь максимум\n"
			"3 - просмотр\n"
			"4 - найти\n"
			"5 - удалить по имени\n"
			"6 - изменение приоритета по имени\n"
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
			struct node* f = find(name);
			if (f) printf("Найден: %s, приоритет %d\n", f->inf, f->priority);
			break;
		}
		case 5:
			printf("Имя для удаления: ");
			scanf("%255s", name);
			del(name);
			break;
		case 6:
			printf("Имя  элемента:");
			scanf("%255s", name);
			int new_prio;
			printf("Новый приоритет:");
			scanf("%d",&new_prio);
			if (change_priority(name, new_prio));
			printf("приоритет изменён\n");
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
