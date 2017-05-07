#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "list.h"

//The static function to show error message if error happened.
//The message will be put to stderr for it with no buffer.
//So even the buffer has not been flushed, we can still see the message.
//The message will be print in the format as follows
//
//********************
//Error in [location] :
//    [err_msg]
//********************
//
//Return what fprintf returns.
inline static int list_error(char *location, char *err_msg) {
    return fprintf(stderr, "\n********************\nError in %s :\n\t%s\n********************\n", location, err_msg);
}

//Static memory allocating function.
//Return a void* .
inline static void* list_malloc(size_t size_of_element) {
    void *p = malloc(size_of_element);
    if (p == NULL) {    //The handle the case that no memory to allocate.
        list_error("list_malloc", "Memory allocating error!");
        exit(1);
    }
    return p;
}

//The default comparer to compare two items.
//Using the memcmp to compare them by bytes.
//Better not to use this, for it may not work correctly in most cases.
inline static int list_default_comparer(const Item *pitem1, const Item *pitem2) {
    return memcmp(pitem1, pitem2, sizeof(Item));
}

//The constructor to initialize a List uninitialized.
//If plist is already initialized, that causes memory leak.
//comp is the Comparer the users defined to compare the type Item defined.
//comp should return the result of some rule as follows.
//If in the rule:
//i1 < i2 ,then comp(&i1 , &i2)<0
//i1 == i2 ,                   =0
//i1 > i2 ,                   >0
List* list_create(List *plist, Comparer comp) {
    plist->this = (Iterator)(&(plist->this));   //Let this point to the position of the list, also itself because it is the first field.
    plist->count = 0;   //No element inside.
    plist->head = (Iterator)(&(plist->this));   //Assign head and tail.
    plist->tail = (Iterator)(&(plist->this));
    plist->comp = comp;

    return plist;   //Return plist.
}

//Return if plist is empty.
bool list_empty(const List *plist) {
    return plist->head == plist->this;
}

//Return the number of elements in the list.
unsigned int list_size(const List *plist) {
    return plist->count;
}

//Increase a forward iterator.
//Return the iterator increased, and pt is not changed.
//A correct sample: pt = list_iter_incrs(pt);
//If pt reach the bounder, it stays.That avoids the error resulted from wrong pointer.
Iterator list_iter_incrs(Iterator pt) {
    if (pt == NULL)return NULL;     //Invalid iterator.
    else return (pt == ((List*)pt)->this) ? pt : pt->next;  //If pt point to itself, it reach the end bounder and we just return it no changed.
}

//Decrease a forward iterator.
//Similar to above.
//Remember that the range of an iterator is [head , this], namely head >= tail >= this.
Iterator list_iter_decrs(Iterator pt) {
    if (pt == NULL)return NULL;
    else return (pt == ((List*)pt)->this) ? ((List*)pt)->tail : pt->prev;   //If pt point to itself, the previous one must be tail.
}

//Decrease a reverse iterator.
//In most cases, it equals to increasing a forward iterator.
//But when if  pt == this , it returns itself.
Iterator list_riter_incrs(Iterator pt) {
    if (pt == NULL)return NULL;
    else return (pt == ((List*)pt)->this) ? pt : pt->prev;      //Remember the ranger of a r-iterator is [this , tail], namely this >= head >= tail.
}

//Increase a reverse iterator.
//In most cases, it equals to decreasing a forward iterator.
Iterator list_riter_decrs(Iterator pt) {
    if (pt == NULL)return NULL;
    else return (pt == ((List*)pt)->this) ? ((List*)pt)->head : pt->next;       //But when pt points to itself, returns head.
}

//Return the begin iterator of a list.
Iterator list_begin(const List *plist) {
    return plist->head;
}

//Return the end iterator of a list, in other words, plist->this.
Iterator list_end(const List *plist) {
    return plist->this;
}

//Return the begin reverse-iterator of a list.
Iterator list_rbegin(const List *plist) {
    return plist->tail;
}

//Return the end reverse-iterator of a list (plist->this).
Iterator list_rend(const List *plist) {
    return plist->this;
}

//Return the first Item of a list.
Item list_front(const List *plist) {
    if (plist->head == plist->this) {   //Empty list.
        Item item_t;
        list_error("list_front", "Empty list! Meaningless return value!");
        return item_t;
    }
    else {
        return plist->head->item;
    }
}

//Return the last Item of a list.
Item list_back(const List *plist) {
    if (plist->tail == plist->this) {   //Empty list.
        Item item_t;
        list_error("list_back", "Empty list! Meaningless return value!");
        return item_t;
    }
    else {
        return plist->tail->item;
    }
}

//The destructor, it should ONLY be applied to a List DYNAMICALLY allocated.
//The function will delete all contents, and then delete the List itself.
//If plist is not dynamically allocated, we can not free it, which causes an error.
//I advise to use in that way: plist = list_destroy(plist);
//In that way, we can check if plist == NULL to know if the destructor worked successfully.
List* list_destroy(List *plist) {
    // TODO (hrk#1#2017-4-9) add a error handling
    if(plist == NULL)return NULL;   //Avoid to deleting a NULL pointer.
    Iterator p1 = plist->head, p2, pend = plist->this;
    while (p1 != pend) {    //Delete all elements.
        p2 = p1;
        p1 = p1->next;
        free(p2);
        plist->count--;
    }
    if (plist->count == 0) {    //Destroy successfully!
        free(plist);
        return NULL;
    }
    else return plist;  //Unknown error happened!
}

//Push a new Item to the front of the list.
//Return an iterator to the newly added element.
Iterator list_push_front(List *plist, const Item item) {

    if(plist == NULL) { //NULL pointer
        list_error("list_push_front", "NULL pointer plist");
        return NULL;
    }

    Node *pn;
    pn = (Node*)list_malloc(sizeof(Node));      //Node to add.

    pn->item = item;
    pn->next = plist->head;
    pn->prev = plist->this;

    if (plist->tail == plist->this) { //To check if the list is empty.
        plist->tail = pn;
    }
    else {
        plist->head->prev = pn;
    }
    plist->head = pn;
    plist->count++;

    return pn;
}

//Push new Item to the back of the list.
//Return iterator to it.
Iterator list_push_back(List *plist, const Item item) {

    if(plist == NULL) { //NULL pointer
        list_error("list_push_back", "NULL pointer plist");
        return NULL;
    }

    Node *pn;
    pn = (Node*)list_malloc(sizeof(Node));

    pn->item = item;
    pn->next = plist->this;
    pn->prev = plist->tail;

    if (plist->head == plist->this) { //To check if list is empty
        plist->head = pn;
    }
    else {
        plist->tail->next = pn;
    }
    plist->tail = pn;
    plist->count++;

    return pn;
}

//Pop an element from the front of a list.
//That is, return it, and delete it in list.
Item list_pop_front(List *plist) {
    Item item_t;
    memset(&item_t, 0, sizeof(item_t));

    if(plist == NULL) { //NULL pointer.
        list_error("list_pop_front", "NULL pointer plist");
        return item_t;
    }

    if (plist->head == plist->this) {   //Empty list.
        list_error("list_pop_front", "Empty list! Meaningless return value!");
        return item_t;
    }
    else {
        item_t = plist->head->item;
        Iterator pnext = plist->head->next;

        if (pnext == plist->this) { //List only has one element, need to connect the tail to this.
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

//Pop an element from the back of a list.
Item list_pop_back(List *plist) {
    Item item_t;
    memset(&item_t, 0, sizeof(item_t));

    if(plist == NULL) { //NULL pointer.
        list_error("list_pop_front", "NULL pointer plist");
        return item_t;
    }

    if (plist->head == plist->this) {   //Empty
        list_error("list_pop_back", "Empty list! Meaningless return value!");
        return item_t;
    }
    else {
        item_t = plist->tail->item;
        Iterator pprev = plist->tail->prev;

        if (pprev == plist->this) {     //Only element. Need to connect head to this.
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

//Swap two list
//Return 1 means worked successfully, 0 otherwise.
int list_swap(List *plist1, List *plist2) {
    if(plist1 == NULL) {
        list_error("list_swap", "NULL ptr plist1");
        return 0;
    }
    else if(plist2 == NULL) {
        list_error("list_swap", "NULL ptr plist2");
        return 0;
    }

    List list_t;    //tmp list
    list_t = *plist1;

    *plist1 = *plist2;      // Let list1=list2
    plist1->this = (Iterator)plist1;    //Correct this ptr inside.
    if (plist2->head == plist2->this) {     //list2 is empty.
        plist1->head = plist1->tail = plist1->this;
    }
    else {      //Not empty.
        plist1->head->prev = plist1->tail->next = plist1->this;
    }

    *plist2 = list_t;       // Let list2 = list_t
    plist2->this = (Iterator)plist2;    //Correct this ptr inside.
    if (list_t.head == list_t.this) {       //list1 is empty.
        plist2->head = plist2->tail = plist2->this;
    }
    else {      //Not empty.
        plist2->head->prev = plist2->tail->next = plist2->this;
    }

    return 1;
}

//Insert Item into Dst in the position before pos.
//Return iterator to new element.
Iterator list_insert(List *Dst, Iterator pos, const Item item) {
    if (pos == NULL) {  //NULL ptr.
        return NULL;
    }
    /*else if(pos == Dst->head) {
    	return list_push_front(Dst, item);
    }
    else if(pos == Dst->this) {
    	return list_push_back(Dst, item);
    }*/
    else {
        Iterator prev = (pos == Dst->this) ? Dst->tail : pos->prev;     //prev is the previous one before pos.If pos == this, prev should be tail.
        Iterator add = (Iterator)list_malloc(sizeof(Node));     //add points to the new Node.
        Iterator Real_End = Dst->this;      //Real_End is this.

        add->item = item;      //Set the add.
        add->prev = prev;
        add->next = pos;

        if (prev != Real_End) {
            prev->next = add;
        }
        else {      //Insert before the first one.Correct the head.
            Dst->head = add;
        }

        if (pos != Real_End) {
            pos->prev = add;
        }
        else {      //Insert behind the last one.Correct the tail.
            Dst->tail = add;
        }

        Dst->count++;   //Never forget to increase count! :D

        return add;
    }
}

//One of the overloading function in STL.
//Insert the range [first , last ) into Dst in the position before pos.
//Return iterator to the LAST element successfully added to Dst.
//Remember the range "from first to last", always means [first , last ) .
Iterator list_insert2(List *Dst, Iterator pos, Iterator first, Iterator last) {
    if (pos == NULL || first == NULL || last == NULL || first == ((List*)first)->this) { //To guarantee the validity of "first"
        return NULL;
    }
    else {
        Iterator pt;
        Iterator prev = (pos == Dst->this) ? Dst->tail : pos->prev;     //The original previous iterator before pos.
        Iterator add;   //iter to add.
        Iterator Real_End = Dst->this;

        for (pt = first; pt != last; pt = pt->next) {   //Insert one by one.
            add = (Node*)list_malloc(sizeof(Node));

            add->item = pt->item;   //Steps the same to which in "list_insert"
            add->prev = prev;
            add->next = pos;

            if (prev != Real_End) {
                prev->next = add;
            }
            else {
                Dst->head = add;
            }

            if (pos != Real_End) {
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

//Assign Src to Dst.
//Contents in Dst will be deleted.
//Return iterator to the LAST element added to Dst.
Iterator list_assign(List *Dst, const List *Src) {
    if (Src->head == Src->this) {   //Src is empty.
        list_clear(Dst);    //Just clear Dst.
        return Dst->this;
    }
    else {
        Iterator pt;    //To iterate
        Iterator first = Src->head;     //First iter in Src
        Iterator add;       //To content element copied from Src
        Iterator last = Src->this;      //Lasr iter in Src
        Iterator End1 = Dst->this, prev = End1;     //End1 is the last iter in Dst. Prev is the previous iter of pt in the loop.

        list_clear(Dst);

        for (pt = first; pt != last; pt = pt->next) {
            add = (Node*)list_malloc(sizeof(Node));

            add->item = pt->item;
            add->prev = prev;
            add->next = End1;

            if (prev != End1) {
                prev->next = add;
            }
            else {      //Add to an empty list.
                Dst->head = add;
            }

            Dst->tail = add;
            Dst->count++;


            prev = add;
        }

        return Dst->tail;
    }
}

//Erase the element in the position of pos
//Return the next iter of pos.
Iterator list_erase(List * Dst, Iterator pos) {
    if (pos == NULL || pos == ((List*)pos)->this) { //NULL ptr or empty list.
        return pos;
    }
    else {
        Iterator prev = pos->prev;  //Previous to pos
        Iterator next = pos->next;  //Next to pos

        if (prev != Dst->this) {
            prev->next = next;
        }
        else {  //Erase the first one.
            Dst->head = next;
        }

        if (next != Dst->this) {
            next->prev = prev;
        }
        else {    //Erase the last one.
            Dst->tail = prev;
        }

        Dst->count--;
        free(pos);

        return next;
    }
}

//Delete the range [ first , last ) in Dst.
//Return the iter next to the LAST one deleted.(Generally, is last).
Iterator list_erase2(List * Dst, Iterator first, Iterator last) {
    if (first == NULL || last == NULL || first == ((List*)first)->this) {   //Invalid range.
        return NULL;
    }
    else {
        Iterator pt1, pt2, prev = first->prev;  //In the loop, pt2 is next to pt1. prev is previous to first.

        if (prev != Dst->this) {
            prev->next = last;
        }
        else {  //Delete the first one.
            Dst->head = last;
        }

        if (last != Dst->this) {
            last->prev = prev;
        }
        else {  //Delete the last one.
            Dst->tail = prev;
        }

        for (pt1 = first; pt1 != last; pt1 = pt2) {
            pt2 = pt1->next;    //Store the next one of pt1 before its being deleted.
            free(pt1);
            Dst->count--;
        }

        return last;
    }
}

//Delete all contents in list.
//Return 1 if succeeds.
int list_clear(List * plist) {
    if(!plist) {    //NULL ptr
        list_error("list_clear", "NULL ptr");
        return 0;
    }

    Iterator pt1, pt2;      //Steps are similar to above.
    Iterator Real_End = plist->this;

    for (pt1 = plist->head; pt1 != Real_End; pt1 = pt2) {
        pt2 = pt1->next;
        free(pt1);
    }

    plist->count = 0;
    plist->head = Real_End;
    plist->tail = Real_End;

    return 1;
}

bool list_equal(const List * plist1, const List * plist2) {
    if (plist1 == plist2) {
        return 1;
    }
    else if (plist1->comp != plist2->comp) {
        if (plist1->comp == NULL) {
            list_error("list_equal", "Former List without specialized Comparer!");
            return 0;
        }
        else if (plist2->comp == NULL) {
            list_error("list_equal", "Latter List without specialized Comparer!");
            return 0;
        }
        else {
            list_error("list_equal", "Conflict Comparer!");
            return 0;
        }
    }
    else {
        Iterator pt1, pt2;
        Iterator end1 = plist1->this, end2 = plist2->this;

        if (plist1->comp && plist2->comp) {
            Comparer cmp = plist1->comp;

            for (pt1 = plist1->head, pt2 = plist2->head;
                    pt1 != end1 && pt2 != end2;
                    pt1 = pt1->next, pt2 = pt2->next)
            {
                if ((*cmp)(&(pt1->item), &(pt2->item)) != 0) {
                    return 0;
                }
            }
            if (pt1 == end1 && pt2 == end2)return 1;
            else return 0;
        }
        else {
            for (pt1 = plist1->head, pt2 = plist2->head;
                    pt1 != end1 && pt2 != end2;
                    pt1 = pt1->next, pt2 = pt2->next)
            {
                if (memcmp(&(pt1->item), &(pt2->item), sizeof(Item)) != 0) {
                    return 0;
                }
            }
            if (pt1 == end1 && pt2 == end2)return 1;
            else return 0;
        }
    }
    return 0;
}

int list_for_each(Iterator first, Iterator last, void(*foo)(Item*)) {
    if (first == NULL || last == NULL || foo == NULL || first == ((List*)first)->this) {
        list_error("list_for_each", "invalid iterator first OR last OR invalid function OR empty list!");
        return 0;
    }
    else {
        Iterator pt = first;
        int count = 0;

        for (; pt != last; pt = pt->next) {
            (*foo)(&(pt->item));
            count++;
        }

        return count;
    }
}

int list_for_each_reverse(Iterator first, Iterator last, void(*foo)(Item*)) {
    if (first == NULL || last == NULL || foo == NULL || first == ((List*)first)->this) {
        list_error("list_for_each_reverse", "Invalid Iterator first OR last OR Invalid function OR Empty list!");
        return 0;
    }
    else {
        Iterator pt = (last == ((List*)last)->this) ? ((List*)last)->tail : last->prev;
        Iterator off_first = first->prev;
        int count = 0;

        for (; pt != off_first; pt = pt->prev) {
            (*foo)(&(pt->item));
            count++;
        }

        return count;
    }
}

Iterator list_find(const List * plist, const Item * pitem) {
    Iterator pt = plist->head;
    Comparer cmp = plist->comp;

    if (cmp == NULL) {
        for (; pt != plist->this; pt = pt->next) {
            if (memcmp(&(pt->item), pitem, sizeof(Item)) == 0)return pt;
        }
    }
    else {
        for (; pt != plist->this; pt = pt->next) {
            if (((*cmp)(&(pt->item), pitem)) == 0)return pt;
        }
    }
    return NULL;
}

Iterator list_find_if(const List *plist, Pred pred) {
    Iterator pt = plist->head;
    if (pred == NULL) {
        list_error("list_find_if", "NULL predicate");
        return NULL;
    }
    else {
        for (; pt != plist->this; pt = pt->next) {
            if ((*pred)(&(pt->item)))return pt;
        }
    }
    return NULL;
}

Item *list_get_item_ptr(Iterator pt) {
    if (pt == NULL || pt == ((List*)pt)->this) {
        list_error("list_get_item_ptr", "Invalid pointer!");
        return NULL;
    }
    else return &(pt->item);
}

Item list_get_item(Iterator pt) {
    if (pt == NULL || pt == ((List*)pt)->this) {
        Item item_t;
        //memset(&item_t, 0, sizeof(item_t));
        list_error("list_get_item", "Empty list! Meaningless return value!");
        return item_t;
    }
    else return pt->item;
}

Iterator list_resize(List* plist, unsigned int n) {
    if (plist == NULL) {
        list_error("list_resize", "plist is NULL!");
        return NULL;
    }
    else {
        unsigned int i = plist->count;
        if (i == n || (int)n < 0) {
            list_error("list_resize", "i==n OR n<0");
            return plist->tail;
        }
        else if (n == 0) {
            list_error("list_resize", "n==0!");
            list_clear(plist);
        }
        else if (i > n) {
            while (i-- > n) {
                //list_pop_back(plist);
                Iterator pprev = plist->tail->prev;

                if (pprev == plist->this) {
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
        else if (i < n) {
            Item item;
            memset(&item, 0, sizeof(item));
            while (i++ < n) {
                //list_push_back(plist,item);
                Node *pn;
                pn = (Node*)list_malloc(sizeof(Node));

                pn->item = item;
                pn->next = plist->this;
                pn->prev = plist->tail;

                if (plist->head == plist->this) { //To judge if list is empty
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

int list_for_all(const List *plist, void(*foo)(Item*)) {
    if (plist->head == plist->this) {
        list_error("list_for_all", "(Empty List)");
        return 0;
    }
    else {
        Iterator Real_End = plist->this;
        Iterator pt = plist->head;
        int count = 0;
        for (; pt != NULL && pt != Real_End; pt = pt->next) {
            (*foo)(&(pt->item));
            count++;
        }
        return count;
    }
}

int list_for_all_reverse(const List *plist, void(*foo)(Item*)) {
    if (plist->head == plist->this) {
        list_error("list_for_all_reverse", "(Empty List)");
        return 0;
    }
    else {
        Iterator Real_End = plist->this;
        Iterator pt = plist->tail;
        int count = 0;
        for (; pt != NULL && pt != Real_End; pt = pt->prev) {
            (*foo)(&(pt->item));
            count++;
        }
        return 0;
    }
}

Iterator list_splice(List *Dst, Iterator pos, List *Src) {
    if (Src->head == Src->this) {
        list_error("list_splice", "List Src is Empty!");
        return pos;
    }
    else {
        Iterator End1 = Dst->this;
        Iterator pprev = (pos == End1) ? Dst->tail : pos->prev;

        if (pprev == End1) {
            Dst->head = Src->head;
        }
        else {
            pprev->next = Src->head;
        }
        Src->head->prev = pprev;

        if (pos == End1) {
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
    if (x == NULL || x == Src->this) {
        list_error("list_splice1", "Iterator x is not valid!");
        return pos;
    }
    else if (pos == NULL) {
        list_error("list_splice1", "Iterator pos is not valid!");
        return pos;
    }
    else if (pos == x || pos == x->next) {
        list_error("list_splice1", "Iterator x equals to pos!\nOR is already before pos!");
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

        if (pos == End1) {
            Dst->tail = x;
        }
        else {
            pos->prev = x;
        }

        if (prev1 == End1) {
            Dst->head = x;
        }
        else {
            prev1->next = x;
        }

        ++Dst->count;
        --Src->count;

        if (prev2 == End2) {
            Src->head = next2;
        }
        else {
            prev2->next = next2;
        }

        if (next2 == End2) {
            Src->tail = prev2;
        }
        else {
            next2->prev = prev2;
        }

        return x;
    }
}

Iterator list_splice2(List *Dst, Iterator pos, List *Src, Iterator first, Iterator last) {
    if (pos == NULL) {
        list_error("list_splice2", "NULL Iterator pos!");
        return NULL;
    }
    else if (first == NULL || first == Src->this) {
        list_error("list_splice2", "Invalid Iterator first!");
        return NULL;
    }
    else if (last == NULL || first == last) {
        list_error("list_splice2", "Invalid Iterator last!");
        return NULL;
    }
    else if (pos == last) {
        return pos;
    }
    else {
        int count = 0;
        Iterator pt = first;
        Iterator End1 = Dst->this;
        Iterator End2 = Src->this;

        for (; pt && pt != last; pt = pt->next) {
            if (pt == pos)break;
            count++;
        }
        if (pt == pos) {
            list_error("list_splice2", "Iterator pos included in the range [first, last)!");
            return NULL;
        }
        else if (pt == NULL) {
            list_error("list_splice2", "Bad List (Pointer to NULL)!");
            return NULL;
        }

        Iterator prev1 = (pos == End1) ? Dst->tail : pos->prev;
        Iterator prev2 = first->prev;
        Iterator back2 = (last == End2) ? Src->tail : last->prev;

        first->prev = prev1;
        back2->next = pos;

        if (prev1 == End1) {
            Dst->head = first;
        }
        else {
            prev1->next = first;
        }

        if (pos == End1) {
            Dst->tail = back2;
        }
        else {
            pos->prev = back2;
        }

        Dst->count += count;
        Src->count -= count;

        if (prev2 == End2) {
            Src->head = last;
        }
        else {
            prev2->next = last;
        }

        if (last == End2) {
            Src->tail = prev2;
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
    if (cmp) {
        while (pt && pt != end) {
            if ((*cmp)(&(pt->item), &item) == 0) {
                //pt = list_erase(plist, pt);
                prev = pt->prev;
                next = pt->next;

                if (prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if (next != end) {
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
        while (pt && pt != end) {
            if (memcmp(&(pt->item), &item, sizeof(Item)) == 0) {
                //pt = list_erase(plist, pt);
                prev = pt->prev;
                next = pt->next;

                if (prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if (next != end) {
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
    if (cmp) {
        while (p2 && p2 != end) {
            if (p1 != end && (*cmp)(&(p1->item), &(p2->item)) == 0) {
                //p2 = list_erase(plist, p2);
                prev = p2->prev;
                next = p2->next;

                if (prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if (next != end) {
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
        while (p2 && p2 != end) {
            if (p1 != end && memcmp(&(p1->item), &(p2->item), sizeof(Item)) == 0) {
                //p2 = list_erase(plist, p2);
                prev = p2->prev;
                next = p2->next;

                if (prev != end) {
                    prev->next = next;
                }
                else {
                    plist->head = next;
                }

                if (next != end) {
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
    if (Dst == Src) {
        list_error("list_merge", "The same List!");
        return NULL;
    }
    else if (Dst->comp != Src->comp) {
        if (Dst->comp == NULL) {
            list_error("list_merge", "Former List without specialized Comparer!");
            return NULL;
        }
        else if (Src->comp == NULL) {
            list_error("list_merge", "Latter List without specialized Comparer!");
            return NULL;
        }
        else {
            list_error("list_merge", "Conflict Comparer!");
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
        while (p1 && p2 && p1 != end1 && p2 != end2) {
            if ((*cmp)(&(p1->item), &(p2->item)) > 0) {
                next = p2->next;
                list_splice1(Dst, p1, Src, p2);
                p2 = next;
            }
            else {
                p1 = p1->next;
            }
        }
        if (p1 == end1 && p2 != end2) {
            list_splice(Dst, p1, Src);
        }
        return p1;
    }
}

bool list_less(const List *plist1, const List *plist2) {
    if (plist1 == plist2) {
        return 0;
    }
    else if (plist1->comp != plist2->comp) {
        if (plist1->comp == NULL) {
            list_error("list_less", "Former List without specialized Comparer!");
            return 0;
        }
        else if (plist2->comp == NULL) {
            list_error("list_less", "Latter List without specialized Comparer!");
            return 0;
        }
        else {
            list_error("list_less", "Conflict Comparer!");
            return 0;
        }
    }
    else {
        Iterator pt1, pt2;
        Iterator end1 = plist1->this, end2 = plist2->this;

        if (plist1->comp && plist2->comp) {
            Comparer cmp = plist1->comp;

            for (pt1 = plist1->head, pt2 = plist2->head;
                    pt1 != end1 && pt2 != end2;
                    pt1 = pt1->next, pt2 = pt2->next)
            {
                if ((*cmp)(&(pt1->item), &(pt2->item)) < 0) {
                    return 1;
                }
            }
            if (pt1 == end1 && pt2 == end2)return 0;
            else if (pt1 == end1)return 1;
            else return 0;
        }
        else {
            for (pt1 = plist1->head, pt2 = plist2->head;
                    pt1 != end1 && pt2 != end2;
                    pt1 = pt1->next, pt2 = pt2->next)
            {
                if (memcmp(&(pt1->item), &(pt2->item), sizeof(Item)) < 0) {
                    return 1;
                }
            }
            if (pt1 == end1 && pt2 == end2)return 0;
            else if (pt1 == end1)return 1;
            else return 0;
        }
    }
    return 0;
}


#ifdef SORT_MODE_QUICK
//Using quick sort
static void list_quick_sort(List *plist, Iterator left, Iterator right) {
    if (left == right || right->next == left || ((List*)right)->head == left) {
        return;
    }
    Comparer cmp = plist->comp ? plist->comp : list_default_comparer;
    Iterator p1 = left, p2 = left->next, i = right, i_t;
    left = p1->prev, right = i->next;
    const Item *k = &(p1->item);
    int c;

    while (i != p2) {
        c = (*cmp)(k, &(i->item));
        if (c > 0) {
            i_t = i->prev;
            list_splice1(plist, p1, plist, i);
            i = i_t;
        }
        else if (c == 0) {
            i_t = i->prev;
            list_splice1(plist, p2, plist, i);
            i = i_t;
        }
        else {
            i = i->prev;
        }

    }

    c = (*cmp)(k, &(p2->item));
    if (c > 0) {
        i_t = p2->next;
        list_splice1(plist, p1, plist, p2);
        p2 = i_t;
    }
    else if (c == 0) {
        p2 = p2->next;
    }

    left = ((List*)left)->this == left ? plist->head : left->next;
    right = ((List*)right)->this == right ? plist->tail : right->prev;
    if (left != p1) {
        list_quick_sort(plist, left, p1->prev);
    }
    if (p2 != right) {
        list_quick_sort(plist, p2, right);
    }

}


Iterator list_sort(List *plist) {
    if (plist == NULL || plist->this != (Iterator)plist) {
        list_error("list_sort", "Uninitialized List!");
        return NULL;
    }
    else if (plist->count == 0) {
        list_error("list_sort", "Empty List!");
        return NULL;
    }
    else {
        list_quick_sort(plist, plist->head, plist->tail);
        return plist->head;
    }
}

#else
//Using the merge similar to "tstl2cl"
#ifdef DEBUG
static void view(List *p) {
    Iterator pt = p->head;
    int cnt = 0;
    puts("------------------------------");
    printf("List %p:  head: %p  tail: %p  this: %p  count:%d\n", p, p->head, p->tail, p->this, p->count);
    for (; pt != p->this; pt = pt->next) {
        printf("Node %d:\n", cnt++);
        printf("\tprev:%p  this:%p  next:%p\n", pt->prev, pt, pt->next);
        printf("\t%.2f  %.2f\n", pt->item.coe, pt->item.pow);
    }
    puts("------------------------------\n\n");
}
#endif // DEBUG
Iterator list_sort(List *plist) {
    if (plist == NULL || plist->this != (Iterator)plist) {
        list_error("list_sort", "Uninitialized List!");
        return NULL;
    }
    else if (plist->count == 0) {
        list_error("list_sort", "Empty List!");
        return NULL;
    }
    else {
        List cry;
        List cnt[64];
        int fill = 0, i = 0;
        list_create(&cry, plist->comp);
        for (fill = 0; fill < 64; ++fill) {
            list_create(&cnt[fill], plist->comp);
        }
        fill = 0;
        while (plist->count) {
            list_splice1(&cry, cry.head, plist, plist->head);


            i = 0;
            while (i < fill && cnt[i].count) {
                list_merge(&cry, &cnt[i++]);
            }
            list_swap(&cry, &cnt[i]);

            if (i == fill) {
                ++fill;
            }
        }

        for (i = 1; i < fill; ++i) {
            list_merge(&cnt[i], &cnt[i - 1]);
        }
        list_swap(plist, &cnt[fill - 1]);

        return plist->head;
    }
}

#endif // SORT_MODE_QUICK

//Return if list is sorted in ascending order.
bool list_is_sorted(List *plist) {
    if(!plist) {    //NULL ptr
        list_error("list_is_sorted", "NULL ptr");
        return false;
    }
    Iterator end = plist->this;     //end is this
    Iterator p1 = end;      // In the loop, p1 is previous to p2.
    Iterator p2 = plist->head;
    Comparer cmp = plist->comp;     //Get the Comparer

    if (plist->count < 2)return true;       //No need to check "order" in a list with 1 or no elements.

    if (cmp) {      //Comparer defined.
        while (p2 && p2 != end) {
            if (p1 != end && (*cmp)(&(p1->item), &(p2->item)) > 0) {    //Exist a descending partition.
                return false;
            }

            p1 = p2;
            p2 = p2->next;
        }
        return true;    //Is sorted.
    }
    else {      //Using memcmp, which is DANGEROUS!
        while (p2 && p2 != end) {
            if (p1 != end && memcmp(&(p1->item), &(p2->item), sizeof(Item)) > 0) {
                return false;
            }

            p1 = p2;
            p2 = p2->next;
        }
        return true;
    }
}
