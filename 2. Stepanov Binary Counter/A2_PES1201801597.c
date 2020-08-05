#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "assignment_2.h"

//3 assumptions as mentioned in the Assignment 2 Doubts Excel Sheet: numbers are all distinct, non-negative; n ≥ 2

//node structure
typedef struct node
{
	int num;
	struct node *next;
}node;

//list structure having header pointers to nodes
typedef struct list
{
	node *head;
}list_array;

static void init(list_array *list, int space)
{
	//making the headers point to NULL initially
	for(int i = 0; i < space; i++)
	{
		list[i].head = NULL;
	}
}

static node* create_node(int elem)
{
	//creates a node with elem as its stored value
	node *t = (node *)malloc(sizeof(node));
	t->num = elem;
	t->next = NULL;
	return t;
}

static void print_set(node *head)
{
	//prints LL starting at head
	while(head != NULL)
	{
		printf("%d ", head->num);
		head = head->next;
	}
}

void find_second_greatest(int *numbers, int length)
{
	int space = ceil(log2(length))+1; //space needed
	list_array list[space]; //an array of header pointers to nodes; column "i" will have the element(that has won "i" comparisions) and its losing elements
	init(list, space); //initialises the list array

	int last_pos; //holds the value of the column that might play a match with "t"; else its free to hold "t"; "t is defined in line 63"
	int elem;

	//go through the elements in the "numbers" array
	for(int i = 0; i < length; i++)
	{
		last_pos = 0;
		elem = numbers[i]; //reading element from the "numbers" array
		node *t = create_node(elem); //creates node with elem as data; subsequently it will become LL having the winning set after possibly playing matches with existing competing sets
		while(1)
		{
			//if column is free, place the elem there
			if(list[last_pos].head == NULL)
			{
				list[last_pos].head = t;
				break;
			}

			//there's a match to be played between "t" and "list[last_pos].head"
			else
			{
				//printing competing sets
				print_set(list[last_pos].head);
				printf("x ");
				print_set(t);
				printf("= ");

				//comparing to find winner between the two sets
				if(list[last_pos].head->num > t->num)
				{
					t->next = list[last_pos].head->next;
					list[last_pos].head->next = t;
					t = list[last_pos].head;
					list[last_pos].head = NULL;
					last_pos++;
				}
				else
				{
					list[last_pos].head->next = t->next;
					t->next = list[last_pos].head;
					list[last_pos].head = NULL;
					last_pos++;
				}

				//printing the winning set of the match played
				print_set(t);
				printf("\n");
			}
		}
	}
	
	node *t; //is a LL of final winning set when n is not a power of 2

	//if length is not a power of 2(the (space-1)st column will be free), columns have to be given a "bye" and played with subsequent columns if a possible match is possible
	//Note : There will be atleast 2 non free columns when n is not a power of 2
	if(list[space-1].head == NULL)
	{
		int i = 0; //column index for searching for non free columns
		node *pres; //non free column for playing match with "t"

		//finding first non free column
		while(list[i].head == NULL)
		{
			i++;
		}
		t = list[i].head;
		i++;

		while(1)
		{
			//finding the next non free column to play match with "t"
			while(list[i].head == NULL)
			{
				i++;
			}
			pres = list[i].head;

			//printing competing sets
			print_set(pres);
			printf("x ");
			print_set(t);
			printf("= ");

			//comparing to find winner between the two sets
			if(pres->num > t->num)
			{
				t->next = pres->next;
				pres->next = t;
				t = pres;
			}
			else
			{
				pres->next = t->next;
				t->next = pres;
			}

			//printing winning set
			print_set(t);
			printf("\n");
			
			//checking for end of list array
			i++;
			if(i == space-1)
			{
				break;
			}
		}
	}


	node *sg; //LL containing elements who lost to highest element; stands for second greatest

	//if length is non power of 2, then sg should be t->next
	//else it should be the last column's head pointer's next
	if(list[space-1].head == NULL)
	{
		sg = t->next;
	}
	else
	{
		sg = list[space-1].head->next;
	}

	//finding second highest element
	int second_greatest = sg->num;
	sg = sg->next;
	while(sg != NULL)
	{
		if(sg->num > second_greatest)
			second_greatest = sg->num;
		sg = sg->next;
	}
	printf("\n%d\n", second_greatest);
}
