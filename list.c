#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<error.h>
#include "list.h"

inline static void* list_malloc(size_t size_of_element) {
    void *p = malloc(size_of_element);
    if(p == NULL) {
        fprintf(stderr, "Memory allocating error!\n");
        exit(1);
    }
    return p;
}

inline static int list_default_comparer(const Item *pitem1, const Item *pitem2) {
    return memcmp(pitem1, pitem2, sizeof(Item));
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

Iterator list_insert(List *Dst, Iterator pos, const Item item) {
    if(pos == NULL) {
        return NULL;
    }
    /*else if(pos == Dst->head) {
        return list_push_front(Dst, item);
    }
    else if(pos == Dst->this) {
        return list_push_back(Dst, item);
    }*/
    else {
        Iterator prev = (pos == Dst->this) ? Dst->tail : pos->prev;
        Iterator add = (Iterator)list_malloc(sizeof(Node));
        Iterator Real_End = Dst->this;

        add->item = item;
        add->prev = prev;
        add->next = pos;

        if(prev != Real_End) {
            prev->next = add;
        }
        else {
            Dst->head = add;
        }

        if(pos != Real_End) {
            pos->prev = add;
        }
        else {
            Dst->tail = add;
        }

        Dst->count++;

        return add;
    }
}

Iterator list_insert2(List *Dst, Iterator pos, Iterator first, Iterator last) {
    if(pos == NULL || first == NULL || last == NULL || first == ((List*)first)->this) { //To guarantee the validity of "first"
        return NULL;
    }
    else {
        Iterator pt;
        Iterator prev = (pos == Dst->this) ? Dst->tail : pos->prev;
        Iterator add;
        Iterator Real_End = Dst->this;

        for(pt = first; pt != last; pt = pt->next) {
            add = (Node*)list_malloc(sizeof(Node));

            add->item = pt->item;
            add->prev = prev;
            add->next = pos;

            if(prev != Real_End) {
                prev->next = add;
            }
            else {
                Dst->head = add;
            }

            if(pos != Real_End) {
                pos->prev = add;
            }
            else {
                Dst->tail = add;
            }
            Dst->count++;


            prev = add;
        }

        return add;
    }
}

Iterator list_assign(List *Dst, const List *Src) {
    if(Src->head == Src->this) {
        return Dst->this;
    }
    else {
        Iterator pt;
        Iterator first = Src->head;
        Iterator add;
        Iterator last = Src->this;
        Iterator End1 = Dst->this, prev = End1;

        list_clear(Dst);

        for(pt = first; pt != last; pt = pt->next) {
            add = (Node*)list_malloc(sizeof(Node));

            add->item = pt->item;
            add->prev = prev;
            add->next = End1;

            if(prev != End1) {
                prev->next = add;
            }
            else {
                Dst->head = add;
            }

            Dst->tail = add;
            Dst->count++;


            prev = add;
        }

        return Dst->tail;
    }
}

Iterator list_erase(List * Dst, Iterator pos) {
    if(pos == NULL || pos == ((List*)pos)->this ) {
        return pos;
    }
    else {
        Iterator prev = pos->prev;
        Iterator next = pos->next;

        if(prev != Dst->this) {
            prev->next = next;
        }
        else {
            Dst->head = next;
        }

        if(next != Dst->this) {
            next->prev = prev;
        }
        else {
            Dst->tail = prev;
        }

        Dst->count--;
        free(pos);

        return next;
    }
}

Iterator list_erase2(List * Dst, Iterator first, Iterator last) {
    if(first == NULL || last == NULL || first == ((List*)first)->this) {
        return NULL;
    }
    else {
        Iterator pt1, pt2, prev = first->prev;

        if(prev != Dst->this) {
            prev->next = last;
        }
        else {
            Dst->head = last;
        }

        if(last != Dst->this) {
            last->prev = prev;
        }
        else {
            Dst->tail = prev;
        }

        for(pt1 = first; pt1 != last; pt1 = pt2) {
            pt2 = pt1->next;
            free(pt1);
            Dst->count--;
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

bool list_equal(const List * plist1, const List * plist2) {
    if(plist1 == plist2) {
        return 1;
    }
    else if(plist1->comp != plist2->comp) {
        if(plist1->comp == NULL) {
            fprintf(stderr, "Former List without specialized Comparer!\n");
            return 0;
        }
        else if(plist2->comp == NULL) {
            fprintf(stderr, "Latter List without specialized Comparer!\n");
            return 0;
        }
        else {
            fprintf(stderr, "Conflict Comparer!\n");
            return 0;
        }
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
        fprintf(stderr, "Invalid Iterator first OR last OR Invalid function OR Empty list!\n");
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
        fprintf(stderr, "Invalid Iterator first OR last OR Invalid function OR Empty list!\n");
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
        fprintf(stderr, "Invalid pointer!\n");
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
    if(plist == NULL) {
        fprintf(stderr, "plist is NULL!\n");
        return NULL;
    }
    else {
        unsigned int i = plist->count;
        if(i == n || (int)n < 0) {
            fprintf(stderr, "i==n OR n<0 \n");
            return plist->tail;
        }
        else if(n == 0) {
            fprintf(stderr, "n==0!\n");
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
}

int list_for_all(const List *plist, void (*foo)(Item*)) {
    if(plist->head == plist->this) {
        fprintf(stderr, "(Empty List)\n");
        return 0;
    }
    else {
        Iterator Real_End = plist->this;
        Iterator pt = plist->head;
        int count = 0;
        for(; pt != NULL && pt != Real_End; pt = pt->next) {
            (*foo)(&(pt->item));
            count++;
        }
        return count;
    }
}

int list_for_all_backward(const List *plist, void (*foo)(Item*)) {
    if(plist->head == plist->this) {
        fprintf(stderr, "(Empty List)\n");
        return 0;
    }
    else {
        Iterator Real_End = plist->this;
        Iterator pt = plist->tail;
        int count = 0;
        for(; pt != NULL && pt != Real_End; pt = pt->prev) {
            (*foo)(&(pt->item));
            count++;
        }
        return 0;
    }
}

Iterator list_splice(List *Dst, Iterator pos, List *Src) {
    if(Src->head == Src->this) {
        fprintf(stderr, "Source is Empty!\n");
        return pos;
    }
    else {
        Iterator End1 = Dst->this;
        Iterator pprev = (pos == End1) ? Dst->tail : pos->prev;

        if(pprev == End1) {
            Dst->head = Src->head;
        }
        else {
            pprev->next = Src->head;
        }
        Src->head->prev = pprev;

        if(pos == End1) {
            Dst->tail = Src->tail;
        }
        else {
            pos->prev = Src->tail;
        }
        Src->tail->next = pos;

        Dst->count += Src->count;
        Src->count = 0;
        Src->head = Src->tail = Src->this;

        return pos;
    }
}

Iterator list_splice1(List *Dst, Iterator pos, List *Src, Iterator x) {
    if(x == NULL || x == Src->this) {
        fprintf(stderr, "Iterator x is not valid!\n");
        return pos;
    }
    else if(pos == NULL) {
        fprintf(stderr, "Iterator pos is not valid!\n");
        return pos;
    }
    else if(pos == x || pos == x->next) {
        fprintf(stderr, "Iterator x equals to pos!\nOR is already before pos!\n");
        return  pos;
    }
    else {
        Iterator End1 = Dst->this;
        Iterator End2 = Src->this;
        Iterator prev1 = (pos == End1) ? Dst->tail : pos->prev;
        Iterator prev2 = x->prev;
        Iterator next2 = x->next;

        x->prev = prev1;
        x->next = pos;

        if(pos == End1) {
            Dst->tail = x;
        }
        else {
            pos->prev = x;
        }

        if(prev1 == End1) {
            Dst->head = x;
        }
        else {
            prev1->next = x;
        }

        ++Dst->count;
        --Src->count;

        if(prev2 == End2) {
            Src->head = next2;
        }
        else {
            prev2->next = next2;
        }

        if(next2 == End2) {
            Src->tail = prev2;
        }
        else {
            next2->prev = prev2;
        }

        return x;
    }
}

Iterator list_splice2(List *Dst, Iterator pos, List *Src, Iterator first, Iterator last) {
    if(pos == NULL) {
        fprintf(stderr, "NULL Iterator pos!\n");
        return NULL;
    }
    else if(first == NULL || first == Src->this) {
        fprintf(stderr, "Invalid Iterator first!\n");
        return NULL;
    }
    else if(last == NULL || first == last) {
        fprintf(stderr, "Invalid Iterator last!\n");
        return NULL;
    }
    else if(pos == last) {
        return pos;
    }
    else {
        int count = 0;
        Iterator pt = first;
        Iterator End1 = Dst->this;
        Iterator End2 = Src->this;

        for(; pt && pt != last; pt = pt->next) {
            if(pt == pos)break;
            count++;
        }
        if(pt == pos) {
            fprintf(stderr, "Iterator pos included in the range [first, last)!\n");
            return NULL;
        }
        else if(pt == NULL) {
            fprintf(stderr, "Bad List (Pointer to NULL)!\n");
            return NULL;
        }

        Iterator prev1 = (pos == End1) ? Dst->tail : pos->prev;
        Iterator prev2 = first->prev;
        Iterator back2 = (last == End2) ? Src->tail : last->prev;

        first->prev = prev1;
        back2->next = pos;

        if(prev1 == End1) {
            Dst->head = first;
        }
        else {
            prev1->next = first;
        }

        if(pos == End1) {
            Dst->tail = back2;
        }
        else {
            pos->prev = back2;
        }

        Dst->count += count;
        Src->count -= count;

        if(prev2 == End2) {
            Src->head = last;
        }
        else {
            prev2->next = last;
        }

        if(last == End2) {
            Src->tail = back2;
        }
        else {
            last->prev = prev2;
        }

        return pos;
    }

    return NULL;
}

Iterator list_remove(List *plist, const Item item) {
    Iterator pt = plist->head;
    Iterator end = plist->this;
    Iterator prev, next;
    Comparer cmp = plist->comp;
    if(cmp) {
        while(pt && pt != end) {
            if((*cmp)(&(pt->item), &item) == 0) {
                //pt = list_erase(plist, pt);
                prev = pt->prev;
                next = pt->next;

                if(prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if(next != end) {
                    next->prev = prev;
                }
                else {
                    plist->tail = prev;
                }

                plist->count--;
                free(pt);

                pt = next;
            }
            else pt = pt->next;
        }
    }
    else {
        while(pt && pt != end) {
            if(memcmp(&(pt->item), &item, sizeof(Item)) == 0) {
                //pt = list_erase(plist, pt);
                prev = pt->prev;
                next = pt->next;

                if(prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if(next != end) {
                    next->prev = prev;
                }
                else {
                    plist->tail = prev;
                }

                plist->count--;
                free(pt);

                pt = next;
            }
            else pt = pt->next;
        }
    }
    return pt;
}

Iterator list_unique(List *plist) {
    Iterator p1, p2, prev, next;
    p1 = plist->this;
    p2 = plist->head;
    Iterator end = plist->this;
    Comparer cmp = plist->comp;
    if(cmp) {
        while(p2 && p2 != end) {
            if(p1 != end && (*cmp)(&(p1->item), &(p2->item)) == 0) {
                //p2 = list_erase(plist, p2);
                prev = p2->prev;
                next = p2->next;

                if(prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if(next != end) {
                    next->prev = prev;
                }
                else {
                    plist->tail = prev;
                }

                plist->count--;
                free(p2);

                p2 = next;
            }
            else {
                p1 = p2;
                p2 = p2->next;
            }
        }
    }
    else {
        while(p2 && p2 != end) {
            if(p1 != end && memcmp(&(p1->item), &(p2->item), sizeof(Item)) == 0) {
                //p2 = list_erase(plist, p2);
                prev = p2->prev;
                next = p2->next;

                if(prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if(next != end) {
                    next->prev = prev;
                }
                else {
                    plist->tail = prev;
                }

                plist->count--;
                free(p2);

                p2 = next;
            }
            else {
                p1 = p2;
                p2 = p2->next;
            }
        }
    }
    return p2;
}

Iterator list_merge(List *Dst, List *Src) {
    if(Dst == Src) {
        fprintf(stderr, "The same List!\n");
        return NULL;
    }
    else if(Dst->comp != Src->comp) {
        if(Dst->comp == NULL) {
            fprintf(stderr, "Former List without specialized Comparer!\n");
            return NULL;
        }
        else if(Src->comp == NULL) {
            fprintf(stderr, "Latter List without specialized Comparer!\n");
            return NULL;
        }
        else {
            fprintf(stderr, "Conflict Comparer!\n");
            return NULL;
        }
    }
    else {
        Iterator p1 = Dst->head;
        Iterator p2 = Src->head;
        Iterator end1 = Dst->this;
        Iterator end2 = Src->this;
        Iterator next;
        Comparer cmp = Dst->comp ? Dst->comp : list_default_comparer;
        while(p1 && p2 && p2 != end2) {
            if((*cmp)(&(p1->item), &(p2->item)) > 0) {
                next = p2->next;
                list_splice1(Dst, p1, Src, p2);
                p2 = next;
            }
            else {
                p1 = p1->next;
                if(p1 == end1) {
                    list_splice2(Dst, p1, Src, p2, Src->this);
                    break;
                }
            }
        }
        return p1;
    }
}

bool list_less(const List *plist1, const List *plist2) {
    if(plist1 == plist2) {
        return 0;
    }
    else if(plist1->comp != plist2->comp) {
        if(plist1->comp == NULL) {
            fprintf(stderr, "Former List without specialized Comparer!\n");
            return 0;
        }
        else if(plist2->comp == NULL) {
            fprintf(stderr, "Latter List without specialized Comparer!\n");
            return 0;
        }
        else {
            fprintf(stderr, "Conflict Comparer!\n");
            return 0;
        }
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
                if( (*cmp)(&(pt1->item), &(pt2->item)) < 0) {
                    return 1;
                }
            }
            if(pt1 == end1 && pt2 == end2)return 0;
            else if(pt1 == end1)return 1;
            else return 0;
        }
        else {
            for(pt1 = plist1->head, pt2 = plist2->head;
                    pt1 != end1 && pt2 != end2;
                    pt1 = pt1->next, pt2 = pt2->next)
            {
                if( memcmp(&(pt1->item), &(pt2->item), sizeof(Item)) < 0) {
                    return 1;
                }
            }
            if(pt1 == end1 && pt2 == end2)return 0;
            else if(pt1 == end1)return 1;
            else return 0;
        }
    }
    return 0;
}


#ifdef SORT_MODE_QUICK
static void list_quick_sort(Comparer cmp, Iterator left, Iterator right){
    if(left==right||right->next==left||((List*)right)->head==left){
        return;
    }
    Iterator i=left,j=right;
    Item k=left->item;
    while(i!=j){
        while(i!=j&&cmp(&(j->item),&k)>=0){
            j=j->prev;
        }
        if(i!=j){
            i->item=j->item;
        }

        while(i!=j&&cmp(&(i->item),&k)<=0){
            i=i->next;
        }
        if(i!=j){
            j->item=i->item;
        }
    }
    i->item=k;
    list_quick_sort(cmp,left,i->prev);
    list_quick_sort(cmp,i->next,right);
}


Iterator list_sort(List *plist){
    if(plist==NULL||plist->this!=(Iterator)plist){
        fprintf(stderr,"Uninitialized List!\n");
        return NULL;
    }
    else if(plist->count==0){
        fprintf(stderr,"Empty List!\n");
        return NULL;
    }
    else{
        list_quick_sort(plist->comp?plist->comp:list_default_comparer,plist->head,plist->tail);
        return plist->head;
    }
}

#endif // SORT_MODE_QUICK
