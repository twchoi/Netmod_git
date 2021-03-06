/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida
Copyright (C) 2007  Tae Woong Choi <twchoi@ufl.edu>, University of Florida

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

#ifndef starsky__greedymessage_H
#define starsky__greedymessage_H

#include <message.h>
#include <addressednode.h>
#include <deetoonetwork.h>
#include <ran1random.h>
#include <random.h>
#include <memory>

//#define INT64
#ifdef INT64
  typedef unsigned long long my_int;
#else
  typedef unsigned long my_int;
#endif

namespace Starsky {

/**
 * Message that does a greey routing.  
 */
	
class GreedyMessage : public Message {

  public:
    GreedyMessage(Node* target, bool is1D, bool isCache);
    virtual DeetooNetwork* visit(Node* source, Network& net);
    bool isReached() { return _is_success; }

  protected:
    bool _is_success;
    bool _isCache;
    bool _is1D;
    AddressedNode* _target;
    void visit(AddressedNode* target, Network& net_to_visit, DeetooNetwork& visited_net);
    //void visit1D(Node* target, Network& net_to_visit, DeetooNetwork& visited_net);
    //void visit2D(Node* target, Network& net_to_visit, DeetooNetwork& visited_net);
  };
}

#endif
