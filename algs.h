#ifndef STANDARD_DEFINITIONS
#define STANDARD_DEFINITIONS 1
#define TRUE 1
#define FALSE 0
#define VERBOSE TRUE
typedef char BOOL;
#endif 
//end of STANDARD_DEFINITIONS check

int __root(int i, int* array)
{
	while(i != array[i])
	{
		i = array[i];
	}
	return i;
}

#ifndef QUICK_UNION
#define QUICK_UNION 1
typedef struct __quick_union quick_union;
//quick union check
//quick union implementation
struct __quick_union
{
	int* array;
	int sz;
	char object_id[80];
	BOOL (*is_connected)(quick_union*, int, int);
	void (*link)(quick_union*, int, int);
	void (*print)(quick_union*);
};

BOOL QU__is_connected(quick_union* obj, int arg1, int arg2)
{
	return (__root(arg1, obj->array) == __root(arg2, obj->array));
}

void QU__link(quick_union* obj, int arg1, int arg2)
{
	int J = __root(arg1, obj->array);
	int I = __root(arg2, obj->array);
	obj->array[I] = J;
}

void QU__print(quick_union* obj)
{
	printf("\n");
	for(int i = 0; i < obj->sz; i++)
	{
		printf("%d ", obj->array[i]);
	}
}

quick_union* init_q_union(int n, const char* name)
{
	quick_union* Q = malloc(1*sizeof(struct __quick_union));
	Q->array = malloc(n*sizeof(int));
	Q->sz = n;
	strcpy(Q->object_id, name);
	Q->is_connected = QU__is_connected;
	Q->link = QU__link;
	Q->print = QU__print;
	for(int i = 0; i < Q->sz; i++)
	{
		Q->array[i] = i;
	}
	return Q;
}

void destroy_q_union(quick_union* obj)
{
	char name[80];
	strcpy(name, (const char*)obj->object_id);
	free(obj->array);
	free(obj);
	if(VERBOSE)
		printf("\nsuccessfully destroyed quick find object with object id: %s\n", name);
}
#endif
//end of conditional quick union implemintation

#ifndef WEIGHTED_QUICK_UNION
#define WEIGHTED_QUICK_UNION 1
typedef struct __weighted_quick_union weighted_quick_union;
//beggining of weighted quick union implementation
struct __weighted_quick_union
{
	int* array;
	int sz;
	int* tree_size;
	char object_id[80];
	BOOL (*is_connected)(weighted_quick_union*, int, int);
	void (*link)(weighted_quick_union*, int, int);
	void (*print)(weighted_quick_union*);
};

BOOL WQU__is_connected(weighted_quick_union* obj, int arg1, int arg2)
{
	return (__root(arg1, obj->array) == __root(arg2, obj->array));
}

void WQU__link(weighted_quick_union* obj, int arg1, int arg2)
{
	int J = __root(arg1, obj->array);
	int I = __root(arg2, obj->array);
	if(I == J)
		goto END;
	if(obj->tree_size[I] <= obj->tree_size[J])
	{
		obj->array[I] = J;
		obj->tree_size[J] += obj->tree_size[I];
	}
	else
	{
		obj->array[J] = I;
		obj->tree_size[I] += obj->tree_size[J];
	}
	END:;
}

void WQU__print(weighted_quick_union* obj)
{
	printf("\n");
	for(int i = 0; i < obj->sz; i++)
	{
		printf("%d ", obj->array[i]);
	}
}

weighted_quick_union* init_weighted_q_union(int n, const char* name)
{
	weighted_quick_union* Q = malloc(1*sizeof(struct __weighted_quick_union));
	Q->array = malloc(n*sizeof(int));
	Q->tree_size = malloc(n*sizeof(int));
	Q->sz = n;
	strcpy(Q->object_id, name);
	Q->is_connected = WQU__is_connected;
	Q->link = WQU__link;
	Q->print = WQU__print;
	for(int i = 0; i < Q->sz; i++)
	{
		Q->array[i] = i;
		Q->tree_size[i] = 1;
	}
	return Q;
}

void destroy_weighted_q_union(weighted_quick_union* obj)
{
	char name[80];
	strcpy(name, (const char*)obj->object_id);
	free(obj->array);
	free(obj->tree_size);
	free(obj);
	if(VERBOSE)
		printf("\nsuccessfully destroyed quick find object with object id: %s\n", name);
}
#endif
//end of weighted quick union conditional implemintation
#ifndef QUICK_FIND
#define QUICK_FIND 1
typedef struct __quick_find quick_find;

struct __quick_find
{
	int* array;
	int sz;
	char object_id[80];
	BOOL (*is_connected)(quick_find*, int, int);
	void (*link)(quick_find*, int, int);
	void (*print)(quick_find*);
};

BOOL QF__is_connected(quick_find* Q, int node1, int node2)
{
	if(Q->array[node1] == Q->array[node2])
		return TRUE;
	else
		return FALSE;
}

void QF__link(quick_find* Q, int node1, int node2)
{
	int pid = Q->array[node1];
	int qid = Q->array[node2];
	for(int i = 0; i < Q->sz; i++)
	{
		if(Q->array[i] == pid)
		{
			Q->array[i] = qid;
		}
	}
}

void QF__print(quick_find* obj)
{
	printf("\n");
	for(int i = 0; i < obj->sz; i++)
	{
		printf("%d ", obj->array[i]);
	}
}

quick_find* init_q_find(int n, const char* name)
{
	quick_find* Q = malloc(1*sizeof(struct __quick_find));
	Q->array = malloc(n*sizeof(int));
	Q->sz = n;
	strcpy(Q->object_id, name);
	Q->is_connected = QF__is_connected;
	Q->link = QF__link;
	Q->print = QF__print;
	for(int i = 0; i < Q->sz; i++)
	{
		Q->array[i] = i;
	}
	return Q;
}

void destroy_q_find(quick_find* obj)
{
	char name[80];
	strcpy(name, (const char*)obj->object_id);
	free(obj->array);
	free(obj);
	if(VERBOSE)
		printf("\nsuccessfully destroyed quick find object with object id: %s\n", name);
}
#endif
//end of conditional quick find implementation
