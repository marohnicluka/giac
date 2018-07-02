#ifndef NAUTYWRAPPER_H
#define NAUTYWRAPPER_H

class nautywrapper {
    int nv;
    int m;
    bool is_single_graph;
public:
    nautywrapper(bool isdirected,int n,int *adj1,int *adj2=0);
    ~nautywrapper();
    bool is_isomorphic() const;
    int *labeling(int which) const;
};

#endif // NAUTYWRAPPER_H
