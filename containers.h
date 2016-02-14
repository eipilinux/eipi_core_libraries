#ifndef STANDARD_DEFINITIONS
#define STANDARD_DEFINITIONS 1
#define TRUE 1
#define FALSE 0
#define VERBOSE TRUE
typedef char BOOL;
#endif 
//end of STANDARD_DEFINITIONS check

#ifndef MATRIX
#define MATRIX 1
typedef struct __matrix matrix;

struct __matrix
{
	float* data;
	int num_elements, num_deminsions;
	void (*set_element)(matrix*, int*, float);
};

void __set_element(matrix* obj, int* location, float value)
{
	*(obj->data) = value;
}

#endif
//end of conditional matrix implementation definition
#ifndef STACK_ARRAY
#define STACK_ARRAY 1
typedef struct __stack_array stack_array;

struct __stack_array
{
	int* array;
	int sz;
	char object_id[80];
	void (*push)(stack_array*, int);
	void (*pop)(stack_array*, int*);
};

void AR__push(stack_array* obj, int arg1)
{
	
}

void AR__pop(stack_array* obj, int* result)
{
	
}
#endif
//end of conditional stack_array implementation definition

#ifndef STACK_LINKED_LIST
#define STACK_LINKED_LIST 1
typedef struct __stack_linked_list stack_linked_list;

struct __node
{
	int n_value;
	struct __node* next;
};

struct __stack_linked_list
{
	struct __node* linked_list;
	int sz;
	char object_id[80];
	void (*push)(stack_linked_list*, int);
	void (*pop)(stack_linked_list*, int*);
};

void __push(stack_linked_list* obj, int val)
{
	//traverse linked list and add element to end of list
}

void __pop(stack_linked_list* obj, int* return_val)
{
	//traverse linked list and return value at end of list, then delete last element from list
}
#endif
//end of conditional stack_linked_list implementation definition
