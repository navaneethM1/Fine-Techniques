#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "assignment_1.h"

char *p = NULL;
int size;

struct book
{
	int mark; //mark = 1 if free, mark = 0 if allocated
	int size;
	char *memory;
	struct book *next;
};
typedef struct book book;

static book *head = NULL; //head pointer to list of books
static int book_size = sizeof(book);

void allocate(int n)
{
	//mallocing n bytes
	p = (char *)malloc(n*sizeof(char));
	size = n;

	head = (book *)p;

	//initialising head pointer
	head->mark = 1;
	head->size = n - book_size;
	head->memory = (char *)(p + book_size);
	head->next = NULL;
}

void* mymalloc(int size)
{
	int tot_size = size + book_size;

	book *temp = head;
	
	//will point to free book according to best fit policy
	book *best_book = NULL;

	//difference between temp->size and size which is minimum so far
	int best = INT_MAX;

	while(temp != NULL)
	{
		//best fit policy where min(temp->size - size which is >= 0) is given
		//this will result in best fit by definition
		if((temp->mark == 1) && (temp->size >= size) && (temp->size - size < best))
		{
			best = temp->size - size;
			best_book = temp;
		}
		temp = temp->next;
	}

	//if best fit book not found
	if(best_book == NULL)
		return NULL;

	//if memory is sufficient for both data asked and to create another book keeping info
	if(tot_size < best_book->size)
	{
		//steps for allocating:
		book *x = (book *)(best_book->memory + size); //new book keeping info of free space after allocation
		best_book->mark = 0; //since it is now allocated
		x->mark = 1; //since it is the new reduced free block
		x->size = best_book->size - (tot_size); //amount of free space after allocation
		best_book->size = size; //amount of allocated memory
		x->memory = best_book->memory + tot_size; //pointer to the reduced free block

		//inserting book x after best_book in the list of books
		x->next = best_book->next;
		best_book->next = x;
	}
	//memory sufficient for data asked alone
	else
	{
		//steps for allocating:
		//just giving the entire block (along with extra memory)
		best_book->mark = 0;
	}

	//returning pointer to allocated block of memory of size "size" or more(in case memory is sufficient for data asked alone)  bytes
	return best_book->memory;
}

void myfree(void *b)
{
	//if b is NULL
	if(b == NULL)
		return;

	book *pres = head;
	book *prev = NULL;

	//searching for book which stores info about b
	while(pres != NULL)
	{		
		if((pres->mark == 0) && (pres->memory == b))
		{
			break;
		}
		prev = pres;
		pres = pres->next;
	}

	//b was never allocated or it was invalid
	if(pres == NULL)
		return;

	book *pres_next = pres->next;
	
	//checking if a merge is possible b/w prev and pres
	if((prev != NULL) && (prev->mark == 1))
	{
		prev->size += (pres->size + book_size); //increasing prev's size
		prev->next = pres->next; //deleting pres from list of books
		pres = prev; //having pres point to merged ie prev
	}
	else
	{
		pres->mark = 1; //pres is freed
	}

	//if pres_next is NULL, then there is no other work to be done
	if(pres_next == NULL)
		return;

	//but if pres_next is present, then it should be checked for merging with pres
	if(pres_next->mark == 1)
	{
		pres->size += (pres_next->size + book_size); //increasing pres's size
		pres->next = pres_next->next; //deleting pres_next from list of books
	}
	return;
}

void print_book()
{
	printf("%d\n", book_size); //prints the size of book keeping structure
}

void display_mem_map()
{
	book *temp = head;
	
	while(temp != NULL)
	{
		printf("%ld\t%d\tbook\n", (char *)temp-p, book_size); //info about book
		if(temp->mark == 0)
		{
			printf("%ld\t%d\tallocated\n", temp->memory-p, temp->size); //if allocated
		}
		else
		{
			printf("%ld\t%d\tfree\n", temp->memory-p, temp->size); //if free
		}
		temp = temp->next;
	}
}
