#include "nauty/nauty.h"
#include "nauty/nautinv.h"
#include "nautywrapper.h"

int nautywrapper_is_isomorphic(int isdir,int n,int *adj1,int *adj2,int *sigma) {
    DYNALLSTAT(int,lab1,lab1_sz);
    DYNALLSTAT(int,lab2,lab2_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(graph,g1,g1_sz);
    DYNALLSTAT(graph,g2,g2_sz);
    DYNALLSTAT(graph,cg1,cg1_sz);
    DYNALLSTAT(graph,cg2,cg2_sz);
    static DEFAULTOPTIONS_GRAPH(opts_undir);
    static DEFAULTOPTIONS_DIGRAPH(opts_dir);
    optionblk *options=isdir!=0?&opts_dir:&opts_undir;
    statsblk stats;
    int m=SETWORDSNEEDED(n);
    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
    DYNALLOC1(int,lab1,lab1_sz,n,"malloc");
    DYNALLOC1(int,lab2,lab2_sz,n,"malloc");
    DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
    DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
    DYNALLOC2(graph,g1,g1_sz,n,m,"malloc");
    EMPTYGRAPH(g1,m,n);
    DYNALLOC2(graph,g2,g2_sz,n,m,"malloc");
    EMPTYGRAPH(g2,m,n);
    DYNALLOC2(graph,cg1,cg1_sz,n,m,"malloc");
    DYNALLOC2(graph,cg2,cg2_sz,n,m,"malloc");
    options->getcanon=TRUE;
    options->writeautoms=FALSE;
    options->outfile=NULL;
    int i=0,j=0,k;
    /* create the first graph */
    while (1) {
        if ((k=adj1[i++])==-1) { if ((++j)==n) break; }
        else if (j<k && isdir==0) { ADDONEEDGE(g1,j,k,m); }
        else if (isdir!=0) { ADDONEARC(g1,j,k,m); }
    }
    /* create the second graph */
    i=j=0;
    while (1) {
        if ((k=adj2[i++])==-1) { if ((++j)==n) break; }
        else if (j<k && isdir==0) { ADDONEEDGE(g2,j,k,m); }
        else if (isdir!=0) { ADDONEARC(g2,j,k,m); }
    }
    /* canonically label both graphs */
    densenauty(g1,lab1,ptn,orbits,options,&stats,m,n,cg1);
    densenauty(g2,lab2,ptn,orbits,options,&stats,m,n,cg2);
    /* return nonzero iff the canonical labelings match */
    size_t cnt=0;
    for (;cnt<m*(size_t)n;++cnt) {
        if (cg1[cnt]!=cg2[cnt]) break;
    }
    boolean isomorphic=cnt==m*(size_t)n?TRUE:FALSE;
    if (isomorphic) {
        for (i=0;i<n;++i) {
            sigma[lab1[i]] = lab2[i];
        }
    }
    DYNFREE(lab1,lab1_sz);
    DYNFREE(lab2,lab1_sz);
    DYNFREE(ptn,ptn_sz);
    DYNFREE(orbits,orbits_sz);
    DYNFREE(g1,g1_sz);
    DYNFREE(g2,g2_sz);
    DYNFREE(cg1,cg1_sz);
    DYNFREE(cg2,cg2_sz);
    return isomorphic;
}

void nautywrapper_aut_generators(int isdir,int n,int *adj,FILE *f) {
    DYNALLSTAT(int,lab,lab_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(graph,g,g_sz);
    static DEFAULTOPTIONS_GRAPH(opts_undir);
    static DEFAULTOPTIONS_GRAPH(opts_dir);
    optionblk *options=isdir!=0?&opts_dir:&opts_undir;
    statsblk stats;
    int m=SETWORDSNEEDED(n);
    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
    DYNALLOC1(int,lab,lab_sz,n,"malloc");
    DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
    DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
    DYNALLOC2(graph,g,g_sz,n,m,"malloc");
    EMPTYGRAPH(g,m,n);
    options->getcanon=FALSE;
    options->writeautoms=TRUE;
    options->linelength=RAND_MAX;
    options->outfile=f;
    int i=0,j=0,k;
    /* create the graph */
    while (1) {
        if ((k=adj[i++])==-1) { if ((++j)==n) break; }
        else if (j<k && isdir==0) { ADDONEEDGE(g,j,k,m); }
        else if (isdir!=0) { ADDONEARC(g,j,k,m); }
    }
    densenauty(g,lab,ptn,orbits,options,&stats,m,n,NULL);
    DYNFREE(lab,lab_sz);
    DYNFREE(ptn,ptn_sz);
    DYNFREE(orbits,orbits_sz);
    DYNFREE(g,g_sz);
}

void nautywrapper_canonical(int isdir,int n,int *adj,int *clab) {
    DYNALLSTAT(int,lab,lab_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(graph,g,g_sz);
    DYNALLSTAT(graph,cg,cg_sz);
    static DEFAULTOPTIONS_GRAPH(opts_undir);
    static DEFAULTOPTIONS_DIGRAPH(opts_dir);
    optionblk *options=isdir!=0?&opts_dir:&opts_undir;
    statsblk stats;
    int m=SETWORDSNEEDED(n);
    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
    DYNALLOC1(int,lab,lab_sz,n,"malloc");
    DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
    DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
    DYNALLOC2(graph,cg,cg_sz,n,m,"malloc");
    DYNALLOC2(graph,g,g_sz,n,m,"malloc");
    EMPTYGRAPH(g,m,n);
    options->getcanon=TRUE;
    options->writeautoms=FALSE;
    options->outfile=NULL;
    int i=0,j=0,k;
    /* create the graph */
    while (1) {
        if ((k=adj[i++])==-1) { if ((++j)==n) break; }
        else if (j<k && isdir==0) { ADDONEEDGE(g,j,k,m); }
        else if (isdir!=0) { ADDONEARC(g,j,k,m); }
    }
    densenauty(g,lab,ptn,orbits,options,&stats,m,n,cg);
    for (i=0;i<n;++i) clab[i]=lab[i];
    DYNFREE(lab,lab_sz);
    DYNFREE(ptn,ptn_sz);
    DYNFREE(orbits,orbits_sz);
    DYNFREE(g,g_sz);
    DYNFREE(cg,cg_sz);
}
