int min(int x, int y);

struct capacity
{
	int A;
	int B;
	int C;
};
typedef struct capacity capacity;

void init_capacity(capacity *s, int A, int B, int C);

struct state
{
	int a;
	int b;
	int c;
	int fn_index; //needed for state space search
};
typedef struct state state;

void init_state(state *s, int a, int b, int c);
void disp_state(const state* s);
void moveAB(const state *src, state *dst, const capacity *cap);
void moveAC(const state *src, state *dst, const capacity *cap);
void moveBA(const state *src, state *dst, const capacity *cap);
void moveBC(const state *src, state *dst, const capacity *cap);
void moveCA(const state *src, state *dst, const capacity *cap);
void moveCB(const state *src, state *dst, const capacity *cap);
int are_same(const state *s, const state *t);

struct node
{
	state st;
	struct node *prev;
	struct node *next;
};
typedef struct node node;

struct list
{
	node *head;
	node *tail;
};
typedef struct list list;

void init_list(list *ptr_list);
void push(const state *ptr_state, list *ptr_list);
void pop(list *ptr_list);
int is_empty(list *ptr_list);
int is_repeated(const state *s, list *ptr_list);
void disp_list(list *ptr_list);
