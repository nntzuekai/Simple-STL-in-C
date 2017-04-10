#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include<stdbool.h>
#include<stddef.h>

struct _item {
    int coe, pow;
};
typedef struct _item Item;

typedef int (*Comparer)(const Item*, const Item*);

struct _node {
    struct _node *prev, *next;
    Item item;
};
typedef struct _node Node;
typedef Node *Iterator;

struct _list {
    Iterator head, tail;
    int count;
    Comparer comp;
};
typedef struct _list List;

void* list_malloc(size_t size_of_element);

List* list_create(List *plist, Comparer comp) ;

bool list_is_empty(const List *plist);

unsigned int list_count(const List *plist);

Iterator list_head(const List *plist);

Iterator list_tail(const List *plist);

List* list_destroy(List *plist);

int list_push_head(List *plist, const Item item);

int list_push_tail(List *plist, const Item item);

Item list_pop_head(List *plist);

Item list_pop_tail(List *plist);

int list_swap(List *plist1, List *plist2);

Iterator list_insert(List *plist, Iterator pos, const Item item);

Iterator list_insert2(List *plist, Iterator pos, Iterator first, Iterator last);

Iterator list_erase(List *plist, Iterator pos);

Iterator list_erase2(List *plist, Iterator first, Iterator last);

int list_clear(List *plist);

int list_equal(const List *plist1, const List *plist2);

int list_for_each(const List *plist, void (*foo)(Item*));

int list_for_each_backward(const List *plist, void (*foo)(Item*));

Iterator list_search(const List *plist, const Item*);

Item *list_get_item_pointer(Iterator pt);

Item list_get_item(Iterator pt);

Iterator list_iterator_increase(Iterator pt);

Iterator list_iterator_decrease(Iterator pt);
#endif // LIST_H_INCLUDED
