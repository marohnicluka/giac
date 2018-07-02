#include "nauty/nausparse.h"
#include "nautywrapper.h"

DYNALLSTAT(int,nauty_lab1,nauty_lab1_sz);
DYNALLSTAT(int,nauty_lab2,nauty_lab2_sz);
DYNALLSTAT(int,nauty_ptn,nauty_ptn_sz);
DYNALLSTAT(int,nauty_orbits,nauty_orbits_sz);
static DEFAULTOPTIONS_SPARSEGRAPH(nauty_options);
SG_DECL(nauty_sg1);
SG_DECL(nauty_sg2);
SG_DECL(nauty_cg1);
SG_DECL(nauty_cg2);

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

nautywrapper::nautywrapper(bool isdirected,int n,int *adj1,int *adj2) {
    nv=n;
    is_single_graph=adj2==0;
    m=SETWORDSNEEDED(n);
    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
    nauty_options.digraph=isdirected?TRUE:FALSE;
    nauty_options.linelength=RAND_MAX;
    DYNALLOC1(int,nauty_lab1,nauty_lab1_sz,n,"malloc");
    DYNALLOC1(int,nauty_lab2,nauty_lab2_sz,n,"malloc");
    DYNALLOC1(int,nauty_ptn,nauty_ptn_sz,n,"malloc");
    DYNALLOC1(int,nauty_orbits,nauty_orbits_sz,n,"malloc");
    int *degrees=new int[n];
    int ne1=count_edges(n,adj1,degrees);
    SG_ALLOC(nauty_sg1,n,ne1,"malloc");
    nauty_sg1.nv=n;
    nauty_sg1.nde=ne1;
    memcpy(nauty_sg1.d,degrees,n*sizeof(int));
    add_edges(&nauty_sg1,n,adj1);
    if (adj2!=0) {
        int ne2=count_edges(n,adj2,degrees);
        SG_ALLOC(nauty_sg2,n,ne2,"malloc");
        nauty_sg2.nv=n;
        nauty_sg2.nde=ne2;
        memcpy(nauty_sg2.d,degrees,n*sizeof(int));
        add_edges(&nauty_sg2,n,adj2);
    }
    delete[] degrees;
    nauty_options.outfile=tmpfile();
}

nautywrapper::~nautywrapper() {
    SG_FREE(nauty_sg1);
    SG_FREE(nauty_cg1);
    if (!is_single_graph) {
        SG_FREE(nauty_sg2);
        SG_FREE(nauty_cg2);
    }
    DYNFREE(nauty_lab1,nauty_lab1_sz);
    DYNFREE(nauty_lab2,nauty_lab2_sz);
    DYNFREE(nauty_ptn,nauty_ptn_sz);
    DYNFREE(nauty_orbits,nauty_orbits_sz);
    if (nauty_options.outfile!=NULL)
        fclose(nauty_options.outfile);
}

bool nautywrapper::is_isomorphic() const {
    statsblk nauty_stats;
    nauty_options.getcanon=TRUE;
    nauty_options.writeautoms=FALSE;
    sparsenauty(&nauty_sg1,nauty_lab1,nauty_ptn,nauty_orbits,&nauty_options,&nauty_stats,&nauty_cg1);
    sparsenauty(&nauty_sg2,nauty_lab2,nauty_ptn,nauty_orbits,&nauty_options,&nauty_stats,&nauty_cg2);
    return (bool)aresame_sg(&nauty_cg1,&nauty_cg2);
}

int *nautywrapper::labeling(int which) const {
    if (which==1)
        return nauty_lab1;
    if (which==2)
        return nauty_lab2;
    return 0;
}

char *nautywrapper::aut_generators() const {
    statsblk nauty_stats;
    nauty_options.getcanon=FALSE;
    nauty_options.writeautoms=TRUE;
    sparsenauty(&nauty_sg1,nauty_lab1,nauty_ptn,nauty_orbits,&nauty_options,&nauty_stats,NULL);
    FILE *f=nauty_options.outfile;
    if (f!=NULL) {
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
        return res;
    }
    return NULL;
}
