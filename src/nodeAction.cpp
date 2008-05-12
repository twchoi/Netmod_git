#include <netmodeler.h>
#include "nodeAction.h"
#include "netmodeler.h"

using namespace Starsky;
using namespace std;

NodeLeaveAction::NodeLeaveAction(EventScheduler& sched, DeetooNetwork& cn, DeetooNetwork& qn, AddressedNode* me)
      : _sched(sched), _cnet(cn), _qnet(qn), _me(me)
{

}
void NodeLeaveAction::Execute() {
  
  cout << "-------------NodeLeaveAction-------------- " 
	  << _me->getAddress(1) << ", "
	  << _me->getAddress(0) << endl;
  
  //schedule a time to add a new node:
  std::cout << _sched.getCurrentTime() << "\t"
            << _cnet.getNodeSize() << "\t"
            << _cnet.getEdgeSize() << "\t"
	    << _qnet.getNodeSize() << "\t"
	    << _qnet.getEdgeSize() 
            << std::endl;
  my_int caddr = _me->getAddress(1);
  my_int qaddr = _me->getAddress(0);
  cout << "caddr & qaddr: " << caddr << ", " << qaddr << endl;
  auto_ptr<NodeIterator> ni (_cnet.getNodeIterator() );
  while (ni->moveNext() ) { 
    AddressedNode* c = dynamic_cast<AddressedNode*> (ni->current() );
    cout << "cur addr: " << c->getAddress(1) << ", " << c->getAddress(0) << endl;
  }
  map<my_int, AddressedNode*>::const_iterator mit;
  for (mit=_cnet.node_map.begin(); mit!=_cnet.node_map.end(); mit++) {
    cout << "c_addr: " << mit->first << endl;
  }
  cout << "---------" << endl;
  _cnet.node_map.erase(_cnet.node_map.find(caddr));
  cout << "---------" << endl;
  _qnet.node_map.erase(_qnet.node_map.find(qaddr));
  cout << "---------" << endl;
  _cnet.remove(_me);
  cout << "---------" << endl;
  _qnet.remove(_me);
  cout << "---------" << endl;
  //my_int caddr = _me->getAddress(1);
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
  cout << "-------------NodeJoinAction-------------- " << endl;
  my_int c_addr = (my_int)(_r.getDouble01() * WMAX);
  //cout << "c_addr: " << c_addr << endl;
  //std::set<std::string> items;
  std::vector<StringObject> items;
  items.clear();
  AddressedNode* me = 0;
  bool fresh_addr = 0;
  while (!fresh_addr) {
    if (_cnet.node_map.find(c_addr) == _cnet.node_map.end() && c_addr != 0) {
      me = new AddressedNode(c_addr, items);
      //cout << "in while addr: " << me->getAddress(1) << endl;
      fresh_addr = 1;
    }
  }
  my_int q_addr = me->getAddress(0);
  //cout << "q_addr: " << q_addr << endl;
  getConnection(_cnet, me, true);
  getConnection(_qnet, me, false);
  //Make sure I get added no matter what
  _cnet.add(me);
  _qnet.add(me);
  _cnet.node_map[c_addr] = me;
  _qnet.node_map[q_addr] = me;

  //Plan to leave:
  //double lifetime = 3600.0 * _r.getDouble01();
  double lifetime = _r.getExp(3600.0);
  Action* leave = new NodeLeaveAction(_sched, _cnet, _qnet, me);
  _sched.after(lifetime, leave);
  //Plan to rejoin
  //double sleeptime = 3600.0 * _r.getDouble01();
  double sleeptime = _r.getExp(3600.0);
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
  //cout << "-------------getConnection: " << endl;
  my_int addr = me->getAddress(cache);
  /**
  map<my_int, AddressedNode*>::const_iterator ttt;
  for (ttt = net.node_map.begin(); ttt != net.node_map.end(); ttt++) {
    cout << ttt->first << ", ";
  }
  cout << endl;
  */
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
    map<my_int, AddressedNode*>::const_iterator cit = net.node_map.upper_bound(addr);
    AddressedNode* neighbor0;
    AddressedNode* neighbor1; 
    if (cit == net.node_map.end() || cit == net.node_map.begin() ) { //my node has the biggest or smallest address.
      //cout << "biggest or smallest" << endl;
      neighbor1 = (net.node_map.begin())->second;
      neighbor0 = (net.node_map.rbegin())->second; 
      //cout << neighbor1->getAddress(cache) << ", " << neighbor0->getAddress(cache) << endl; 
    }
    else {  //my node has an address between min and max address
      neighbor0 = cit->second;
      cit--;
      neighbor1 = cit->second;
      net.makeShortcutConnection(net.node_map, cache);
      //cout << neighbor1->getAddress(cache) << ", " << neighbor0->getAddress(cache) << endl; 
    }
    //remove edge between neighbor0 and neighbor1 if it exists
    Edge* old_edge = net.getEdge(neighbor0,neighbor1);
    if (old_edge != 0) {
      //AddressedNode* no0 = dynamic_cast<AddressedNode*> (old_edge->first);
      //AddressedNode* no1 = dynamic_cast<AddressedNode*> (old_edge->second);
      //cout << "old_edge: " << no0->getAddress(cache)
	//    << " : " << no1->getAddress(cache) << endl;
      net.remove(old_edge);
    }
    //cout << "before edge addition: " << net.getEdgeSize() << endl;
    net.add(Edge(me, neighbor0));
    net.add(Edge(me, neighbor1));
    //make sure ring is closed.
    AddressedNode* front = net.node_map.begin()->second;
    AddressedNode* back = (net.node_map.rbegin())->second;
    net.add(Edge(front,back));
    //cout << "after edge addition: " << net.getEdgeSize() << endl;
  }
}
//CacheAction::CacheAction(EventScheduler& sched, Random& r, DeetooNetwork* n, StringObject so, double sq_alpha) : _sched(sched), _r(r), _net(n), _so(so),  _sq_alpha(sq_alpha)
CacheAction::CacheAction(EventScheduler& sched, Random& r, INodeSelector& ns, DeetooNetwork& n, StringObject so, double sq_alpha) : _sched(sched), _r(r), _ns(ns), _net(n), _so(so),  _sq_alpha(sq_alpha)
{
}
void CacheAction::Execute() {
  cout << "cacheaction start here" << endl;
  std::cout << _sched.getCurrentTime() << "\t"
            << _net.getNodeSize() << "\t"
            << _net.getEdgeSize() << "\t"
	    << std::endl;
  //schedule a time to cache object to nodes in the range:
  UniformNodeSelector u_node(_r);
  _ns.selectFrom(&_net);
  AddressedNode* node = dynamic_cast<AddressedNode*> (_ns.select() );
  double guess = _net.guessNetSizeLog(node,1);
  cout << "c_addr: " << node->getAddress(1) << ", sq_alpha: " << _sq_alpha << ", guessNetSize: " << guess << endl;
  double cqsize = (double) (((WMAX) / (double)sqrt(_net.guessNetSizeLog(node,1) ) ) * _sq_alpha);
  cout << "cqsize: " << cqsize << endl;
  std::pair<my_int, my_int> range = _net.getRange(cqsize);
  my_int rg_start = range.first;
  my_int rg_end = range.second;
  cout << "rg_start: "<< rg_start << ", rg_end: " << rg_end << endl;
  _so.start = rg_start;
  _so.end = rg_end;
  node->insertObject(_so);
  auto_ptr<DeetooMessage> cache_m(new DeetooMessage(rg_start, rg_edn, true, _r, 0.0) );
  auto_ptr<DeetooNetwork> tmp_net (cache_m.visit(node, _net));
  auto_ptr<NodeIterator> ni (tmp_net->getNodeIterator() );
  while (ni->moveNext() ) {
    AddressedNode* inNode = dynamic_cast<AddressedNode*> (ni->current() );
    inNode->insertObject(_so);
  }
}
QueryAction::QueryAction(EventScheduler& sched, Random& r, INodeSelector& ns, DeetooNetwork& n, StringObject so, double sq_alpha) : _sched(sched), _r(r), _ns(ns), _net(n), _so(so), _sq_alpha(sq_alpha)
{

}
void QueryAction::Execute() {
  //schedule a time to query object to nodes in the range:
  cout << "queryaction start here" << endl;
  std::cout << _sched.getCurrentTime() << "\t"
            << _net.getNodeSize() << "\t"
            << _net.getEdgeSize() << "\t"
	    << std::endl;
  UniformNodeSelector u_node(_r);
  _ns.selectFrom(&_net);
  AddressedNode* node = dynamic_cast<AddressedNode*> (_ns.select() );
  double guess = _net.guessNetSizeLog(node,0);
  cout << "q_addr: " << node->getAddress(0) << ", sq_alpha: " << _sq_alpha << ", guessNetSize: " << guess << endl;
  double cqsize = (double) (((WMAX ) / (double)sqrt(_net.guessNetSizeLog(node,0) ) ) * _sq_alpha);
  std::pair<my_int, my_int> range = _net.getRange(cqsize);
  my_int rg_start = range.first, rg_end = range.second;
  auto_ptr<DeetooMessage> query_m (new DeetooMessage(rg_start, rg_end,false, _r, 0.0) );
  auto_ptr<DeetooNetwork> tmp_net (query_m.visit(node, _net));
  no_msg = tmp_net->getNodeSize();
  int q_in_depth = tmp_net->getDistance(query_m->init_node);
  depth = q_in_depth + query_m->out_edge_count;
  sum_hits = 0;
  auto_ptr<NodeIterator> ni (tmp_net->getNodeIterator() );
  while (ni->moveNext() ) {
    AddressedNode* inNode = dynamic_cast<AddressedNode*> (ni->current() );
    sum_hits += inNode->searchObject(_so);
  } 
}
