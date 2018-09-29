#ifndef BN_H
#define BN_H

#include "factor.h"
#include<map>
#include<set>
#include<vector>
#include<iostream>

class bn { // class representing a Bayesian network
public:
	// a dataset is a sequence of assignments to variables
	typedef std::vector<factor::assign> dataset;

	// f needs to be a factor that includes v in its scope
	// (and represents the conditional distribution of v)
	// if the parameters are to be learned from data,
	//  the values in the factor do not matter (only the scope)
	void addvar(int v, const factor &f);

	// ML estimate of parameters from complete dataset
	// this *should* adjust the bn
	// structure should *not* change
	// parameters should change (old parameters are to be completely ignored)
	void learnparams(const dataset &D);

	void print(std::ostream &os) const;
	
private:
	// invariant:
	// if factors maps i to f, then f has i in its scope
	//    and if f is marginalized over i, it becomes a factor of all 1s
	//    (that is, it is a conditional distribution over i)
	// Note, the members of the scope of f that are not i are the
	//   parents of i in the BN
	typedef std::map<int,factor> fmap;
	fmap factors;
};
	
#endif
