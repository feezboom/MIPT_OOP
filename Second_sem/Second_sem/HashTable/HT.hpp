#define MAX_FILE_NAME 255
#pragma warning (disable:4996)
#define MAXCOUNT 1000
#define MaxStringSize 100

table* CreateTable(int tsize)
{
	if (tsize == 0) return NULL;
	int i;
	table* Table = (table*)malloc(sizeof(table));
	Table->Size = tsize;
	Table->NumberOfElements = 0;
	Table->mytable = (node**)malloc(tsize*sizeof(node*));
	for (i = 0; i < tsize; ++i) Table->mytable[i] = NULL;
	return Table;
}
int InsertInTheTable(table* Table, data* Data)
{
	if (!Table) return 1;
	int hash = HashU(Data->KEY, Table->Size);
	node* temp = Table->mytable[hash];
	node* p = (node*)malloc(sizeof(node));
	p->input = Data;
	p->next = temp;
	p->previous = NULL;
	if (temp != NULL) temp->previous = p;
	Table->mytable[hash] = p;
	Table->NumberOfElements++;
	return 0;
}
data* FindInTheTable(table* Table, key KEY)
{
	if (!Table) return NULL;
	int hash = HashU(KEY, Table->Size);
	node* p = Table->mytable[hash];
	while (p != NULL && strcmp(p->input->KEY, KEY))
		p = p->next;
	if (p != NULL) return p->input;
	else return NULL;
}
int DeleteFromTable(table* Table, key KEY, FILE *out)
{
	if (!Table) return 1;
	int hash = HashU(KEY, Table->Size);
	node* p = Table->mytable[hash];
	node* temp; char deleted = 0;
	while (p != NULL)
	{
		temp = p->next;
		if (!strcmp(p->input->KEY, KEY))
		{
			if (p->previous != NULL)
				p->previous->next = p->next;
			else
				Table->mytable[hash] = temp;

			if (p->next != NULL)
				p->next->previous = p->previous;

			free(p->input);
			free(p);
			Table->NumberOfElements--;

			fprintf(out, "\n%s was deleted.", KEY);
			deleted = 1;
		}
		p = temp;
	}
	if (!deleted) fprintf(out, "Nothing was deleted...");
	return 0;
}
void PrintTable(table* Table, FILE *out)
{
	if (!Table) return;
	int i, tsize = Table->Size;
	node* p;

	for (i = 0; i < tsize; ++i){
		p = Table->mytable[i];
		fprintf(out, "(Hash == %d) -->", i);
		while (p != NULL)
		{
			fprintf(out, "%s(%d)<-->", p->input->KEY, p->input->my_data);
			p = p->next;
		}
		fprintf(out, "NULL\n");
	}
}
void PrintList(node* list, FILE *out)
{
	if (!list) return;
	while (list != NULL)
	{
		fprintf(out, "%d\t", list->input->my_data);
		list = list->next;
	}
}
int HashU(char* s, int M)
{
	if (M <= 1) return 0;
	int a = 31415, b = 27183, h = 0;
	for (; (*s) != '\0'; (s++), a = a*b % (M - 1))
	{
		h = (a*h + *s) % M;
	}
	return h;
}
void ClearTable(table* Table)
{
	if (!Table) return;

	int i;
	node *p, *temp;
	for (i = 0; i < Table->Size; ++i){
		p = Table->mytable[i];
		while (p != NULL){
			temp = p->next;
			if (p->input != NULL)
				free(p->input);
			free(p);
			p = temp;
		}
		Table->mytable[i] = NULL;
	}
	Table->NumberOfElements = 0;
}
void DeleteTable(table* Table)
{
	if (!Table) return;
	ClearTable(Table);
	free(Table->mytable);
	free(Table);
	Table = NULL;
}


#include "hashtest.hpp"