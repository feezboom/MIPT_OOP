#ifndef _HT_H
#define _HT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* key;

typedef struct data
{
	key KEY;
	int my_data;
} data;
typedef struct node
{
	struct node* next;
	struct node* previous;
	struct data* input;
} node;
typedef struct table
{
	int Size;
	int NumberOfElements;
	struct node** mytable;
} table;

int HashU(char* s, int M);
table* CreateTable(int tsize);
int InsertInTheTable(table* Table, data* Data);
data* FindInTheTable(table* Table, key KEY);
int DeleteFromTable(table* Table, key KEY, FILE *out);
void PrintTable(table* Table, FILE* out);
void PrintList(struct node* list, FILE *out);
int HashU(char* s, int M);
void ClearTable(table* Table);
void DeleteTable(table* Table);
int hashtest();

#include "HT.hpp"

#endif