#include "bn.h"
#include <cstdarg>
#include <time.h>

using namespace std;

// handy way to set an assignment quickly
// (variable arguments at the end are all integers: the assignments "in order")
factor::assign A(const factor::scope s, ...) {
     factor::assign ret;
     va_list as;
     va_start(as,s);
     for(factor::scope::const_iterator si=s.begin();si!=s.end();si++) {
          int v = va_arg(as,int);
          ret[si->first] = v;
     }
     va_end(as);
     return ret;
}

// Make the student example from the assignment (not quite the same as
//  the one in the book)
bn makeex() {
	bn n;
	// variable mapping C=0, D=1, I=2, T=3, G=4, S=5, L=6, J=7
	enum {C=0,D=1,I=2,T=3,G=4,S=5,L=6,J=7};

	factor::scope c; c[C] = 2;
	factor::scope d; d[C] = 2; d[D] = 2;
	factor::scope i; i[I] = 2;
	factor::scope t; t[I] = 2; t[T] = 2;
	factor::scope g; g[D] = 2; g[T] = 2; g[G] = 3;
	factor::scope s; s[T] = 2; s[S] = 2;
	factor::scope l; l[G] = 3; l[L] = 2;
	factor::scope j; j[S] = 2; j[L] = 2; j[J] = 2;


	factor pc(c),pd(d),pi(i),pt(t),pg(g),ps(s),pl(l),pj(j);
	pc(A(c,0)) = 0.5; pc(A(c,1)) = 0.5;

	pd(A(d,0,0)) = 0.4; pd(A(d,0,1)) = 0.6;
	pd(A(d,1,0)) = 0.8; pd(A(d,1,1)) = 0.2;

	pi(A(i,0)) = 0.6; pi(A(i,1)) = 0.4;

	pt(A(t,0,0)) = 0.9; pt(A(t,0,1)) = 0.1;
	pt(A(t,1,0)) = 0.4; pt(A(t,1,1)) = 0.6;

	pg(A(g,0,0,0)) = 0.3; pg(A(g,0,0,1)) = 0.4; pg(A(g,0,0,2)) = 0.3;
	pg(A(g,1,0,0)) = .05; pg(A(g,1,0,1)) = .25; pg(A(g,1,0,2)) = 0.7;
	pg(A(g,0,1,0)) = 0.9; pg(A(g,0,1,1)) = .08; pg(A(g,0,1,2)) = .02;
	pg(A(g,1,1,0)) = 0.5; pg(A(g,1,1,1)) = 0.3; pg(A(g,1,1,2)) = 0.2;

	ps(A(s,0,0)) = 0.95; ps(A(s,0,1)) = 0.05;
	ps(A(s,1,0)) = 0.2;  ps(A(s,1,1)) = 0.8;

	pl(A(l,0,0)) = 0.1; pl(A(l,0,1)) = 0.9;
	pl(A(l,1,0)) = 0.4; pl(A(l,1,1)) = 0.6;
	pl(A(l,2,0)) = 0.99; pl(A(l,2,1)) = 0.01;

	pj(A(j,0,0,0)) = 0.9; pj(A(j,0,0,1)) = 0.1;
	pj(A(j,1,0,0)) = 0.4; pj(A(j,1,0,1)) = 0.6;
	pj(A(j,0,1,0)) = 0.3; pj(A(j,0,1,1)) = 0.7;
	pj(A(j,1,1,0)) = 0.1; pj(A(j,1,1,1)) = 0.9;

	n.addvar(C,pc);
	n.addvar(D,pd);
	n.addvar(I,pi);
	n.addvar(T,pt);
	n.addvar(G,pg);
	n.addvar(S,ps);
	n.addvar(L,pl);
	n.addvar(J,pj);

	return n;
}
// Make one bn from other book
bn makeex2() {
    bn n;
    // variable mapping A=0, B=1, C=2, D=3, E=4, F=5, G=6,
    enum {A=0, B=1, C=2, D=3, E=4, F=5, G=6};
    
    factor::scope a; a[A] = 2;
    factor::scope b; b[A] = 2; b[B] = 2;
    factor::scope c; c[B] = 2; c[C] = 2;
    factor::scope d; d[B] = 2; d[D] = 2;
    factor::scope f; f[F] = 2;
    factor::scope e; e[F] = 3; e[E] = 2;e[C] = 2;
    factor::scope g; g[F] = 2; g[G] = 2;g[D] = 2;
    
    
    factor pa(a,1),pb(b,1),pc(c,1),pd(d,1),pe(e,1),pf(f,1),pg(g,1);
    
    
    n.addvar(A,pa);
    n.addvar(B,pb);
    n.addvar(C,pc);
    n.addvar(D,pd);
    n.addvar(E,pe);
    n.addvar(F,pf);
    n.addvar(G,pg);
    
    return n;
}
// Make one bn from other book extend student
bn makeex3() {
    bn n;
    // variable mapping C=0, D=1, I=2, G=3, S=4, L=5, J=6, H=7
    enum {C=0, D=1, I=2, G=3, S=4, L=5, J=6, H=7};
    
    factor::scope c; c[C] = 2;
    factor::scope d; d[C] = 2; d[D] = 2;
    factor::scope i; i[I] = 2;
    factor::scope g; g[D] = 2; g[I] = 2; g[G] = 2;
    factor::scope s; s[I] = 2; s[S] = 2;
    factor::scope l; l[G] = 3; l[L] = 2;
    factor::scope j; j[S] = 2; j[L] = 2; j[J] = 2;
    factor::scope h; h[G] = 2; h[J] = 2; h[H] = 2;
    
    factor pc(c,1),pd(d,1),pi(i,1),pg(g,1),ps(s,1),pl(l,1),pj(j,1),ph(h,1);
    
    
    n.addvar(C,pc);
    n.addvar(D,pd);
    n.addvar(I,pi);
    n.addvar(G,pg);
    n.addvar(S,ps);
    n.addvar(L,pl);
    n.addvar(J,pj);
    n.addvar(H,ph);
    
    return n;
}

void cliquetreePrint(bn::cliquetree& a){
    cout<<"cliques number: "<<a.nodes.size()<<endl;
    cout<<"adj number: "<<a.adj.size()/2<<endl;
    

    cout<<"cliques nodes: "<<endl;
    int maxCliques = 0;
    for (int i=0; i<a.nodes.size(); ++i) {
        cout<<i<<": ";
        for (factor::scope::iterator it=a.nodes[i].begin();it!=a.nodes[i].end();++it) {
            cout<<it->first<<",";
        }
        cout<<endl;
        if (a.nodes[i].size()>maxCliques) {
            maxCliques=(int)a.nodes[i].size();
        }
    }
    cout<<"Max Cliques: "<<maxCliques<<endl;
}

int main(int argc, char **argv) {

	bn n = makeex();
//    static const int arr[] = {7,6,4,5,3,2,1,0};//this one has 5 cliques, and max clique has 4
//    static const int arr[] = {4,7,6,5,3,2,1,0};//this one has 5 cliques, and max clique has 4
//    static const int arr[] = {0,1,2,3,4,5,6,7};//this one has 6 cliques, and max clique has 3
//    vector<int> order (arr, arr + sizeof(arr) / sizeof(arr[0]) );
//    bn::cliquetree re;
//    re = n.cliqueTreeCon(order);
//    cliquetreePrint(re);
    
    bn n2 = makeex2();
//        static const int arr2[] = {0,1,2,3,4,5,6};//6,4,5,3,2,1,0
//    static const int arr2[] = {6,4,5,3,2,1,0};//this one has 5 cliques, and max clique has 3
//    static const int arr2[] = {2,5,0,4,6,1,3};//this one has 3 cliques, and max clique has 5
//    vector<int> order (arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]) );
//    bn::cliquetree re2;
//    re2 = n2.cliqueTreeCon(order);
//    cliquetreePrint(re2);
    bn n3 = makeex3();
//    static const int arr[] = {0,1,2,3,4,5,6,7};
//    static const int arr[] = {0,1,2,7,3,4,5,6};/////
//    static const int arr[] = {3,2,4,5,7,0,1,6};
//    vector<int> order (arr, arr + sizeof(arr) / sizeof(arr[0]) );
//    bn::cliquetree re;
//    re = n3.cliqueTreeCon(order);
//    cliquetreePrint(re);
    
    //===========one method======
//    bn::cliquetree re1;
//    clock_t start, finish;
//    double totaltime;
//    start = clock();
//    for (int i=0; i<10000; ++i) {
//        re1 = n.cliqueTreeCon(order);
//    }
//    finish = clock();
//    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
//    cout << "Time for clique tree constrction using Variable Elimination is: " << totaltime << "seconds！" << endl;
//    cliquetreePrint(re1);

    //===========second method======
//    bn::cliquetree re;
//    start = clock();
//    for (int i=0; i<10000; ++i) {
//        re = n3.cliqueTreeConCG();
//    }
//    finish = clock();
//    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
//    cout << "Time for clique tree constrction using Chordal Graphs is: " << totaltime << "seconds！" << endl;
//    cliquetreePrint(re);
//
//    cout << endl;
    
    //randomly time test=========3===============
    static const int arr[] = {0,1,2,3,4,5,6,7};//this one has 6 cliques, and max clique has 3
    vector<int> order (arr, arr + sizeof(arr) / sizeof(arr[0]) );
    bn::cliquetree re1;
    clock_t start, finish;
    double totaltime;
    for (int k=0; k<50; ++k) {
        start = clock();
        for (int i=0; i<10000; ++i) {
//            re1 = n3.cliqueTreeCon(order);
            re1 = n3.cliqueTreeConCG();
        }
        finish = clock();
        totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
        cout<<totaltime<<endl;
        std::random_shuffle(order.begin(), order.end());
    }
}






