#include "nauty/nausparse.h"
#include "nautywrapper.h"

static int count_edges(int n,int *adj,int *degrees) {
    int ne=0,i=0,j=0,k,deg=0;
    while (j<n) {
        k=*(adj+i);
        ++i;
        if (k==-1) {
            *(degrees+j)=deg;
            ++j;
            deg=0;
            continue;
        }
        ++ne;
        ++deg;
    }
    return ne;
}

static void add_edges(sparsegraph *g,int n,int *adj) {
    int i=0,j=0,k;
    *(g->v)=0;
    while (true) {
        k=*(adj+i);
        if (k==-1) {
            if ((++j)==n)
                break;
            *(g->v+j)=i-j+1;
        } else *(g->e+i-j)=k;
        ++i;
    }
}

void init_sparsegraph(sparsegraph *g,int n,int *adj,int *degrees) {
    memcpy(g->d,degrees,n*sizeof(int));
    add_edges(g,n,adj);
}

nautywrapper::nautywrapper(bool isdirected,int n,int *adj_1,int *adj_2) {
    m=SETWORDSNEEDED(n);
    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
    nv=n;
    isdir=isdirected;
    adj1=adj_1;
    adj2=adj_2;
    lab1_sz=lab2_sz=ptn_sz=orbits_sz=0;
    DYNALLOC1(int,lab1,lab1_sz,n,"malloc");
    DYNALLOC1(int,lab2,lab2_sz,n,"malloc");
    DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
    DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
    degrees=new int[n];
}

nautywrapper::~nautywrapper() {
    delete[] degrees;
    DYNFREE(lab1,lab1_sz);
    DYNFREE(lab2,lab1_sz);
    DYNFREE(ptn,ptn_sz);
    DYNFREE(orbits,orbits_sz);
}

bool nautywrapper::is_isomorphic() {
    statsblk stats;
    DEFAULTOPTIONS_SPARSEGRAPH(opt_undir);
    DEFAULTOPTIONS_SPARSEDIGRAPH(opt_dir);
    optionblk *options=isdir?&opt_dir:&opt_undir;
    options->getcanon=TRUE;
    options->writeautoms=FALSE;
    SG_DECL(sg1); SG_DECL(sg2);
    SG_DECL(cg1); SG_DECL(cg2);
    int ne=count_edges(nv,adj1,degrees);
    SG_ALLOC(sg1,nv,ne,"malloc");
    init_sparsegraph(&sg1,nv,adj1,degrees);
    ne=count_edges(nv,adj2,degrees);
    SG_ALLOC(sg2,nv,ne,"malloc");
    init_sparsegraph(&sg2,nv,adj2,degrees);
    sparsenauty(&sg1,lab1,ptn,orbits,options,&stats,&cg1);
    sparsenauty(&sg2,lab2,ptn,orbits,options,&stats,&cg2);
    bool yes=(bool)aresame_sg(&cg1,&cg2);
    SG_FREE(sg1); SG_FREE(cg1);
    SG_FREE(sg2); SG_FREE(cg2);
    return yes;
}

int *nautywrapper::labeling(int which) {
    if (which==1)
        return lab1;
    if (which==2)
        return lab2;
    return 0;
}

char *nautywrapper::aut_generators() {
    statsblk stats;
    DEFAULTOPTIONS_SPARSEGRAPH(opt_undir);
    DEFAULTOPTIONS_SPARSEDIGRAPH(opt_dir);
    optionblk *options=isdir?&opt_dir:&opt_undir;
    options->getcanon=FALSE;
    options->writeautoms=TRUE;
    if ((options->outfile=tmpfile())==NULL)
        return NULL;
    options->linelength=RAND_MAX;
    SG_DECL(sg);
    int ne=count_edges(nv,adj1,degrees);
    SG_ALLOC(sg,nv,ne,"malloc");
    init_sparsegraph(&sg,nv,adj1,degrees);
    sparsenauty(&sg,lab1,ptn,orbits,options,&stats,NULL);
    printf("RES: %d\n",stats.numgenerators);
    SG_FREE(sg);
    FILE *f=options->outfile;
    long sz=ftell(f);
    char *res=new char[sz+1];
    rewind(f);
    int i=0;
    char c;
    do {
        *(res+i)=c=fgetc(f);
        ++i;
    } while (c!=EOF);
    *(res+i-1)='\0';
    fclose(f);
    return res;
}

void nautywrapper::canonical() {
    statsblk stats;
    DEFAULTOPTIONS_SPARSEGRAPH(opt_undir);
    DEFAULTOPTIONS_SPARSEDIGRAPH(opt_dir);
    optionblk *options=isdir?&opt_dir:&opt_undir;
    options->getcanon=TRUE;
    options->writeautoms=FALSE;
    SG_DECL(sg);
    SG_DECL(cg);
    int ne=count_edges(nv,adj1,degrees);
    SG_ALLOC(sg,nv,ne,"malloc");
    init_sparsegraph(&sg,nv,adj1,degrees);
    sparsenauty(&sg,lab1,ptn,orbits,options,&stats,&cg);
    SG_FREE(sg);
    SG_FREE(cg);
}
