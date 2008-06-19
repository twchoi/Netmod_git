/** 
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of Florida
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

#include "addressednode.h"

using namespace Starsky;
using namespace std;
//#define INT64
#ifdef INT64
  typedef unsigned long long my_int;
  #define WMAX 18446744073709551615LL
  #define AMAX 4294967296LL
#else
  typedef unsigned long my_int;
  #define AMAX 65536L
  #define WMAX 4294967295L
#endif

AddressedNode::AddressedNode()
{
  _c_address=0;
  _q_address=0;
}


AddressedNode::AddressedNode(const my_int addr, std::set<std::string> itemSet)
{
  _c_address = addr;
  addr_j = addr % AMAX;
  addr_i = (addr - addr_j) / AMAX;
  _q_address = addr_j*AMAX + addr_i;
  _itemSet = itemSet;
}

AddressedNode::AddressedNode(const my_int addr, std::set<StringObject> objSet)
{
  _c_address = addr;
  addr_j = addr % AMAX;
  addr_i = (addr - addr_j) / AMAX;
  _q_address = addr_j*AMAX + addr_i;
  _objSet = objSet;
}

my_int AddressedNode::getAddress(bool cache)
{
    if (cache) { return _c_address; }
    else { return _q_address; }
}

my_int AddressedNode::getDistanceTo(my_int t_addr, bool cache)
{
  my_int this_addr;
  if (cache) { this_addr = _c_address; }
  else { this_addr = _q_address;}
  _small = std::min (this_addr, t_addr);
  _big = std::max (this_addr, t_addr);
  //ring distance
  _dist = std::min ((_big-_small), ( WMAX - _big + _small +1));
  return _dist;
}

bool AddressedNode::searchItem(std::string qItem)
{
  if (_itemSet.find(qItem) != _itemSet.end() ) {
    return true;
  }
  else {
    return false;
  }
}
bool AddressedNode::searchObject(StringObject qObj)
{
  
  bool ret;	
  //cout << "qObj: " << qObj.content << endl;
  //cout << "_objSet.size() : " << _objSet.size() << endl;
  std::set<StringObject>::const_iterator sit;
  if (sit == _objSet.end() ) {
    ret = false;
  }
  else {
    for (sit = _objSet.begin(); sit != _objSet.end(); sit++) {
      StringObject ob = *sit;
      if (ob.content == qObj.content ) {
        ret = true;
      }
      else { ret = false; }
      cout << endl; 
    }
  }
  return ret;
}
void AddressedNode::insertItem(std::string item)
{
    _itemSet.insert(item);
}

void AddressedNode::insertObject(StringObject obj)
{
    _objSet.insert(obj);
}

void AddressedNode::deleteItem(std::string item)
{
    _itemSet.erase(item);
}
/**
void AddressedNode::deleteObject(StringObject obj)
{
  //erase() need iterator 
  std::set<StringObject>::iterator it = _objSet.find(obj);
  _objSet.erase(it);
}
*/
