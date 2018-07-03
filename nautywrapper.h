#ifndef NAUTYWRAPPER_H
#define NAUTYWRAPPER_H

#include <stdio.h>

class nautywrapper {
    int nv;
    int m;
    int *adj1;
    int *adj2;
    int *lab1;
    size_t lab1_sz;
    int *lab2;
    size_t lab2_sz;
    int *ptn;
    size_t ptn_sz;
    int *orbits;
    size_t orbits_sz;
    int *degrees;
    bool isdir;
public:
    nautywrapper(bool isdirected,int n,int *adj_1,int *adj_2=0);
    ~nautywrapper();
    bool is_isomorphic();
    char *aut_generators();
    int *labeling(int which);
    void canonical();
};

#endif // NAUTYWRAPPER_H
