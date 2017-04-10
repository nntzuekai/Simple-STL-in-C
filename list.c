#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<error.h>
#include "list.h"

void* list_malloc(size_t size_of_element) {
    void *p = malloc(size_of_element);
    if(p == NULL) {
        fprintf(stderr, "Memory allocating error!\n");
        exit(1);
    }
    return p;
}

List* list_create(List *plist, Comparer comp) {
    plist->count = 0;
    plist->head = NULL;
    plist->tail = NULL;
    plist->comp = comp;

    return plist;
}

bool list_is_empty(const List *plist) {
    return plist->head == NULL;
}

unsigned int list_count(const List *plist) {
    return plist->count;
}

Iterator list_head(const List *plist) {
    return plist->head;
}

Iterator list_tail(const List *plist) {
    return plist->tail;
}

List* list_destroy(List *plist) {
    // TODO (hrk#1#2017-4-9) add a error handling
    Iterator p1 = plist->head, p2;
    while(p1 != NULL) {
        p2 = p1;
        p1 = p1->next;
        free(p2);
        plist->count--;
    }
    if(plist->count == 0) {
        free(plist);
        return NULL;
    }
    else return plist;
}
//TODO (hrk#1#) Exception handling!
int list_push_head(List *plist, const Item item) {
    Node *pn;
    pn = (Node*)list_malloc(sizeof(Node));

    pn->item = item;
    pn->next = plist->head;
    pn->prev = NULL;

    if(plist->count == 0) {
        plist->tail = pn;
    }
    else {
        plist->head->prev = pn;
    }
    plist->head = pn;
    plist->count++;

    return 1;
}

int list_push_tail(List *plist, const Item item) {
    Node *pn;
    pn = (Node*)list_malloc(sizeof(Node));

    pn->item = item;
    pn->next = NULL;
    pn->prev = plist->tail;

    if(plist->count == 0) {
        plist->head = pn;
    }
    else {
        plist->tail->next = pn;
    }
    plist->tail = pn;
    plist->count++;

    return 1;
}

Item list_pop_head(List *plist) {
    // TODO (hrk#1#2017-4-9) Free error handling
    Node node_t = *(plist->head);
    Iterator pt = plist->head->next;
    pt->prev = NULL;
    free(plist->head);
    if(plist->head == plist->tail)plist->tail = NULL;
    plist->head = pt;
    plist->count--;

    return node_t.item;
}

Item list_pop_tail(List *plist) {
    // TODO (hrk#1#2017-4-9) Free error handling
    Node node_t = *(plist->tail);
    Iterator pt = plist->tail->prev;
    pt->next = NULL;
    if(plist->head == plist->tail)plist->head = NULL;
    free(plist->tail);
    plist->tail = pt;
    plist->count--;

    return node_t.item;
}

int list_swap(List *plist1, List *plist2) {
    List list_t;
    list_t = *plist1;
    *plist1 = *plist2;
    *plist2 = list_t;

    return 1;
}

Iterator list_insert(List *plist, Iterator pos, const Item item) {
    if(pos == plist->head) {
        return list_push_head(plist, item);
    }
    else {
        Iterator prev = pos->prev;
        Iterator add = (Iterator)list_malloc(sizeof(Node));

        add->item = item;
        add->prev = prev;
        add->next = pos;

        prev->next = add;
        pos->prev = add;

        plist->count++;
        if(pos==plist->head)plist->head=add;
        return add;
    }
}

Iterator list_insert2(List *plist, Iterator pos, Iterator first, Iterator last) {
    Iterator pt, prev = pos?pos->prev:NULL, next;

    for(pt = first;pt!=last; pt = pt->next) {
        next = (Node*)list_malloc(sizeof(Node));
        plist->count++;

        next->item = pt->item;
        next->prev = prev;
        next->next = NULL;

        if(prev)prev->next = next;
        else plist->head = next;
        prev = next;
    }

    next->next = pos;
    if(pos)pos->prev=next;
    return next;
}

Iterator list_erase(List *plist, Iterator pos) {
    if(pos == plist->head) {
        list_pop_head(plist);
        return NULL;
    }
    else if(pos == plist->tail) {
        list_pop_tail(plist);
        return NULL;
    }
    else {
        Iterator prev = pos->prev, next = pos->next;

        prev->next = next;
        next->prev = prev;
        plist->count--;
        free(pos);

        return next;
    }
}

Iterator list_erase2(List *plist, Iterator first, Iterator last) {
    Iterator pt1, pt2, prev = first->prev;

    for(pt1 = first; pt1 != last;) {
        pt2 = pt1->next;
        free(pt1);
        plist->count--;
        pt1 = pt2;
    }

    if(prev)prev->next = last;

    if(last)last->prev = prev;
    else plist->tail=NULL;

    if(plist->head==first)plist->head=last;

    return last;
}

int list_clear(List *plist) {
    Iterator pt1, pt2;
    for(pt1 = plist->head; pt1 != NULL; pt1 = pt2) {
        pt2 = pt1->next;
        free(pt1);
    }
    plist->count = 0;
    plist->head = NULL;
    plist->tail = NULL;
    return 1;
}

int list_equal(const List *plist1, const List *plist2) {
    if(plist1->comp!=plist2->comp){
        return 0;
    }
    if(plist1->count != plist2->count) {
        return 0;
    }
    else {
        Iterator pt1, pt2;
        for(pt1 = plist1->head, pt2 = plist2->head;
                pt1 != NULL && pt2 != NULL;
                pt1 = pt1->next, pt2 = pt2->next
           ) {
            if((*(plist1->comp))(&(pt1->item), &(pt2->item)) != 0) {
                return 0;
            }
        }
        return 1;
    }
}

int list_for_each(const List *plist, void(* foo)(Item*)) {
    Iterator pt = plist->head;
    int count = 0;
    for(; pt != NULL; pt = pt->next) {
        (*foo)(&(pt->item));
        count++;
    }

    return count;
}

int list_for_each_backward(const List *plist, void (*foo)(Item*)) {
    Iterator pt = plist->tail;
    int count = 0;
    for(; pt != NULL; pt = pt->prev) {
        (*foo)(&(pt->item));
        count++;
    }

    return count;
}

Iterator list_search(const List* plist, const Item *pitem) {
    Iterator pt;
    for(pt = plist->head; pt != NULL; pt = pt->next) {
        if((plist->comp(&(pt->item), pitem)) == 0)return pt;
    }
    return NULL;
}

Item *list_get_item_pointer(Iterator pt) {
    return &(pt->item);
}

Item list_get_item(Iterator pt) {
    return pt->item;
}

Iterator list_iterator_increase(Iterator pt){
    return pt?pt->next:NULL;
}

Iterator list_iterator_decrease(Iterator pt){
    return pt?pt->prev:NULL;
}
