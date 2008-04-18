#include <netmodeler.h>
#include "nodeAction.h"

using namespace Starsky;
using namespace std;

NodeLeaveAction::NodeLeaveAction(EventScheduler& sched, DeetooNetwork& cn, DeetooNetwork& qn, AddressedNode* me)
      : _sched(sched), _cnet(cn), _qnet(qn), _me(me)
{

}
void NodeLeaveAction::Execute() {
  //schedule a time to add a new node:
  _cnet.remove(_me);
  _qnet.remove(_me);
  my_int caddr = _me->getAddress(1);
  my_int qaddr = _me->getAddress(0);
  _cnet.node_map.erase(_cnet.node_map.find(caddr));
  _qnet.node_map.erase(_qnet.node_map.find(qaddr));
  std::cout << _sched.getCurrentTime() << "\t"
            << _cnet.getNodeSize() << "\t"
            << _cnet.getEdgeSize() << "\t"
	    << _qnet.getNodeSize() << "\t"
	    << _qnet.getEdgeSize() 
            << std::endl;
}


/**
 * Represents a node joining a network
 * When we execute, we select two random nodes and make edges to them,
 * and schedule a time to leave.
 */
NodeJoinAction::NodeJoinAction(EventScheduler& sched, Random& r, DeetooNetwork& cn, DeetooNetwork& qn) : _sched(sched), _r(r), _cnet(cn), _qnet(qn)
{

}

void NodeJoinAction::Execute() {
  my_int c_addr = (my_int)(_r.getDouble01() * WMAX);
  std::set<std::string> items;
  items.clear();
  AddressedNode* me = 0;
  while (_cnet.node_map.find(c_addr) != _cnet.node_map.end() || c_addr != 0) {
  //if (_cnet.node_map.find(c_addr) == _cnet.node_map.end() && c_addr != 0) {
    me = new AddressedNode(c_addr, items);
  }
  my_int q_addr = me->getAddress(0);
      
  getConnection(_cnet, me, true);
  getConnection(_qnet, me, false);
  //Make sure I get added no matter what
  _cnet.add(me);
  _qnet.add(me);
  _cnet.node_map[c_addr] = me;
  _qnet.node_map[q_addr] = me;

  //Plan to leave:
  double lifetime = 3600.0 * _r.getDouble01();
  Action* leave = new NodeLeaveAction(_sched, _cnet, _qnet, me);
  _sched.after(lifetime, leave);
  //Plan to rejoin
  double sleeptime = 3600.0 * _r.getDouble01();
  Action* rejoin = new NodeJoinAction(_sched, _r, _cnet, _qnet);
  _sched.after(lifetime + sleeptime, rejoin);
  //Print out results:
  std::cout << _sched.getCurrentTime() << "\t"
            << _cnet.getNodeSize() << "\t"
            << _cnet.getEdgeSize() << "\t"
            << _qnet.getNodeSize() << "\t"
            << _qnet.getEdgeSize() 
	    << std::endl;
}

void NodeJoinAction::getConnection(DeetooNetwork& net, AddressedNode* me, bool cache)
{
  my_int addr = me->getAddress(cache);
  // make connections: ring connection as well as shortcut connections
  if (net.getNodeSize() == 0) {
    // *me* is the only node in this network
    // no connection is needed.
  }
  else if ( net.getNodeSize() == 1) {
    AddressedNode* neighbor = net.node_map.begin()->second;
    if(neighbor != 0) {
      net.add(Edge(me, neighbor));
    }
  }
  else {
    // For caching network.
    map<my_int, AddressedNode*>::const_iterator cit = net.node_map.upper_bound(addr);
    AddressedNode* neighbor0;
    AddressedNode* neighbor1; 
    if (cit == net.node_map.end() ) { //my node has the biggest address.
      neighbor1 = (net.node_map.begin())->second;
      cit--;
      neighbor0 = cit->second; 
    }
    else if(cit == net.node_map.begin() ) { // my node has the smallest address.
      neighbor1 = cit->second;
      cit = net.node_map.end();
      cit--;
      neighbor0 = cit->second;
    }
    else {  //my node has an address between min and max address
      neighbor0 = cit->second;
      cit--;
      neighbor1 = cit->second;
      net.makeShortcutConnection(net.node_map, cache);
    }
    //remove edge between neighbor0 and neighbor1
    Edge* old_edge = net.getEdge(neighbor0,neighbor1);
    net.remove(old_edge);
    net.add(Edge(me, neighbor0));
    net.add(Edge(me, neighbor1));
  }
}
