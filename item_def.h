#ifndef ITEM_DEF_H_INCLUDED

#define ITEM_DEF_H_INCLUDED

#define DOUBLE_ITEM
//#define SORT_MODE_QUICK 1

#ifdef DOUBLE_ITEM
struct _item {
    double coe, pow;
};
#else
struct _item {
    int coe, pow;
};
#endif // DOUBLE_ITEM

#endif // ITEM_DEF_H_INCLUDED
