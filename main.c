#include<stdio.h>
#include<math.h>
#include "list.h"
#include "overload.h"
void l_print(Item *t) {
    if(t)printf("%.2f  %.2f\n", t->coe, t->pow);
    else puts("NULL");
}
void l_scan(Item *t) {
    if (t) {
        scanf("%lf", &t->coe);
        t->pow = 1;
    }
    else puts("NULL");
}
int l_cmp(const Item *t1, const Item *t2) {
    if(t1->pow != t2->pow) {
        return t1->pow > t2->pow ? 1 : -1;
    }
    else if(t1->coe != t2->coe) {
        return t1->coe > t2->coe ? 1 : -1;
    }
    else return 0;
}
bool neg(const Item *t){
    return t->coe<0;
}
int main(void) {
    List l1, l2;
    int n;

    create(l1, l_cmp);
    create(l2, l_cmp);

    printf("input n:\n");
    scanf("%d", &n);

    resize(l1, n);
    for_each(begin(l1), end(l1), l_scan);

    printf("Origin:\n");
    for_all(l1, l_print);

    assign(l2, l1);
    sort(l2);
    printf("Sorted:\n");
    for_all(l2, l_print);

    assign(l2, l1);
    Iterator p1 = find(l2,((Item){5,1}));
    Iterator p2 =find(l2,((Item){4,1}));
    splice2(l2,find(l2,((Item){1,1})),l2,p1,p2);
    printf("splice:\n");
    for_all(l2,l_print);

    printf("insert&erase:\n");
    assign(l2,l1);
    insert2(l2,begin(l2),begin(l1),end(l1));
    erase2(l2,begin(l2),previous(end(l2)));
    for_all(l2,l_print);

    assign(l2,l1);
    sort(l2);
    splice1(l2,end(l2),l1,begin(l1));
    size(l1);
    push_back(l2,pop_back(l2));
    push_front(l2,pop_front(l2));
    remove(l2,((Item){0,0}));
    return 0;
}
