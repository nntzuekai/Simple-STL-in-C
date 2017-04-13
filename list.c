#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<error.h>
#include "list.h"

static void* list_malloc(size_t size_of_element) {
    void *p = malloc(size_of_element);
    if(p == NULL) {
        fprintf(stderr, "Memory allocating error!\n");
        exit(1);
    }
    return p;
}

List* list_create(List *plist, Comparer comp) {
    plist->this = (Iterator)(&(plist->this));
    plist->count = 0;
    plist->head = (Iterator)(&(plist->this));
    plist->tail = (Iterator)(&(plist->this));
    plist->comp = comp;

    return plist;
}

bool list_empty(const List *plist) {
    return plist->head == plist->this;
}

unsigned int list_size(const List *plist) {
    return plist->count;
}

Iterator list_iterator_increase(Iterator pt) {
    if(pt == NULL)return NULL;
    else return (pt == ((List*)pt)->this ) ? ((List*)pt)->head : pt->next;
}

Iterator list_iterator_decrease(Iterator pt) {
    if(pt == NULL)return NULL;
    else return (pt == ((List*)pt)->this ) ? ((List*)pt)->tail : pt->prev;
}

Iterator list_begin(const List *plist) {
    return plist->head;
}

Iterator list_end(const List *plist) {
    return plist->this;
}

Item list_front(const List *plist) {
    if(plist->head == plist->this) {
        Item item_t;
        fprintf(stderr, "Empty list! Meaningless return value!\n");
        return item_t;
    }
    else {
        return plist->head->item;
    }
}

Item list_back(const List *plist) {
    if(plist->tail == plist->this) {
        Item item_t;
        fprintf(stderr, "Empty list! Meaningless return value!\n");
        return item_t;
    }
    else {
        return plist->tail->item;
    }
}

List* list_destroy(List *plist) {
    // TODO (hrk#1#2017-4-9) add a error handling
    Iterator p1 = plist->head, p2, pend = plist->this;
    while(p1 != pend) {
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
Iterator list_push_front(List *plist, const Item item) {
    Node *pn;
    pn = (Node*)list_malloc(sizeof(Node));

    pn->item = item;
    pn->next = plist->head;
    pn->prev = plist->this;

    if(plist->tail == plist->this) { //To judge if list is empty
        plist->tail = pn;
    }
    else {
        plist->head->prev = pn;
    }
    plist->head = pn;
    plist->count++;

    return pn;
}

Iterator list_push_back(List *plist, const Item item) {
    Node *pn;
    pn = (Node*)list_malloc(sizeof(Node));

    pn->item = item;
    pn->next = plist->this;
    pn->prev = plist->tail;

    if(plist->head == plist->this) { //To judge if list is empty
        plist->head = pn;
    }
    else {
        plist->tail->next = pn;
    }
    plist->tail = pn;
    plist->count++;

    return pn;
}

Item list_pop_front(List *plist) {
    // TODO (hrk#1#2017-4-9) Free error handling
    Item item_t;
    memset(&item_t, 0, sizeof(item_t));

    if(plist->head == plist->this) {
        fprintf(stderr, "Empty list! Meaningless return value!\n");
        return item_t;
    }
    else {
        item_t = plist->head->item;
        Iterator pnext = plist->head->next;

        if(pnext == plist->this) {
            plist->tail = plist->this;
        }
        else {
            pnext->prev = plist->this;
        }

        free(plist->head);
        plist->head = pnext;
        plist->count--;

        return item_t;
    }
}

Item list_pop_back(List *plist) {
    // TODO (hrk#1#2017-4-9) Free error handling
    Item item_t;
    memset(&item_t, 0, sizeof(item_t));

    if(plist->head == plist->this) {
        fprintf(stderr, "Empty list! Meaningless return value!\n");
        return item_t;
    }
    else {
        item_t = plist->tail->item;
        Iterator pprev = plist->tail->prev;

        if(pprev == plist->this) {
            plist->head = plist->this;
        }
        else {
            pprev->next = plist->this;
        }
        free(plist->tail);
        plist->tail = pprev;
        plist->count--;

        return item_t;
    }
}

int list_swap(List *plist1, List *plist2) {
    List list_t;
    list_t = *plist1;
    *plist1 = *plist2;
    *plist2 = list_t;

    return 1;
}

Iterator list_insert(List *plist, Iterator pos, const Item item) {
    if(pos == NULL) {
        return NULL;
    }
    /*else if(pos == plist->head) {
        return list_push_front(plist, item);
    }
    else if(pos == plist->this) {
        return list_push_back(plist, item);
    }*/
    else {
        Iterator prev = (pos == plist->this) ? plist->tail : pos->prev;
        Iterator add = (Iterator)list_malloc(sizeof(Node));
        Iterator Real_End = plist->this;

        add->item = item;
        add->prev = prev;
        add->next = pos;

        if(prev != Real_End) {
            prev->next = add;
        }
        else {
            plist->head = add;
        }

        if(pos != Real_End) {
            pos->prev = add;
        }
        else {
            plist->tail = add;
        }

        plist->count++;

        return add;
    }
}

Iterator list_insert2(List *plist, Iterator pos, Iterator first, Iterator last) {
    if(pos == NULL || first == NULL || last == NULL || first == ((List*)first)->this) { //To guarantee the validity of "first"
        return NULL;
    }
    else {
        Iterator pt;
        Iterator prev = (pos == plist->this) ? plist->tail : pos->prev;
        Iterator add;
        Iterator Real_End = plist->this;

        for(pt = first; pt != last; pt = pt->next) {
            add = (Node*)list_malloc(sizeof(Node));

            add->item = pt->item;
            add->prev = prev;
            add->next = pos;

            if(prev != Real_End) {
                prev->next = add;
            }
            else {
                plist->head = add;
            }

            if(pos != Real_End) {
                pos->prev = add;
            }
            else {
                plist->tail = add;
            }
            plist->count++;


            prev = add;
        }

        return add;
    }
}

Iterator list_assign(List *plist1, const List *plist2) {
    if(plist2->head == plist2->this) {
        return plist1->this;
    }
    else {
        Iterator pt;
        Iterator first = plist2->head;
        Iterator add;
        Iterator last = plist2->this;
        Iterator End1 = plist1->this, prev = End1;

        list_clear(plist1);

        for(pt = first; pt != last; pt = pt->next) {
            add = (Node*)list_malloc(sizeof(Node));

            add->item = pt->item;
            add->prev = prev;
            add->next = End1;

            if(prev != End1) {
                prev->next = add;
            }
            else {
                plist1->head = add;
            }

            plist1->tail = add;
            plist1->count++;


            prev = add;
        }

        return plist1->tail;
    }
}
Iterator list_erase(List * plist, Iterator pos) {
    if(pos == NULL || pos == ((List*)pos)->this ) {
        return pos;
    }
    else {
        Iterator prev = pos->prev;
        Iterator next = pos->next;

        if(prev != plist->this) {
            prev->next = next;
        }
        else {
            plist->head = next;
        }

        if(next != plist->this) {
            next->prev = prev;
        }
        else {
            plist->tail = prev;
        }

        plist->count--;
        free(pos);

        return next;
    }
}

Iterator list_erase2(List * plist, Iterator first, Iterator last) {
    if(first == NULL || last == NULL || first == ((List*)first)->this) {
        return NULL;
    }
    else {
        Iterator pt1, pt2, prev = first->prev;

        if(prev != plist->this) {
            prev->next = last;
        }
        else {
            plist->head = last;
        }

        if(last != plist->this) {
            last->prev = prev;
        }
        else {
            plist->tail = prev;
        }

        for(pt1 = first; pt1 != last; pt1 = pt2) {
            pt2 = pt1->next;
            free(pt1);
            plist->count--;
        }

        return last;
    }
}

int list_clear(List * plist) {
    Iterator pt1, pt2;
    Iterator Real_End = plist->this;

    for(pt1 = plist->head; pt1 != Real_End; pt1 = pt2) {
        pt2 = pt1->next;
        free(pt1);
    }

    plist->count = 0;
    plist->head = Real_End;
    plist->tail = Real_End;

    return 1;
}

int list_equal(const List * plist1, const List * plist2) {
    if(plist1->comp == NULL && plist2->comp != NULL) {
        fprintf(stderr, "Former list without Comparer!\n");
        return 0;
    }
    else if(plist2->comp == NULL && plist1->comp != NULL) {
        fprintf(stderr, "Latter list without Comparer!\n");
        return 0;
    }
    else if(plist1->count != plist2->count) {
        return 0;
    }
    else if(plist1->comp != plist2->comp) {
        return 0;
    }
    else {
        Iterator pt1, pt2;
        Iterator end1 = plist1->this, end2 = plist2->this;

        if(plist1->comp && plist2->comp) {
            Comparer cmp = plist1->comp;

            for(pt1 = plist1->head, pt2 = plist2->head;
                    pt1 != end1 && pt2 != end2;
                    pt1 = pt1->next, pt2 = pt2->next)
            {
                if( (*cmp)(&(pt1->item), &(pt2->item)) != 0) {
                    return 0;
                }
            }
            if(pt1 == end1 && pt2 == end2)return 1;
            else return 0;
        }
        else {
            for(pt1 = plist1->head, pt2 = plist2->head;
                    pt1 != end1 && pt2 != end2;
                    pt1 = pt1->next, pt2 = pt2->next)
            {
                if( memcmp(&(pt1->item), &(pt2->item), sizeof(Item)) != 0) {
                    return 0;
                }
            }
            if(pt1 == end1 && pt2 == end2)return 1;
            else return 0;
        }
    }
    return 0;
}

int list_for_each(Iterator first, Iterator last, void (*foo)(Item*)) {
    if(first == NULL || last == NULL || foo == NULL || first == ((List*)first)->this) {
        return 0;
    }
    else {
        Iterator pt = first;
        int count = 0;

        for(; pt != last; pt = pt->next) {
            (*foo)(&(pt->item));
            count++;
        }

        return count;
    }
}

int list_for_each_backward(Iterator first, Iterator last, void (*foo)(Item*)) {
    if(first == NULL || last == NULL || foo == NULL || first == ((List*)first)->this) {
        return 0;
    }
    else {
        Iterator pt = (last == ((List*)last)->this ) ? ((List*)last)->tail : last->prev;
        Iterator off_first = first->prev;
        int count = 0;

        for(; pt != off_first; pt = pt->prev) {
            (*foo)(&(pt->item));
            count++;
        }

        return count;
    }
}

Iterator list_find(const List * plist, const Item * pitem) {
    Iterator pt = plist->head;
    Comparer cmp = plist->comp;

    if(cmp == NULL) {
        for(; pt != plist->this ; pt = pt->next) {
            if(memcmp(&(pt->item), pitem, sizeof(Item)) == 0)return pt;
        }
    }
    else {
        for(; pt != plist->this ; pt = pt->next) {
            if(( (*cmp)(&(pt->item), pitem) ) == 0)return pt;
        }
    }
    return NULL;
}

Item *list_get_item_ptr(Iterator pt) {
    if(pt == NULL || pt == ((List*)pt)->this) {
        return NULL;
    }
    else return &(pt->item);
}

Item list_get_item(Iterator pt) {
    if(pt == NULL || pt == ((List*)pt)->this) {
        Item item_t;
        //memset(&item_t, 0, sizeof(item_t));
        fprintf(stderr, "Empty list! Meaningless return value!\n");
        return item_t;
    }
    else return pt->item;
}

Iterator list_resize(List* plist, unsigned int n) {
    if(plist == NULL)return NULL;

    unsigned int i = plist->count;
    if(i == n || (int)n < 0) {
        return plist->tail;
    }
    else if(n==0){
        list_clear(plist);
    }
    else if(i > n) {
        while(i-- > n) {
            //list_pop_back(plist);
            Iterator pprev = plist->tail->prev;

            if(pprev == plist->this) {
                plist->head = plist->this;
            }
            else {
                pprev->next = plist->this;
            }

            free(plist->tail);
            plist->tail = pprev;
            plist->count--;
        }
        return plist->tail;
    }
    else if(i < n) {
        Item item;
        memset(&item, 0, sizeof(item));
        while(i++ < n) {
            //list_push_back(plist,item);
            Node *pn;
            pn = (Node*)list_malloc(sizeof(Node));

            pn->item = item;
            pn->next = plist->this;
            pn->prev = plist->tail;

            if(plist->head == plist->this) { //To judge if list is empty
                plist->head = pn;
            }
            else {
                plist->tail->next = pn;
            }
            plist->tail = pn;
            plist->count++;
        }
        return plist->tail;
    }

    return NULL;
}
