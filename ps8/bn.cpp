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
		stringstream ss;
		ss << v;
		string reason;
		ss >> reason;
		reason = string("adding variable ")+reason+" with factor that does not include it";
		throw new badfactorexception(reason);
	}
	map<int,factor>::iterator i = factors.lower_bound(v);
	if (i!=factors.end() && i->first==v) {
		stringstream ss;
		ss << v;
		string reason;
		ss >> reason;
		reason = string("adding variable ")+reason+" to BN that alreadys includes that variable";
		throw new badfactorexception(reason);
	}
	factors.insert(i,make_pair(v,f));
}

void bn::print(ostream &os) const {
	for(fmap::const_iterator i=factors.begin();i!=factors.end();++i) {
		os << "variable " << i->first << ": " << endl;
		(i->second).print(os);
		os << endl;
	}
}

void bn::learnparams(const dataset &D) {
	// Your code here
}

