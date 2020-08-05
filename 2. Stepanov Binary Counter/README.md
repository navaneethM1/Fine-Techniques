# Data Structures used:
The data structure used is an array of head pointers. Each of these point to the beginning of 
a linked list. Column "i" in the array will be the linked list that has its first element 
as the winner of "i" comparisions so far. The rest of the elements will be the elements that lost to the 
first element. Also, the columns are binary, meaning they are either filled or empty. I chose the array of linked lists implementation because it is easy to insert the losing element in the second position in the winning set(as per convention mentioned) when compared to 2D array implementation.

# How to compile (I have used math.h):
gcc A2_PES1201801597.c sanity_client.c -lm
where "sanity_client.c" is the file having the test case/main program

# Key takeaway from this assignment:
Using normal method of keeping track of highest and second highest found so far using two 
variables is not so efficient. It takes 2*(n-1) comparisions. On the other hand, 
the efficiency of the algorithm implemented in this assignment is due to the fact that we are transforming 
the problem into a tournament based tree style, wherein the second highest element must compulsorily be one of the elements that has lost to the highest element(at some point of time) during the course of the tournament. The number of elements that lose to the highest element is the height of the tournament tree(logn). Thus, number of comparisions is (n-1) + (logn - 1), for finding highest and second highest element respectively.
