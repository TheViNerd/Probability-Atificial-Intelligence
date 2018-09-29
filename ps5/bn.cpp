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
		reason = string("adding variable ")+reason+" to BN that already includes that variable";
		throw new badfactorexception(reason);
	}
	factors.insert(i,make_pair(v,f));
}

bn::ctpotentials bn::calibrate(const bn::cliquetree &ct,
			const map<int,int> &alpha) const {
}

