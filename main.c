#include<stdio.h>
#include<math.h>
#include"list.h"
void l_print(Item *t){
    if(t)printf("%.2f  %.2f\n",t->coe,t->pow);
    else puts("NULL");
}
void l_scan(Item *t){
    if(t)scanf("%lf%lf",&t->coe,&t->pow);
    else puts("NULL");
}
int main(void){
    List l1,l2;
    Item t;
    list_create(&l1,NULL);
    list_create(&l2,NULL);

    for(int i=0;i<20;++i){
        t.coe=t.pow=i;
        list_push_back(&l1,t);
    }
    t.coe=t.pow=10;
    list_assign(&l2,&l1);
    list_splice2(&l2,list_find(&l2,&(Item){5,5}),&l2,list_find(&l2,&(Item){6,6}),list_find(&l2,&(Item){10,10}));
    puts("Forward:");
    list_for_all(&l2,l_print);
    puts("Backward:");
    list_for_all_backward(&l2,l_print);
    //puts("L2:");
    //list_for_all_backward(&l1,l_print);
    printf("%u  %u\n",l1.count,l2.count);
    return 0;
}
