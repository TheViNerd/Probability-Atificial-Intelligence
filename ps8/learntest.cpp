#include "bn.h"
#include <iostream>

using namespace std;

// no error checking! :(
bn::dataset loaddataset(istream &is) {
	bn::dataset D;

	int nv;
	is >> nv;
	vector<int> varid;
	for(int i=0;i<nv;i++) {
		int v; is >> v;
		varid.push_back(v);
	}
	int M;
	is >> M;
	for(int i=0;i<M;i++) {
		factor::assign xi;
		for(vector<int>::iterator v=varid.begin();v!=varid.end();++v) {
			int val; is >> val;
			if (val>=0) xi[*v] = val; // for this PS, val will always be >=0
		}
		D.push_back(xi);
	}
	return D;
}

static inline bool cmpfirst(pair<int,int> p1, pair<int,int> p2) {
     return p1.first < p2.first;
}

// scope union:
factor::scope operator+(const factor::scope &s1, const factor::scope &s2) {
     factor::scope ret;
     set_union(s1.begin(),s1.end(),s2.begin(),s2.end(),
               inserter(ret,ret.begin()),cmpfirst);
     return ret;
}


// simple network: X1 -> X4 <- X10
//                       |
//                       V
//                       X8

bn makeex1() {
	bn n;

	factor::scope var1; var1[1] = 3;
	factor::scope var4; var4[4] = 4;
	factor::scope var10; var10[10] = 2;
	factor::scope var8; var8[8] = 2;

	factor p1(var1),p4(var1+var4+var10),p10(var10),p8(var4+var8);
	// don't bother to set factor CPDs... we'll be learning them

	n.addvar(1,p1);
	n.addvar(4,p4);
	n.addvar(8,p8);
	n.addvar(10,p10);

	return n;
}
	

int main(int argc, char **argv) {
	bn n = makeex1();
	bn::dataset D = loaddataset(cin);
	n.learnparams(D);
	n.print(cout);
}
