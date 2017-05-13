#ifndef OVERLOAD_H_INCLUDED
#define OVERLOAD_H_INCLUDED


#define assign(Container_Dest,Container_Src) _Generic((Container_Dest),\
                                                    List: _Generic((Container_Src),\
                                                        List: list_assign)\
                                                    )(&Container_Dest,&Container_Src)

#define back(Container) _Generic((Container),List:list_back)(&Container)

#define begin(Container) _Generic((Container),List:list_begin)(&Container)

#define clear(Container) _Generic((Container),List:list_clear)(&Container)

#define create(Container,Comp) _Generic((Container),List: _Generic((Comp),\
                                                                      Comparer:list_create,\
                                                                      void*:list_create))(&Container,Comp)

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
                                                                                               Iterator: list_erase2)))(&Container,first,last)

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

#define less(Container1,Container2) _Generic((Container1),\
                                             List: _Generic((Container2),\
                                                            List: list_less))(&Container1,&Container2)

#define merge(Container_Dest,Container_Src) _Generic((Container_Dest),\
                                              List: _Generic((Container_Src),\
                                                             List: list_merge))(&Container_Dest,&Container_Src)

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

#endif // OVERLOAD_H_INCLUDED
