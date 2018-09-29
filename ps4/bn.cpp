#include "bn.h"
#include <exception>
#include <string>
#include <sstream>
using namespace std;

class badfactorexception : public exception {
public:
	badfactorexception(const string &reason) : w(reason) { }
	virtual ~badfactorexception() throw() { }
	virtual const char *what() const throw() { return w.c_str(); }
private:
	string w;
};
		
void bn::addvar(int v, const factor &f) {
	factor::scope s = f.getscope();
	if (s.find(v) == s.end()) {
		stringstream ss; ss << v; string reason; ss >> reason;
		reason = string("adding variable ")+reason+" with factor that does not include it";
		throw new badfactorexception(reason);
	}
	map<int,factor>::iterator i = factors.lower_bound(v);
	if (i!=factors.end() && i->first==v) {
		stringstream ss; ss << v; string reason; ss >> reason;
		reason = string("adding variable ")+reason+" to BN that already includes that variable";
		throw new badfactorexception(reason);
	}
	factors.insert(i,make_pair(v,f));
}

factor bn::varelim(const set<int> &remaining, const vector<int> &order) const {
	// to be implemented
    vector<int> allOrder = order;
    vector<factor> BigFai;
    for (fmap::const_iterator it=factors.begin(); it!=factors.end(); ++it) {
        int key = it->first;
        factor value = it->second;
        if((remaining.find(key)==remaining.end())&&(find(order.begin(),order.end(),key)==order.end())){
            allOrder.push_back(key);
        }
        BigFai.push_back(value);
    }
    for(int k=0;k<allOrder.size();++k){
        int varIndex = allOrder[k];
        //elimate this var in order
        vector<factor> Fai1;
        vector<factor> Fai2;
        for (int i=0; i<BigFai.size(); ++i) {
            factor::scope temp = BigFai[i].getscope();
            if (temp.find(varIndex)!=temp.end()) {
                Fai1.push_back(BigFai[i]);
            }
            else{
                Fai2.push_back(BigFai[i]);
            }
        }
        BigFai = Fai2;
        for (int i=1; i<Fai1.size(); ++i) {
            Fai1[0] =  Fai1[0]*Fai1[i];
        }
        factor::scope sumScope;
        sumScope[varIndex]=Fai1[0].getscope()[varIndex];
        factor Tao = Fai1[0].marginalize(sumScope);
        BigFai.push_back(Tao);
    }
    for (int i=1; i<BigFai.size(); ++i) {
        BigFai[0] = BigFai[0]*BigFai[i];
    }
    return BigFai[0];
}
