#ifndef BN_H
#define BN_H

#include "factor.h"
#include<map>
#include<set>
#include<vector>

class bn { // class representing a Bayesian network
public:
	// no constructors or destructors needed -- defaults are fine
	
	// f needs to be a factor that includes v in its scope
	// (and represents the conditional distribution of v)
	void addvar(int v, const factor &f);

	// produces the marginal distribution over the variables
	// in marg, as a factor
	// employs variable elimination with the elimination order
	// given by order:
	//   all variables in the parameter order are eliminated first
	//         in the order they appear in the vector order
	//         (if a variable in marg appears in order, it
	//          is ignored -- as if it did not appear in order)
	//   then, if any more variables need to be eliminated
	//     they may be eliminated in any order
	factor varelim(const std::set<int> &marg,
		const std::vector<int> &order = std::vector<int>()) const;
	
private:
	// invariant:
	// if factors maps i to f, then f has i in its scope
	//    and if f is marginalized over i, it becomes a factor of all 1s
	//    (that is, f is a conditional distribution over i)
	// Note, the members of the scope of f that are not i are the
	//   parents of i in the BN
	typedef std::map<int,factor> fmap;
	fmap factors;
};
	
#endif
