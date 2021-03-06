/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

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

#include "randomnetwork.h"
#include <memory>

using namespace Starsky;
using namespace std;

RandomNetwork::RandomNetwork(int nodes,
                             double p,
			     Random& rand) : Network(),
                                              _rand_gen(rand) {
  create(nodes,p);
}
		
void RandomNetwork::create(int nodes, double p) {

    for(int k = 0; k < nodes; k++) {
        add( new Node() );
    }

    auto_ptr<NodeIterator> ni( getNodeIterator() );
    while( ni->moveNext() ) {
      //Start at nodei
      auto_ptr<NodeIterator> nj( ni->clone() );
      while( nj->moveNext() ) {
        if( _rand_gen.getBool(p) ) {
          Node* nodei = ni->current();
	  Node* nodej = nj->current();
          add( Edge(nodei,nodej) );
	}
      }
    }
}
