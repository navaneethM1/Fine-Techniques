# Allocation policy:
Best fit allocation policy gives the user that part of free memory which is closest in terms of what the
user asked.

# Basic logic while implementing Best fit policy:
Following the above definition, I found the minimum of difference of free memory pointed to by each book and size asked for. I returned NULL if size asked exceeded any of the available free blocks of memory.

# Explanation of Book keeping structure:
I have used a single linked list of books to solve the assignment for which
My book keeping structure has 4 fields.
1. mark says whether the memory pointed to by the book's memory field is free or allocated.
2. size field which indicates the size of memory block pointed to by the book's memory field.
3. memory which points to the starting address of memory which the book keeps track of.
4. next for the purposes of maintaining singly linked list of books.

# Key takeaways from this assignment:
I got a complete understanding of how malloc and free work, and how dynamic memory allocation and management work. I also realised the importance of book keeping information that needs to be associated/attached with each block of memory be it free or allocated(though not needed for allocated). Finally, I improved my C skills by working with pointers and 
memory.

# Any additional notes:
While understanding about best fit policy online, I realised that best fit will usually end up creating small pieces of free memory which could remain unoccupied. Even though it gives the closest sized memory, the above 
drawback remains.
