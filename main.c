#include<stdio.h>
#include<math.h>
#include"list.h"
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
int main(void) {
    List l1, l2;
    int n;

    create(l1, l_cmp);
    create(l2, (Comparer)NULL);

    scanf("%d", &n);
    resize(l1, n);
    for_all(l1, l_scan);

    //list_for_all(&l1, l_print);
    sort(l1);

    for_all(l1, l_print);
    putchar('\n');
    for_each_r( rend(l1), rbegin(l1), l_print);

    printf("%u  %u\n", size(l1), size(l2));

    assign(l2,l1);
    unique(l2);
    for_all_r(l2,l_print);

    return 0;
}
