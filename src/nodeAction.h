/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2008  Taewoong Choi <twchoi@ufl.edu>, University of Florida

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

#ifndef starsky__nodeAction_h
#define starsky__nodeAction_h

#include "event.h"
#include "netmodeler.h"
#include "node.h"
#include "edge.h"
#include <map>
#include <iostream>
#include <cmath>

#ifdef INT64
  typedef unsigned long long my_int;
  #define WMAX 18446744073709551615LL
#else
  typedef unsigned long my_int;
  #define WMAX 4294967295L
#endif

namespace Starsky {
  // action for node leaves
  class NodeLeaveAction : public Action {
    public:
      NodeLeaveAction(EventScheduler& sched, DeetooNetwork& cn, DeetooNetwork& qn, AddressedNode* me); 
      void Execute();
    protected:
      EventScheduler& _sched;
      DeetooNetwork& _cnet;
      DeetooNetwork& _qnet;
      AddressedNode* _me;
  };

  // action for node joins 
  class NodeJoinAction : public Action {
    public:
      NodeJoinAction(EventScheduler& sched, Random& r, DeetooNetwork& cn, DeetooNetwork& qn);
      void Execute();
      /**
       * make connections to 2 direct neighbors and 1 shortcut neighbor.
       * @param net network which a node joins
       * @param me joining node
       * @param cache determine if this network is for cache or for query.
       */
      void getConnection(DeetooNetwork& net, AddressedNode* me, bool cache);
      /**
       * copy objects from neighbors
       * @param me joing node
       * @param nei neighbor node
       */
      void copyObjects(AddressedNode* me, AddressedNode* nei, bool cache);
    protected:
      EventScheduler& _sched;
      Random& _r;
      DeetooNetwork& _cnet;
      DeetooNetwork& _qnet;
  };

  // action for caching objects in the network
  class CacheAction : public Action {
    public:
      CacheAction(EventScheduler& sched, Random& r, INodeSelector& ns, DeetooNetwork& net, StringObject so, double sq_alpha);
      void Execute();
    protected:
      EventScheduler& _sched;
      Random& _r;
      DeetooNetwork& _net;
      INodeSelector& _ns;
      StringObject _so;
      double _sq_alpha;
  };
  // action for querying objects
  class QueryAction : public Action {
    public:
      //bool hit = false;
      int sum_hits;
      int no_msg;
      int depth;
      QueryAction(EventScheduler& sched, Random& r, INodeSelector& ns, DeetooNetwork& net, StringObject so, double sq_alpha);
      void Execute();
    protected:
      EventScheduler& _sched;
      Random& _r;
      DeetooNetwork& _net;
      INodeSelector& _ns;
      StringObject _so;
      double _sq_alpha;
  };
}
#endif


