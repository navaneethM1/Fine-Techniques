#include <stdio.h>
#include <stdlib.h>
#include "A3_header_PES1201801597.h"

int main()
{
	int p, q, r;

	//takes the input from the stdin

	//capacities' input
	scanf("%d %d %d", &p, &q, &r);
	capacity cap;
	init_capacity(&cap, p, q, r);

	//initial state
	scanf("%d %d %d", &p, &q, &r);
	state start;
	init_state(&start, p, q, r);

	//final state
	scanf("%d %d %d", &p, &q, &r);
	state goal;
	init_state(&goal, p, q, r);

	//array of callback functions(follows the order mentioned in the doc file)
	void (*move[])(const state *, state *, const capacity *) = {moveAB, moveAC, moveBA, moveBC, moveCA, moveCB};

	int is_soln; //to check if a soln is found
	int c = 0; //counts the number of solns
	state new; //new state which maybe be pushed

	//doubly linked list(shows stack behaviour) initialisation
	//has a sequence of nodes that are part of a 'possible' soln
	//strategy builds the soln(or steps towards the soln) 1 step at a time
	//so contains partial soln to the problem
	//initially start state is pushed
	list mylist;
	init_list(&mylist);
	push(&start, &mylist);

	while(!is_empty(&mylist)) //as long as list is not empty
	{
		move[mylist.tail->st.fn_index](&mylist.tail->st, &new, &cap); //make a water transfer

		//if the new state is valid(not checked here as it is automatically taken care because of the way the move functions are written) and not repeated in the list
		if(!is_repeated(&new, &mylist))
		{
			push(&new, &mylist); //push to list as it maybe part of a soln
			is_soln = are_same(&new, &goal);
			
			//if a soln is found, it must be printed
			//then the last node pushed(ie the final state) must be popped
			//then backtracking should happen to look for more solns
			if(is_soln)
			{
				c++;
				disp_list(&mylist);
				printf("\n");
				pop(&mylist);
				while(!is_empty(&mylist) && ++mylist.tail->st.fn_index == 6)
				{
					pop(&mylist);
				}
			}
		}
		
		//else backtracking should happen to look for valid states
		else
		{
			while(!is_empty(&mylist) && ++mylist.tail->st.fn_index == 6)
			{
				pop(&mylist);
			}
		}
	}

	printf("%d\n", c); //prints the number of solns found
	return 0;
}
