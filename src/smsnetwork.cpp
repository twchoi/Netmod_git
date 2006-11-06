/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2005  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "smsnetwork.h"
#include <cmath>

using namespace Starsky;
using namespace std;

Smsnetwork::Smsnetwork(Ran1Random& r) : Network(), _r_short(r) {}
Smsnetwork::Smsnetwork(int nodes, Ran1Random& r) : _r_short(r) {
    //node_vec.reserve(nodes);
    //create(nodes);
}
/**
bool compareNodes(const AddressedNode* a, const AddressedNode* b) {
    return a->getAddress() < b->getAddress();
}
*/
/**
bool Smsnetwork::isIn(std::vector<AddressedNode*> n_vec, unsigned long int nd_addr) {
    std::vector<AddressedNode*>::iterator it;
    for (it = n_vec.begin(); it != n_vec.end(); it++) {
        if (nd_addr == (*it)->getCacheAddress() ) {
		return nd_addr;
	}
    }
}

*/
#define AMAX 65536
void Smsnetwork::create(int n) {
    //Let's make n different nodes!!
    node_map.clear();
    for(int j=0; j < n; j++) {
	unsigned long int r_int_f = (unsigned long int)_r_short.getDouble01() * AMAX;
	unsigned long int r_int_b = (unsigned long int)_r_short.getDouble01() * AMAX;
	r_addr = r_int_f << 16 + r_int_b;
	std::set<std::string> items;
	items.clear();
	if (node_map.find(r_addr) != node_map.end() ){
            AddressedNode* anode = new AddressedNode(r_addr, items);
	    node_map[r_addr] = anode;
            //node_vec.push_back(anode);
	    add(anode);
	}
	//while( !isIn(node_vec, anode));
    }
			
    //Sort Nodes in address increasing order.
    //sort(node_vec.begin(), node_vec.end(), Compare_nodes);
    //sort(node_vec.begin(), node_vec.end() );
    
    //Form ring.
    //AddressedNode* first = itNode_map.begin()->second;
    AddressedNode* first = node_map.begin()->second;
    //node_vec.push_back(first);
    AddressedNode *tmp, *last = first;
    //_node_map[0] = first;
    add(first);
    std::map<unsigned long int, AddressedNode*>::iterator itNode_map;
    for (itNode_map = node_map.begin(); itNode_map != node_map.end(); itNode_map++) {
    //for(int k = 1; k < n; k++) 
        if (itNode_map == node_map.begin() ) {
		//do nothing. it is already added before for loop.
	}
        //tmp = node_vec[k];
        tmp = itNode_map->second;
        //tmp = *(itNode_vec);
	//add(tmp);
	//_node_map[k] = tmp;
	add(Edge(tmp, last));
	last = tmp;
    }
    //Close the circle
    add(Edge(last, first));
    
    //Establishing shortcut connections:
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      AddressedNode* nodei = dynamic_cast<AddressedNode*> (ni->current());
      double x = _r_short.getDouble01();
      unsigned long int k = (unsigned long int) pow(AMAX, x);
      unsigned long int shortcut_target_addr = (nodei->getCacheAddress() + k) % AMAX;
      unsigned long int shortcut_address = findShortcutAddress(shortcut_target_addr);
      AddressedNode* nodej = node_map[shortcut_address];
      if ((nodei->getCacheAddress() != nodej->getCacheAddress()) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){
          //add(Edge(nodei,nodej));
          add( Edge(nodei,nodej) );
      }

    }
    
}

void Smsnetwork::createQueryNet(std::map<unsigned long int, AddressedNode*> nd_map)
{
    //sort(nd_vec.begin(), nd_vec.end() );

    std::map<unsigned long int, AddressedNode*>::iterator itNd_map;
    AddressedNode* first = nd_map.begin()->second;
    add(first);
    AddressedNode *tmp, *last = first;
    for (itNd_map = nd_map.begin(); itNd_map != nd_map.end(); itNd_map++)
    //for (int k = 1; k<nd_vec.size(); k++)
    {
        if (itNd_map == nd_map.begin() ) {
		//do nothing. it is already added before for loop.
	}
	tmp = itNd_map->second;
	add(tmp);
	add(Edge(tmp, last));
	last = tmp;
    }
    add(Edge(last, first) );
    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while(ni->moveNext() ) {
	AddressedNode* nodei = dynamic_cast<AddressedNode*> (ni->current() );
	double x = _r_short.getDouble01();
        unsigned long int k = (unsigned long int) pow(AMAX, x);
        unsigned long int shortcut_target_addr = (nodei->getCacheAddress() + k) % AMAX;
        unsigned long int shortcut_address = findShortcutAddress(shortcut_target_addr);
        AddressedNode* nodej = nd_map[shortcut_address];
        if ((nodei->getQueryAddress() != nodej->getQueryAddress()) && !(getEdge(nodei, nodej)) && !(getEdge(nodej, nodei))){
            add( Edge(nodei,nodej) );
      }

    }
    
}
	

/**
AddressedNode* Smsnetwork::getNodeFromAddress(const int addr) const {
	map<int, AddressedNode*>::const_iterator i = _node_map.find(addr);
	if (i != _node_map.end()) {
		return i->second;
	}
	return 0;
}*/


// Find the shortcut node's address
// we have target address, let's find the nearest node 
// to the target address
unsigned long int Smsnetwork::findShortcutAddress(unsigned long int t_addr) {
    unsigned long int this_distance, min_distance=AMAX, this_addr, scAddr;
    std::map<unsigned long int, AddressedNode*>::iterator itNM;
    //for (int ii=0; ii< node_map.size(); ii++) {
    for (itNM=node_map.begin(); itNM!=node_map.end(); itNM++) {
	//this_addr = node_map.at(ii)->_c_address;
	this_addr = itNM->first;
        this_distance = distanceTo( this_addr, t_addr);	    
	if ( this_distance < min_distance) {
	    min_distance = this_distance;
	    scAddr = this_addr;
	}
    } return scAddr;
}	
    
unsigned long int Smsnetwork::distanceTo(unsigned long int addr_a, unsigned long int addr_b) {
    unsigned long int sm, bg, dt;
    sm = std::min(addr_a, addr_b);
    bg = std::max(addr_a, addr_b);
    return dt = std::min( (bg-sm),(AMAX-bg+sm) );
}

void Smsnetwork::printNetInfo() {
   auto_ptr<NodeIterator> ni (this->getNodeIterator() );
   while ( ni->moveNext() ) {
      AddressedNode* this_node = dynamic_cast<AddressedNode*> ( ni-> current() );
      //cout<< "current node's address is: " << this_node->getAddress() << endl;
      //cout<< "current node's address is: " << ni->current()->getAddress() << endl;
      if (this->getDegree(this_node) != 0) {
         auto_ptr<NodeIterator> nni (this->getNeighborIterator(this_node) );
	 cout << "neighbors of " << this_node->getCacheAddress() << "are: " << endl;
	 cout << "(";
	 while ( nni->moveNext() ) { 
	    AddressedNode* nei_node = dynamic_cast<AddressedNode*> (nni->current() );
	    cout << nei_node->getCacheAddress() << ",";
         }
	 cout << ")" << endl;
      }
      else {
         cout << "no neighbor" << endl;
      }
   }
}
		   	
vector<int> Smsnetwork::getNeighborDistHist(int bins) const {

   vector<int> ret_val;
   int this_nd;
   int this_bin;

   ret_val.resize(bins);
   for (int j = 0; j < bins; j++) {
      ret_val[j] = 0;
   }
   auto_ptr<EdgeIterator> ei( getEdgeIterator() );
   while(ei->moveNext() ) {
      Edge* e = ei->current();
      //if (e->getAttributes() == "SC") {
	 AddressedNode* node_first = dynamic_cast<AddressedNode*> (e->first);
	 AddressedNode* node_second = dynamic_cast<AddressedNode*> (e->second) ;
         this_nd =  node_first->getDistanceTo(this->getNodeSize(), node_second) ;
         this_bin = this_nd * bins;
	 ret_val[this_bin] = ret_val[this_bin] + 1;
      //}
   }
   return ret_val;
}

void Smsnetwork::cacheItem(std::string content, AddressedNode* cn) 
{
    //Determine cache size
    int csize = (int) (sqrt( AMAX / this->getNodeSize() ) );
    unsigned long int start_addr;
    if ( cn->addr_i == 0) {cn->addr_i = 1;}
    start_addr = (unsigned long int)(cn->addr_i-2/csize)*AMAX;
    unsigned long int end_addr = (unsigned long int)( ( (start_addr % AMAX)+csize)*AMAX+AMAX-1);
    std::map<unsigned long int, AddressedNode*> cache_nm;
    std::map<unsigned long int, AddressedNode*>::iterator itNodeMap;
    for (itNodeMap=node_map.begin(); itNodeMap!=node_map.end(); itNodeMap++) 
    {
	std::set<std::string> cacheItemSet = itNodeMap->second->getItem();
	if ( (itNodeMap->first >= start_addr && itNodeMap->first <= end_addr) && cacheItemSet.find(content)== cacheItemSet.end() )
	{
	    itNodeMap->second->insertItem(content, cn);
	}
    }
}
/**
Smsnetwork* Smsnetwork::queryForContent(AddressedNode* content, NodeIterator* ni)
{

}
*/