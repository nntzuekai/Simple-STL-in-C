#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include<stdbool.h>
#include<stddef.h>

struct _item {
    double coe, pow;
};
typedef struct _item Item;

typedef int (*Comparer)(const Item*, const Item*);
typedef void (*Unary_Func)(Item *);

struct _node {
    struct _node *prev, *next;
    Item item;
};
typedef struct _node Node;
typedef Node *Iterator;

struct _list {
    Iterator this;
    Iterator head, tail;
    unsigned int count;
    Comparer comp;
};
typedef struct _list List;

List* list_create(List *plist, Comparer comp) ;

Iterator list_iterator_increase(Iterator pt);

Iterator list_iterator_decrease(Iterator pt);

bool list_empty(const List *plist);

unsigned int list_size(const List *plist);

Iterator list_begin(const List *plist);

Iterator list_end(const List *plist);

Item list_front(const List *plist);

Item list_back(const List *plist);

Iterator list_assign(List *plist1, const List *plist2);

Iterator list_insert(List *plist, Iterator pos, const Item item);

Iterator list_erase(List *plist, Iterator pos);

int list_clear(List *plist);

List* list_destroy(List *plist);

Iterator list_push_front(List *plist, const Item item);

Iterator list_push_back(List *plist, const Item item);

Item list_pop_front(List *plist);

Item list_pop_back(List *plist);

int list_swap(List *plist1, List *plist2);

Iterator list_insert2(List *plist, Iterator pos, Iterator first, Iterator last);

Iterator list_erase2(List *plist, Iterator first, Iterator last);

int list_equal(const List *plist1, const List *plist2);

int list_for_each(Iterator first, Iterator last, void (*foo)(Item*));

int list_for_each_backward(Iterator first, Iterator last, void (*foo)(Item*));

Iterator list_find(const List *plist, const Item*);

Item *list_get_item_ptr(Iterator pt);

Item list_get_item(Iterator pt);

Iterator list_resize(List *plist, unsigned int n);

#endif // LIST_H_INCLUDED
