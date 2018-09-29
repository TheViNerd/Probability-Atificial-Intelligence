#include "bn.h"
#include <exception>
#include <string>
#include <sstream>
#include <algorithm> 
#include <map>
#include <set>
#include <vector>
using namespace std;

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
// set to scope:


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

bn::cliquetree bn::cliqueTreeCon(const vector<int> &allOrder) const {
	// to be implemented
    cliquetree ret;
//    std::vector<int> allOrder = order;//allOrder is the vars for elimation
    std::vector<factor::scope> BigFaiScope;
    std::map<int,std::set<int> > indexMap;//index for one vars and the index number in BigFai
    for (fmap::const_iterator it=factors.begin(); it!=factors.end(); ++it) {
        factor::scope valueScope = it->second.getscope();
        BigFaiScope.push_back(valueScope);
        for (factor::scope::const_iterator scopeIt=valueScope.begin(); scopeIt!=valueScope.end(); ++scopeIt) {
            indexMap[scopeIt->first].insert((int)BigFaiScope.size()-1);
        }
    }
    int N = (int)BigFaiScope.size();//the limits between new tao and old scope
    for(int k=0;k<allOrder.size();++k){
        int varIndex = allOrder[k];
        //get indexFai1 and erase it from indexMap;
        std::set<int> indexFai1 = indexMap[varIndex];
        //calculate Fai and Tao scope
        factor::scope tempFaiScope;
        for (std::set<int>::const_iterator Fai1It=indexFai1.begin(); Fai1It!=indexFai1.end(); ++Fai1It) {
            if(*Fai1It>=N){
                ret.adj.insert(make_pair((*Fai1It)-N,ret.nodes.size()));
                ret.adj.insert(make_pair(ret.nodes.size(),(*Fai1It)-N));
            }
            factor::scope thisScope = BigFaiScope[*Fai1It];//current scope
            tempFaiScope = tempFaiScope+thisScope;
            //elimate the index of thisFactor
            for (factor::scope::const_iterator scopeIt=thisScope.begin(); scopeIt!=thisScope.end(); ++scopeIt) {
                indexMap[scopeIt->first].erase(*Fai1It);
            }
        }
        ret.nodes.push_back(tempFaiScope);
        indexMap.erase(varIndex);
        //calculate Tao scope
        factor::scope tempTaoScope = tempFaiScope;
        tempTaoScope.erase(varIndex);
        //save Tao
        BigFaiScope.push_back(tempTaoScope);
//        //add the index of TaoScope
        for (factor::scope::const_iterator scopeIt=tempTaoScope.begin(); scopeIt!=tempTaoScope.end(); ++scopeIt) {
            indexMap[scopeIt->first].insert((int)BigFaiScope.size()-1);
        }
    }
    // modify to make smallest clique
    while(true){
        int isOver=1;
        for (std::multimap<int,int>::const_iterator it=ret.adj.begin(); it!=ret.adj.end(); ++it) {
            factor::scope large;
            factor::scope small;
            int removeNode;
            int unremoveNode;
            if (ret.nodes[it->first].size()>ret.nodes[it->second].size()) {
                large = ret.nodes[it->first];
                small = ret.nodes[it->second];
                removeNode = it->second;
                unremoveNode = it->first;
            }else{
                small = ret.nodes[it->first];
                large = ret.nodes[it->second];
                removeNode = it->first;
                unremoveNode = it->second;
            }
            int isContain = 1;
            //test if large<->small has contains
            for (factor::scope::const_iterator it1=small.begin(); it1!=small.end(); ++it1) {
                if (large.find(it1->first)==large.end()) {
                    isContain=0;
                    break;
                }
            }
            if (isContain) {
                //handle it
                //change adj, delete remove neighbor to
                std::vector<std::multimap<int,int>::iterator> removeIt;
                std::vector<pair<int,int> > addElement;
                for (std::multimap<int,int>::iterator findIt=ret.adj.begin(); findIt!=ret.adj.end(); ++findIt) {
                    if (findIt->first==removeNode ) {
                        removeIt.push_back(findIt);
                        if (findIt->second!=unremoveNode) {
                            addElement.push_back(make_pair(unremoveNode, findIt->second));
                        }
                    }
                    if (findIt->second==removeNode ) {
                        removeIt.push_back(findIt);
                        if (findIt->first!=unremoveNode) {
                            addElement.push_back(make_pair(findIt->first, unremoveNode));
                        }
                    }
                }
                for (int i=0; i<removeIt.size(); ++i) {
                    ret.adj.erase(removeIt[i]);
                }
                for (int i=0; i<addElement.size(); ++i) {
                    ret.adj.insert(addElement[i]);
                }
                //remove nodes
                ret.nodes.erase(ret.nodes.begin()+removeNode);
                //change paris that int>removeNode
                std::vector<std::multimap<int,int>::iterator> changeIt;
                std::vector<pair<int, int>> temp;
                for (std::multimap<int,int>::iterator findIt=ret.adj.begin(); findIt!=ret.adj.end(); ++findIt) {
                    if (findIt->first>removeNode && findIt->second<=removeNode) {
                        changeIt.push_back(findIt);
                        temp.push_back(make_pair(findIt->first-1, findIt->second));
                    }
                    if (findIt->second>removeNode && findIt->first<=removeNode) {
                        changeIt.push_back(findIt);
                        temp.push_back(make_pair(findIt->first, findIt->second-1));
                    }
                    if (findIt->second>removeNode && findIt->first>removeNode) {
                        changeIt.push_back(findIt);
                        temp.push_back(make_pair(findIt->first-1, findIt->second-1));
                    }
                }
                for (int i=0; i<changeIt.size(); ++i)
                    ret.adj.erase(changeIt[i]);
                for (int i=0; i<temp.size(); ++i)
                    ret.adj.insert(temp[i]);
                isOver=0;
                break;
            }
        }
        if (isOver) {
            break;
        }
    }
    return ret;
}



bn::cliquetree bn::cliqueTreeConCG() const{
    cliquetree ret;
    //convert to undirect graph
    unGraph ug;
    for (int i=0; i<factors.size(); ++i) {
        ug.vertices.push_back(i);
    }
    std::map<int, std::set<int> > neighbors;
    for (fmap::const_iterator it=factors.begin(); it!=factors.end(); ++it) {
        factor::scope tempScope = it->second.getscope();
        vector<int> allNodes;
        for (factor::scope::const_iterator itScope=tempScope.begin(); itScope!=tempScope.end(); ++itScope) {
            allNodes.push_back(itScope->first);
        }
        for (int i=0; i<allNodes.size(); ++i) {
            for (int j=i+1; j<allNodes.size(); ++j) {
                ug.edges.insert(make_pair(allNodes[i], allNodes[j]));
                ug.edges.insert(make_pair(allNodes[j], allNodes[i]));
                neighbors[allNodes[i]].insert(allNodes[j]);
                neighbors[allNodes[j]].insert(allNodes[i]);
            }
        }
    }
    //triangulations
    std::map<int, std::set<int> > tempNeighbors(neighbors);
    while (tempNeighbors.size()>0) {
        //chose the one with smallest neighbor
        int smallest=(int)factors.size();
        int varIndex = -1;
        for (std::map<int, std::set<int> >::const_iterator it=tempNeighbors.begin();it!=tempNeighbors.end(); ++it) {
            if (it->second.size()<smallest) {
                smallest = (int)it->second.size();
                varIndex = it->first;
            }
        }
        //delete it from tempNeighbors
        //all neighbors near varIndex
        //all element have neighbors that is varIndex
        std::vector<pair<int, std::set<int> > > temp;
        std::set<int> changeIt = tempNeighbors.at(varIndex);
        for (std::set<int>::const_iterator it=changeIt.begin();it!=changeIt.end();++it) {
            std::set<int> t = tempNeighbors.at(*it);
            tempNeighbors.erase(tempNeighbors.find(*it));
            t.erase(t.find(varIndex));
            temp.push_back(make_pair(*it,t));
        }
        for (int i=0;i<temp.size();++i) {
            tempNeighbors.insert(temp[i]);
        }
        tempNeighbors.erase(tempNeighbors.find(varIndex));//this row
        //add induce edges to induced and to neighbors
        if (changeIt.size()>1) {
            std::vector<int> a(changeIt.begin(),changeIt.end());
            for (int i=0; i<a.size(); ++i) {
                for (int j=i+1; j<a.size(); ++j) {
                    if (neighbors.at(a[i]).find(a[j])==neighbors.at(a[i]).end()) {
                        neighbors[a[i]].insert(a[j]);
                        neighbors[a[j]].insert(a[i]);
                        tempNeighbors[a[i]].insert(a[j]);
                        tempNeighbors[a[j]].insert(a[i]);
                        ug.edges.insert(make_pair(a[i], a[j]));
                        ug.edges.insert(make_pair(a[i], a[j]));
                    }
                }
            }
        }
    }
//    find cliques from ug
    std::set<std::set<int> > cliques;
    for (int i=0; i<ug.vertices.size(); ++i) {
        //find the max cliques
        std::vector<int> nei(neighbors[i].begin(),neighbors[i].end());
        if (nei.size()==1) {
            std::set<int> temp;
            temp.insert(nei[0]);
            temp.insert(i);
            cliques.insert(temp);
            continue;
        }
        for (int j1=0; j1<nei.size(); ++j1) {
            for (int j2=j1+1; j2<nei.size(); ++j2) {
                if (neighbors.at(nei[j1]).find(nei[j2])!=neighbors.at(nei[j1]).end()) {
                    std::set<int> temp;
                    temp.insert(nei[j1]);
                    temp.insert(nei[j2]);
                    temp.insert(i);
                    cliques.insert(temp);
                }
            }
        }
    }
    //combine some cliques to 4 if possible
    std::map<std::set<int>,int> tempMap;
    for (std::set<std::set<int> >::const_iterator it1=cliques.begin(); it1!=cliques.end(); ++it1) {
        for (std::set<std::set<int> >::const_iterator it2=cliques.begin(); it2!=cliques.end(); ++it2) {
            std::set<int> temp;
            temp.insert((*it1).begin(),(*it1).end());
            temp.insert((*it2).begin(),(*it2).end());
            if (temp.size()==4) {
                if (tempMap.find(temp)==tempMap.end()) {
                    tempMap.insert(make_pair(temp, 1));
                }
                else{
                    tempMap.at(temp)++;
                }
            }
        }
    }
    std::set<int> newInsert;
    for (std::map<std::set<int>,int>::const_iterator it=tempMap.begin(); it!=tempMap.end(); ++it) {
        if (it->second == 12) {
            newInsert = (it->first);
        }
    }
    for (std::set<std::set<int> >::iterator it1=cliques.begin(); it1!=cliques.end();) {
        std::set<int> temp(newInsert);
        temp.insert((*it1).begin(),(*it1).end());
        if (temp.size()==newInsert.size()) {
            it1 = cliques.erase(it1);
        }
        else
            ++it1;
    }
    cliques.insert(newInsert);
//    //find the connection between cliques;
    for (std::set<std::set<int> >::const_iterator it=cliques.begin(); it!=cliques.end(); ++it) {
        std::vector<int> temp((*it).begin(),(*it).end());
        factor::scope tempScope;
        for (int i=0; i<temp.size(); ++i) {
            tempScope[temp[i]]=2;
        }
        ret.nodes.push_back(tempScope);
    }
    //find the connection one by one
    std::set<pair<int, int>> tempAdj;
    for (int i=0; i<ret.nodes.size(); ++i) {
        int maxInter = 0;
        int maxIndex = -1;
        for (int j=0; j<ret.nodes.size(); ++j) {
            if (j!=i) {
                int a = (int)(ret.nodes[i].size()+ret.nodes[j].size())-(int)(ret.nodes[i]+ret.nodes[j]).size();
                if (a>maxInter) {
                    maxInter = a;
                    maxIndex = j;
                }
            }
        }
        //adj i<--->maxIndex
        tempAdj.insert(make_pair(i, maxIndex));
        tempAdj.insert(make_pair(maxIndex, i));
    }
    ret.adj.insert(tempAdj.begin(),tempAdj.end());
    return ret;
}
