#include "factor.h"

using namespace std;

// You should add your own helper functions and private methods

factor::factor(const scope &sc, double d) {
// your code here
    int numOfAssign = 1;
    scope::const_iterator it;
    for (it=sc.begin(); it!=sc.end(); ++it) {
        vars.push_back(it->first);
        stride.push_back(numOfAssign);
        card.push_back(it->second);
        factorScope[it->first]=it->second;
        numOfAssign *= it->second;
    }
    for (int i=0; i<numOfAssign; ++i) {
        factorArray.push_back(d);
    }
}

double factor::operator()(const factor::assign &i) const {
// your code here
    //calculate the index
    int index = 0;
    for (int j=0; j<vars.size(); ++j) {
        index += (i.at(vars[j]))*stride[j];
    }
    return factorArray[index];
}

double &factor::operator()(const factor::assign &i) {
// your code here
    //calculate the index
    int index = 0;
    for (int j=0; j<vars.size(); ++j) {
        index += (i.at(vars[j]))*stride[j];
    }
    return factorArray[index];
}

void factor::print(ostream &os) const {
// your code here
    //print vars
    for (int i=0; i<vars.size(); ++i) {
        os<<"X"<<vars[i]<<"\t";
    }
    os<<"value"<<endl;
    os<<string(7*(vars.size()+1),'-')<<endl;
    for (int i=0; i<factorArray.size(); ++i) {
        for (int j=0; j<vars.size(); ++j) {
            int tem;
            tem = (i/stride[j])%card[j];
            cout<<tem<<"\t";
        }
        cout<<factorArray[i]<<endl;
    }
}

factor factor::reduce(const assign &a) const {
// your code here
    scope retScope;
    
    for (int i=0; i<vars.size(); ++i) {
        if (a.find(vars[i])==a.end()) {
            retScope[vars[i]]=card[i];
        }
    }

    factor ret(retScope);
    for (int i=0; i<ret.factorArray.size(); ++i) {
        //find the assign of i
        assign temp;
        temp.insert(a.begin(),a.end());
        for (int j=0; j<ret.vars.size(); ++j) {
            int ass = (i/ret.stride[j])%(ret.card[j]);
            temp[ret.vars[j]]=ass;
        }
        double value = this->operator()(temp);
        ret.factorArray[i]=value;
    }
    return ret;
}

factor factor::marginalize(const scope &tosumout) const {
// your code here
    scope retScope;
    for (int i=0; i<vars.size(); ++i) {
        if (tosumout.find(vars[i])==tosumout.end()) {
            retScope[vars[i]]=card[i];
        }
    }
    factor ret(retScope);
    for (int i=0; i<ret.factorArray.size(); ++i) {
        assign temp;
        for (int j=0; j<ret.vars.size(); ++j) {
            int ass = (i/ret.stride[j])%(ret.card[j]);
            temp[ret.vars[j]]=ass;
        }
        factor tempFactor = this->reduce(temp);
        double value=0;
        for (int j=0; j<tempFactor.factorArray.size(); ++j) {
            value += tempFactor.factorArray[j];
        }
        ret.factorArray[i] = value;
    }
    
    return ret;
}

factor factor::operator*(const factor &f2) const {
// your code here
    scope retScope = getscope();
    scope::const_iterator it;
    for (int i=0; i<f2.vars.size(); ++i) {
        retScope[f2.vars[i]]=f2.getscope()[f2.vars[i]];
    }
    factor ret(retScope);
    for (int i=0; i<ret.factorArray.size(); ++i) {
        assign temp;
        for (int j=0; j<ret.vars.size(); ++j) {
            int ass = (i/ret.stride[j])%(ret.card[j]);
            temp[ret.vars[j]]=ass;
        }
        double value1 = (this->reduce(temp)).factorArray[0];
        double value2 = f2.reduce(temp).factorArray[0];
        ret(temp) = value1*value2;
    }
    return ret;
}
factor factor::operator/(const factor &f2) const {
// your code here
    scope retScope = getscope();
    scope::const_iterator it;
    for (int i=0; i<f2.vars.size(); ++i) {
        retScope[f2.vars[i]]=f2.getscope()[f2.vars[i]];
    }
    factor ret(retScope);
    for (int i=0; i<ret.factorArray.size(); ++i) {
        assign temp;
        for (int j=0; j<ret.vars.size(); ++j) {
            int ass = (i/ret.stride[j])%(ret.card[j]);
            temp[ret.vars[j]]=ass;
        }
        double value1 = (this->reduce(temp)).factorArray[0];
        double value2 = f2.reduce(temp).factorArray[0];
        ret(temp) = value1/value2;
    }
    return ret;
}
factor factor::operator+(const factor &f2) const {
// your code here
    scope retScope = getscope();
    scope::const_iterator it;
    for (int i=0; i<f2.vars.size(); ++i) {
        retScope[f2.vars[i]]=f2.getscope()[f2.vars[i]];
    }
    factor ret(retScope);
    for (int i=0; i<ret.factorArray.size(); ++i) {
        assign temp;
        for (int j=0; j<ret.vars.size(); ++j) {
            int ass = (i/ret.stride[j])%(ret.card[j]);
            temp[ret.vars[j]]=ass;
        }
        double value1 = (this->reduce(temp)).factorArray[0];
        double value2 = f2.reduce(temp).factorArray[0];
        ret(temp) = value1+value2;
    }
    return ret;
}
factor factor::operator-(const factor &f2) const {
// your code here
    scope retScope = getscope();
    scope::const_iterator it;
    for (int i=0; i<f2.vars.size(); ++i) {
        retScope[f2.vars[i]]=f2.getscope()[f2.vars[i]];
    }
    factor ret(retScope);
    for (int i=0; i<ret.factorArray.size(); ++i) {
        assign temp;
        for (int j=0; j<ret.vars.size(); ++j) {
            int ass = (i/ret.stride[j])%(ret.card[j]);
            temp[ret.vars[j]]=ass;
        }
        double value1 = (this->reduce(temp)).factorArray[0];
        double value2 = f2.reduce(temp).factorArray[0];
        ret(temp) = value1-value2;
    }
    return ret;
}
factor::scope factor::getscope() const {
// your code here
    scope ret;
    ret.insert(factorScope.begin(),factorScope.end());
    return ret;
}

