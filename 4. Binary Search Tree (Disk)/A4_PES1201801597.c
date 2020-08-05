#include "assignment_4.h"
#include <stdio.h>
#include <stdlib.h>

//convention used for free list - right_offset of deleted node is used to link nodes for free list
//all indices are zero-based
//the offsets are like : 0 for the very first node(below tree header), 1 for the node after the first and so on

//reads header
static void read_header(tree_t *t, FILE *fp)
{
	fseek(fp, 0, SEEK_SET);
	fread(t, sizeof(tree_t), 1, fp);
}

//writes or overwrites header
static void write_header(const tree_t *t, FILE *fp)
{
	fseek(fp, 0, SEEK_SET);
	fwrite(t, sizeof(tree_t), 1, fp);
}

//reads node at offset
static void read_node(node_t *n, int offset, FILE *fp)
{
	fseek(fp, sizeof(tree_t) + offset*sizeof(node_t), SEEK_SET);
	fread(n, sizeof(node_t), 1, fp);
}

//writes or overwrites node at offset
static void write_node(const node_t *n, int offset, FILE *fp)
{
	fseek(fp, sizeof(tree_t) + offset*sizeof(node_t), SEEK_SET);
	fwrite(n, sizeof(node_t), 1, fp);
}

//initializes tree header
static void init_tree_header(FILE *fp)
{
	tree_t t;
	t.free_head = -1;
	t.root = -1;
	write_header(&t, fp);
}

//returns the offset to the node after the last node
static int last_pos(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	int last = (ftell(fp) - sizeof(tree_t))/sizeof(node_t);
	return last;
}

//initializes the nodes key, left and right offsets
static void init_node(node_t *n, int key)
{
	n->key = key;
	n->left_offset = n->right_offset = -1;
}

//returns the position of parent to which the new node needs to be attached
//-1 is returned if the key already exists in the bst built so far
static int search(int key, int pres, FILE *fp)
{
	int prev;
	node_t curr;
	while(pres != -1)
	{
		prev = pres;
		read_node(&curr, pres, fp);
		if(key < curr.key)
			pres = curr.left_offset;
		else if(key > curr.key)
			pres = curr.right_offset;
		else
			return -1; //duplicate key
	}
	return prev;
}

//updates the parent's offset to make it point to "pos" which is the offset of the child inserted
static void set_parent_offset(int prev, int pos, int key, FILE *fp)
{
	node_t parent;
	read_node(&parent, prev, fp);
	if(key < parent.key)
		parent.left_offset = pos;
	else
		parent.right_offset = pos;
	write_node(&parent, prev, fp);
}

//recursive implementation of the inorder traversal
static void inorder(const node_t *root, FILE *fp)
{
	if(root->left_offset != -1)
	{
		node_t temp;
		read_node(&temp, root->left_offset, fp);
		inorder(&temp, fp);
	}
	printf("%d ", root->key);
	if(root->right_offset != -1)
	{
		node_t temp;
		read_node(&temp, root->right_offset, fp);
		inorder(&temp, fp);
	}
}

//recursive implementation of the preorder traversal
static void preorder(const node_t *root, FILE *fp)
{
	printf("%d ", root->key);
	if(root->left_offset != -1)
	{
		node_t temp;
		read_node(&temp, root->left_offset, fp);
		preorder(&temp, fp);
	}
	if(root->right_offset != -1)
	{
		node_t temp;
		read_node(&temp, root->right_offset, fp);
		preorder(&temp, fp);
	}
}

//opens the file in "r+" mode, but initially if the file isnt present, creates it and opens in "w+" mode
//returns the file pointer to the the file opened
FILE* init_tree(const char* filename)
{
	FILE* fp;
	fp = fopen(filename, "r+");
	if(fp == NULL)
	{
		fp = fopen(filename, "w+");
		if(fp == NULL)
		{
			exit(1);
		}
		init_tree_header(fp);
	}
	return fp;
}

//closes the file stream
void close_tree(FILE *fp)
{
	fclose(fp);
}

//display the inorder traversal
void display_inorder(FILE *fp)
{
	tree_t t;
	read_header(&t, fp);
	//passing root only if it exists
	if(t.root != -1)
	{
		node_t root;
		read_node(&root, t.root, fp);
		inorder(&root, fp);
	}
	printf("\n");
}

//displays the preorder traversal
void display_preorder(FILE *fp)
{
	tree_t t;
	read_header(&t, fp);
	//passing root only if it exists
	if(t.root != -1)
	{
		node_t root;
		read_node(&root, t.root, fp);
		preorder(&root, fp);
	}
	printf("\n");
}

void insert_key(int key, FILE *fp)
{
	//reads the tree header
	tree_t t;
	read_header(&t, fp);

	//if no node is present
	if(t.root == -1)
	{
		//no empty nodes
		if(t.free_head == -1)
		{
			t.root = 0;
			write_header(&t, fp);

			node_t root;
			init_node(&root, key);
			write_node(&root, 0, fp);
		}
		
		//empty nodes are present
		else
		{
			node_t root;
			int pos = t.free_head; //offset of the new root
			read_node(&root, pos, fp);

			//updating tree header
			t.free_head = root.right_offset;
			t.root = pos;
			
			write_header(&t, fp);

			init_node(&root, key);
			write_node(&root, pos, fp); //storing it in "pos" since its free
		}
	}

	//some nodes are present already
	else
	{
		//offset of parent node
		int prev = search(key, t.root, fp);
		
		//duplicate key case
		if(prev == -1)
			return;

		//no empty nodes implies key needs to be written at the end of the file
		if(t.free_head == -1)
		{
			int pos = last_pos(fp); //gets the offset of the node after the last node

			set_parent_offset(prev, pos, key, fp); //sets the parent's offset

			node_t child;	
			init_node(&child, key);
			write_node(&child, pos, fp);
		}

		//empty nodes are present
		else
		{
			node_t child;
			int pos = t.free_head; //offset where new node is to be inserted as it is free
			read_node(&child, pos, fp);
			t.free_head = child.right_offset; //updating free list
			write_header(&t, fp);

			set_parent_offset(prev, pos, key, fp);
	
			init_node(&child, key);
			write_node(&child, pos, fp);
		}
	}
}

void delete_key(int key, FILE *fp)
{
	//reads tree header
	tree_t t;
	read_header(&t, fp);

	//tree is empty implies there is nothing to delete
	if(t.root == -1)
		return;

	int pres = t.root;
	int par_del = -1; //offset of parent of node to be deleted
	int del = -1; //offset of node to be deleted

	node_t curr; //node to delete

	//searching for node to delete
	while(pres != -1)
	{
		read_node(&curr, pres, fp);
		if(key == curr.key)
		{
			del = pres;
			break;
		}
		par_del = pres;
		if(key < curr.key)
		{
			pres = curr.left_offset;
		}
		else if(key > curr.key)
		{
			pres = curr.right_offset;
		}
	}
	
	if(del == -1) //key to delete not found
		return;

	//if node to be deleted has one subtree present or no subtrees
	if(curr.left_offset == -1 || curr.right_offset == -1)
	{
		int q; //just to hold the offset of left or right subtree of node to be deleted
		if(curr.left_offset == -1)
		{
			q = curr.right_offset;
		}
		else
		{
			q = curr.left_offset;
		}

		//root is to be deleted
		if(par_del == -1)
		{
			//adding root to free list
			curr.right_offset = t.free_head;
			t.free_head = t.root;
			write_node(&curr, del, fp);

			t.root = q; //updating offset of new root
			write_header(&t, fp);
			return;
		}

		//root is not the one to be deleted

		node_t node_par_del; //parent of node to be deleted
		read_node(&node_par_del, par_del, fp);

		//updating parent's offset appropriately
		if(node_par_del.left_offset == del)
		{
			node_par_del.left_offset = q;
			write_node(&node_par_del, par_del, fp);
		}
		else
		{
			node_par_del.right_offset = q;
			write_node(&node_par_del, par_del, fp);
		}

		//adding curr to free list
		curr.right_offset = t.free_head;
		t.free_head = del;
		write_node(&curr, del, fp);
		write_header(&t, fp);
	}

	//two subtrees are present for the node to be deleted
	else
	{
		int p = -1; //offset of parent of inorder successor
		int temp = curr.right_offset; //offset of inorder successor
		node_t node_temp; //inorder successor node
		read_node(&node_temp, temp, fp);

		//finding p and temp
		while(node_temp.left_offset != -1)
		{
			p = temp;
			temp = node_temp.left_offset;
			read_node(&node_temp, temp, fp);
		}

		//inorder succesor is not the right child of curr
		if(p != -1)
		{
			node_t node_p; //parent of inorder successor
			read_node(&node_p, p, fp);

			node_p.left_offset = node_temp.right_offset;
			write_node(&node_p, p, fp);
		}

		//inorder succesor is the right child of curr
		else
		{
			curr.right_offset = node_temp.right_offset;
		}

		//interchanging the inorder successor and curr's keys
		curr.key = node_temp.key;
		write_node(&curr, del, fp);

		//finally add inorder successor to free list
		node_temp.right_offset = t.free_head;
		write_node(&node_temp, temp, fp);
		t.free_head = temp;
		write_header(&t, fp);
	}
}
