#include <bits/stdc++.h>
using namespace std;

typedef struct node *pnode;
typedef pair<pnode,pnode> pnn;

struct node {
    int key, prior, size;
    pnode l,r;

    node() {} 
    node(int key, int prior): key(key), prior(prior), size(1), l(0), r(0) {}
};

struct treap {
    pnode root;

    treap(): root(0) {}

    int size(pnode p) { return p ? p->size : 0; }
    int size() { return size(root); }

    // Propagate delta to children
    void push(pnode p) {
        if (!p) return;
        
        // push delta
    }

    // Update function and size from children's values
    void pull(pnode p) {
        if (!p) return;

        p->size = 1 + size(p->l) + size(p->r);
        // update function and delta
    }  

    void splitKey(pnode t, int key, pnode &l, pnode &r) {
        if (!t) return void(l = r = 0);
        push(t);
        if (key <= t->key) splitKey(t->l, key, l, t->l), r = t;
        else splitKey(t->r, key, t->r, r), l = t;
        pull(t);
    }
    
    pnn splitKey(int key) {
        pnn res;
        splitKey(root,key,res.first, res.second);
        return res;
    }

    void splitSize(pnode t, int sz, pnode &l, pnode &r) {
        if (!t) return void(l = r = 0);
        push(t);
        if (sz <= size(t->l)) splitSize(t->l, sz, l, t->l), r = t; 
        else splitSize(t->r, sz - 1 - size(t->l), t->r, r), l = t; 
        pull(t);
    }

    pnn splitSize(int sz) {
        pnn res;
        splitSize(root, sz, res.first, res.second);
    }

    void merge(pnode &t, pnode l, pnode r) {
        push(l); push(r);
        if (!l || !r) t = l ? l : r;
        else if (l->prior < r->prior) merge(l->r, l->r, r), t = l;
        else merge(r->l, l, r->l), t = r; 
        pull(t);
    }

    pnode merge(pnode l, pnode r) {
        pnode res; merge(res, l,r);
        return res;
    }

    pnode merge(pnode o) { root = merge(root, o); }

    void insertKey(int key) {
        pnode elem = new node(key,rand());
        pnode t1,t2; splitKey(root, key, t1, t2);
        merge(t1,t1,elem);
        merge(root,t1,t2);
    }

    void insertPos(int pos, int key) {
        pnode elem = new node(key, rand());
        pnode t1,t2; splitSize(root, pos, t1, t2);
        merge(t1,t1,elem);
        merge(root,t1,t2);
    }

    void split3Key(pnode t, int key1, int key2, pnode &t1, pnode &t2, pnode &t3) {
        splitKey(t,key1,t1,t2);
        splitKey(t2,key2, t2, t3);
    }

    void split3Pos(pnode t, int pos1, int pos2, pnode &t1, pnode &t2, pnode &t3) {
        splitSize(t,pos1,t1,t2);
        splitSize(t2,pos2-pos1,t2,t3);
    }

    void eraseKey(int key1, int key2) {
        pnode t1,t2,t3; split3Key(root,key1,key2, t1, t2, t3);
        merge(root,t1,t3);
    }

    void erasePos(int pos1, int pos2) {
        pnode t1,t2,t3; split3Pos(root, pos1, pos2, t1, t2, t3);
        merge(root, t1, t2);
    }

    void erasePos(int pos) {
        erasePos(pos,pos+1);
    }

    void eraseKey(pnode &t, int key) {
        if (!t) return;
        if (key == t->key) merge(t,t->l, t->r);
        else if (key < t->key) eraseKey(t->l, key);
        else eraseKey(t->r, key);
    }

    void eraseKey(int key) {
        eraseKey(root, key);
    }

    pnode findKey(pnode t, int key) {
        if (!t) return 0;
        if (key == t->key) return t;
        if (key < t->key) return findKey(t->l, key);
        return findKey(t->r, key);
    }

    pnode findKey(int key) { findKey(root, key); }

    pnode findPos(pnode t, int pos) {
        if (!t) return 0;
        if (pos <= size(t->l)) return findPos(t->l, pos);
        return findPos(t->r, pos - 1 - size(t->l));
    }
};

ostream &operator<<(ostream &out, pnode t) {
    if (!t) return out;
    out << (t->l);
    out << t->key << ' ';
    out << (t->r);
    return out;
}

ostream& operator<<(ostream &out, const treap &t) {
    return (out << t.root);
}

int main(int argc, const char *argv[])
{
    treap t;
    for(;;) {
        int x; cin >> x;
        if (x == 0) break;
        else if (x > 0) {
            t.insertKey(x);
            cerr << t << endl;
        }
        else {
            x = -x;
            //t.eraseKey(x);
            pnn sp = t.splitKey(x);
            cerr << sp.first << endl << sp.second << endl;
            t.root = t.merge(sp.first, sp.second);
            //t.eraseKey(x,x+1);
        }
    }
    
    return 0;
}