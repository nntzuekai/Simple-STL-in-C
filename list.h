#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#undef DEBUG
#include<stdbool.h>
#include<stddef.h>
#include "item_def.h"

typedef struct _item Item;

typedef int (*Comparer)(const Item*, const Item*);
typedef void (*Unary_func)(Item *);
typedef void (*Binary_func)(Item *, Item *);
typedef bool (*Pred)(const Item *);
typedef bool (*Binary_pred)(const Item *, const Item *);

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

Iterator list_iter_incrs(Iterator pt);

Iterator list_iter_decrs(Iterator pt);

Iterator list_riter_incrs(Iterator pt);

Iterator list_riter_decrs(Iterator pt);

bool list_empty(const List *plist);

unsigned int list_size(const List *plist);

Iterator list_begin(const List *plist);

Iterator list_end(const List *plist);

Iterator list_rbegin(const List *plist);

Iterator list_rend(const List *plist);

Item list_front(const List *plist);

Item list_back(const List *plist);

Iterator list_assign(List *Dst, const List *Src);

Iterator list_insert(List *Dst, Iterator pos, const Item item);

Iterator list_erase(List *Dst, Iterator pos);

int list_clear(List *plist);

List* list_destroy(List *plist);

Iterator list_push_front(List *Dst, const Item item);

Iterator list_push_back(List *Dst, const Item item);

Item list_pop_front(List *Src);

Item list_pop_back(List *Src);

int list_swap(List *plist1, List *plist2);

Iterator list_insert2(List *Dst, Iterator pos, Iterator first, Iterator last);

Iterator list_erase2(List *Dst, Iterator first, Iterator last);

bool list_equal(const List *plist1, const List *plist2);

bool list_less(const List *plist1, const List *plist2);

int list_for_each(Iterator first, Iterator last, Unary_func pf);

int list_for_each_reverse(Iterator first, Iterator last, Unary_func pf);

int list_for_all(const List *plist, Unary_func pf);

int list_for_all_reverse(const List *plist, Unary_func pf);

Iterator list_find(const List *plist, const Item *pitem);

Iterator list_find_if(const List *plist, Pred pred);

Item *list_get_item_ptr(Iterator pt);

Item list_get_item(Iterator pt);

Iterator list_resize(List *plist, unsigned int n);

Iterator list_splice(List *Dst, Iterator pos, List *Src);

Iterator list_splice1(List *Dst, Iterator pos, List *Src, Iterator x);

Iterator list_splice2(List *Dst, Iterator pos, List *Src, Iterator first, Iterator last);

Iterator list_remove(List *plist, const Item item);

Iterator list_unique(List *plist);

Iterator list_merge(List *Dst, List *Src);

Iterator list_sort(List *plist);

bool list_is_sorted(List *plist);

#define assign(Container_Dest,Container_Src) _Generic((Container_Dest),\
                                                    List: _Generic((Container_Src),\
                                                        List: list_assign)\
                                                    )(&Container_Dest,&Container_Src)

#define back(Container) _Generic((Container),List:list_back)(&Container)

#define begin(Container) _Generic((Container),List:list_begin)(&Container)

#define clear(Container) _Generic((Container),List:list_clear)(&Container)

#define create(Container,Comp) _Generic((Container),List: _Generic((Comp),\
                                                                      Comparer:list_create))(&Container,Comp)

#define destroy(Container) _Generic((Container),List:list_destroy)(&Container)

#define empty(Container) _Generic((Container),List:list_empty)(&Container)
#define is_empty(Container) empty(Container)

#define end(Container) _Generic((Container),List:list_end)(&Container)

#define equal(Container1,Container2)  _Generic((Container1),List: _Generic((Container2),\
                                                                               List: list_equal))(&Container1,&Container2)

#define erase(Container,pos) _Generic((Container),List: _Generic((pos),\
                                                                    Iterator: list_erase))(&Container,pos)

#define erase2(Container,first,last) _Generic((Container),List: _Generic((first),\
                                                                            Iterator: _Generic((last),\
                                                                                               list_erase2)))(&Container,first,last)

#define find(Container,item) _Generic((Container),List: _Generic((item),\
                                                                    Item: list_find))(&Container,&item)

#define find_if(Container,predicate) _Generic((Container),List: _Generic((predicate),\
                                                                            Pred: list_find_if))(&Container,predicate)

#define for_all(Container,unary) _Generic((Container),List: _Generic((unary),\
                                                                                 Unary_func: list_for_all))(&Container,unary)

#define for_all_reverse(Container,unary)  _Generic((Container),List: _Generic((unary),\
                                                                                 Unary_func: list_for_all_reverse))(&Container,unary)
#define for_all_r(Container,unary) for_all_reverse(Container,unary)

#define for_each(first,last,unary) _Generic((first), Iterator: _Generic((last),\
                                                                  Iterator: _Generic((unary),\
                                                                                     Unary_func: list_for_each)))(first,last,unary)

#define for_each_reverse(first,last,unary) _Generic((first), Iterator: _Generic((last),\
                                                                  Iterator: _Generic((unary),\
                                                                                     Unary_func: list_for_each_reverse)))(first,last,unary)
#define for_each_r(first,last,unary) for_each_reverse(first,last,unary)

#define front(Container) _Generic((Container), List: list_front)(&Container)

#define get(pos) _Generic((pos),Iterator:list_get_item)(pos)

#define get_item_ptr(pos) _Generic((pos), Iterator:list_get_item_ptr)(pos)

#define insert(Container,pos,item) _Generic((Container),\
                                            List: _Generic((pos),\
                                                           Iterator: _Generic((item),\
                                                                              Item:list_insert)))(&Container,pos,item)

#define insert2(Container,pos,first,last) _Generic((Container),\
                                                   List: _Generic((pos),\
                                                                  Iterator: _Generic((first),\
                                                                                     Iterator: _Generic((last),\
                                                                                                        Iterator : list_insert2))))(&Container,pos,first,last)

#define is_sorted(Container) _Generic((Container),\
                                      List: list_is_sorted)(&Container)


#define decrs(pos) _Generic((pos),\
                            Iterator: list_iter_decrs)(pos)
#define previous(pos) decrs(pos)


#define incrs(pos) _Generic((pos),\
                            Iterator: list_iter_incrs)(pos)
#define next(pos) incrs(pos)


#define merge(Container_Dest,Container_Src) _Generic((Container1+Container2),\
                                              List: list_merge)(&Container_Dest,&Container_Src)

#define pop_back(Container) _Generic((Container),\
                                     List: list_pop_back)(&Container)

#define pop_front(Container) _Generic((Container),\
                                     List: list_pop_front)(&Container)

#define push_back(Container,item) _Generic((Container),\
                                           List: _Generic((item),\
                                                          Item: list_push_back))(&Container,item);

#define push_front(Container,item) _Generic((Container),\
                                           List: _Generic((item),\
                                                          Item: list_push_front))(&Container,item);

#define rbegin(Container) _Generic((Container),\
                                   List: list_rbegin)(&Container)

#define remove(Container,item) _Generic((Container),\
                                        List: _Generic((item),\
                                                       Item: list_remove))(&Container,item)

#define rend(Container) _Generic((Container),\
                                   List: list_rend)(&Container)

#define resize(Container,n) _Generic((Container),\
                                     List: _Generic((n),\
                                                    unsigned int: list_resize,\
                                                    int: list_resize))(&Container,n)

#define rdecrs(pos) _Generic((pos),\
                             Iterator: list_riter_decrs)(pos)
#define rprevious(pos) rdecrs(pos)

#define rincrs(pos) _Generic((pos),\
                             Iterator: list_riter_incrs)(pos)
#define rnext(pos) rincrs(pos)

#define size(Container) _Generic((Container),\
                                 List: list_size)(&Container)

#define sort(Container) _Generic((Container),\
                                 List: list_sort)(&Container)

#define splice(Container_Dest,pos,Container_Src) _Generic((Container_Dest),\
                                                          List: _Generic((pos),\
                                                                   Iterator: _Generic((Container_Src),\
                                                                                      List:list_splice)))(&Container_Dest,pos,&Container_Src)

#define splice1(Container_Dest,pos,Container_Src,x) _Generic((Container_Dest),\
                                                             List: _Generic((pos),\
                                                                            Iterator: _Generic((Container_Src),\
                                                                                               List: _Generic((x),\
                                                                                                              Iterator: list_splice1))))(&Container_Dest,pos,&Container_Src,x)

#define splice2(Container_Dest,pos,Container_Src,first,last) _Generic((Container_Dest),\
                                                                      List: _Generic((pos),\
                                                                                     Iterator: _Generic((Container_Src),\
                                                                                                        List: _Generic((first),\
                                                                                                                       Iterator: _Generic((last),\
                                                                                                                                          Iterator: list_splice2)))))(&Container_Dest,pos,&Container_Src,first,last)

#define swap(Container1,Container2) _Generic((Container1),\
                                             List: _Generic((Container2),\
                                                            List: list_swap))(&Container1,&Container2)

#define unique(Container) _Generic((Container),\
                                   List: list_unique)(&Container)
#endif // LIST_H_INCLUDED
