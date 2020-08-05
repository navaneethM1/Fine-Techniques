#include <stdio.h>
#include <stdlib.h>
#include "A3_header_PES1201801597.h"




//Convention used : Captital letters - 'A', 'B', 'C' are the capacities of the jugs
//Convention used : Small letters - 'a', 'b', 'c' are the water content of the jugs in a particular state
//'A', 'B', 'C' are used only for the capacity structure and this info is gotten from the input




//helper min function
int min(int x, int y)
{
	return (x < y) ? x : y;
}




//function for capacity(created using input given)
void init_capacity(capacity *s, int A, int B, int C)
{
	s->A = A;
	s->B = B;
	s->C = C;
}




//functions for state
void init_state(state *s, int a, int b, int c)
{
	s->a = a;
	s->b = b;
	s->c = c;
	s->fn_index = 0;
}

void disp_state(const state* s)
{
	printf("%d %d %d\n", s->a, s->b, s->c);
}

//Each of these functions represent the moves(ie transfer of water from one jug to the other)
//The tranfer quantity is the min(what src can give, dst can take)
//This logic ensures that water is transferred until src becomes empty or dst becomes full
//One of the jugs not involved in the transfer must be assigned the same value as that of src
void moveAB(const state *src, state *dst, const capacity *cap)
{
	int jugA_can_give = src->a;
	int jugB_can_take = cap->B - src->b;
	int transfer = min(jugA_can_give, jugB_can_take);
	dst->a = src->a - transfer;
	dst->b = src->b + transfer;
	dst->c = src->c;
	dst->fn_index = 0;
}

void moveAC(const state *src, state *dst, const capacity *cap)
{
	int jugA_can_give = src->a;
	int jugC_can_take = cap->C - src->c;
	int transfer = min(jugA_can_give, jugC_can_take);
	dst->a = src->a - transfer;
	dst->c = src->c + transfer;
	dst->b = src->b;
	dst->fn_index = 0;
}

void moveBA(const state *src, state *dst, const capacity *cap)
{
	int jugB_can_give = src->b;
	int jugA_can_take = cap->A - src->a;
	int transfer = min(jugB_can_give, jugA_can_take);
	dst->b = src->b - transfer;
	dst->a = src->a + transfer;
	dst->c = src->c;
	dst->fn_index = 0;
}

void moveBC(const state *src, state *dst, const capacity *cap)
{
	int jugB_can_give = src->b;
	int jugC_can_take = cap->C - src->c;
	int transfer = min(jugB_can_give, jugC_can_take);
	dst->b = src->b - transfer;
	dst->c = src->c + transfer;
	dst->a = src->a;
	dst->fn_index = 0;
}

void moveCA(const state *src, state *dst, const capacity *cap)
{
	int jugC_can_give = src->c;
	int jugA_can_take = cap->A - src->a;
	int transfer = min(jugC_can_give, jugA_can_take);
	dst->c = src->c - transfer;
	dst->a = src->a + transfer;
	dst->b = src->b;
	dst->fn_index = 0;
}

void moveCB(const state *src, state *dst, const capacity *cap)
{
	int jugC_can_give = src->c;
	int jugB_can_take = cap->B - src->b;
	int transfer = min(jugC_can_give, jugB_can_take);
	dst->c = src->c - transfer;
	dst->b = src->b + transfer;
	dst->a = src->a;
	dst->fn_index = 0;
}

//checks if both the states are same
int are_same(const state *s, const state *t)
{
	return (s->a == t->a) && (s->b == t->b) && (s->c == t->c);
}

//is_valid is not required because of the way the move functions have been written



//functions for doubly linked list(containing nodes that have states)
void init_list(list *ptr_list)
{
	ptr_list->head = NULL;
	ptr_list->tail = NULL;
}

//adds at the end
void push(const state *ptr_state, list *ptr_list)
{
	node *temp = (node *)malloc(sizeof(node));
	temp->st = *ptr_state;
	if(ptr_list->head == NULL)
	{
		temp->prev = temp->next = NULL;
		ptr_list->head = ptr_list->tail = temp;
	}
	else
	{
		temp->next = NULL;
		temp->prev = ptr_list->tail;
		ptr_list->tail->next = temp;
		ptr_list->tail = temp;
	}
}

//removes from the end
void pop(list *ptr_list)
{
	if(ptr_list->head == NULL)
	{
		return;
	}
	node *temp = ptr_list->tail;
	if(ptr_list->head == ptr_list->tail)
	{
		ptr_list->head = ptr_list->tail = NULL;
	}
	else
	{
		ptr_list->tail = temp->prev;
		ptr_list->tail->next = NULL;
	}
	free(temp);
}

//checks if the list is empty
int is_empty(list *ptr_list)
{
	if(ptr_list->head == NULL)
		return 1;
	return 0;
}

int is_repeated(const state *s, list *ptr_list)
{
	node *temp = ptr_list->head;
	while(temp != NULL)
	{
		if(are_same(s, &temp->st))
			return 1;
		temp = temp->next;
	}
	return 0;
}

void disp_list(list *ptr_list)
{
	node *temp = ptr_list->head;
	while(temp != NULL)
	{
		disp_state(&temp->st);
		temp = temp->next;
	}
}
