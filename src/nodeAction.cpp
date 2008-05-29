#include <netmodeler.h>
#include "nodeAction.h"
#include "netmodeler.h"
#define FOREACH(it,col) for(it=col.begin();it != col.end();it++)
using namespace Starsky;
using namespace std;

NodeLeaveAction::NodeLeaveAction(EventScheduler& sched, DeetooNetwork& cn, DeetooNetwork& qn, AddressedNode* me)
      : _sched(sched), _cnet(cn), _qnet(qn), _me(me)
{

}
void NodeLeaveAction::Execute() {
  
  //cout << "-------------NodeLeaveAction-------------- " 
  //	  << _me->getAddress(1) << ", "
  //	  << _me->getAddress(0) << endl;
  
  //schedule a time to add a new node:
  my_int caddr = _me->getAddress(1);
  my_int qaddr = _me->getAddress(0);
  //cout << "caddr & qaddr: " << caddr << ", " << qaddr << endl;
  auto_ptr<NodeIterator> ni (_cnet.getNodeIterator() );
  while (ni->moveNext() ) { 
    AddressedNode* c = dynamic_cast<AddressedNode*> (ni->current() );
    //cout << "cur addr: " << c->getAddress(1) << ", " << c->getAddress(0) << endl;
  }
  map<my_int, AddressedNode*>::const_iterator mit;
  for (mit=_cnet.node_map.begin(); mit!=_cnet.node_map.end(); mit++) {
    //cout << "c_addr: " << mit->first << endl;
  }
  _cnet.node_map.erase(_cnet.node_map.find(caddr));
  _qnet.node_map.erase(_qnet.node_map.find(qaddr));
  _cnet.remove(_me);
  _qnet.remove(_me);
  //Need stabilization here!
  //connect broken ring. make connection between left and right neighbors of leaving node.
  map<my_int, AddressedNode*>::const_iterator c_n_it = _cnet.node_map.upper_bound(caddr);
  map<my_int, AddressedNode*>::const_iterator q_n_it = _qnet.node_map.upper_bound(qaddr);
  AddressedNode* c_left;
  AddressedNode* c_right;
  if (c_n_it ==_cnet.node_map.begin() || c_n_it == _cnet.node_map.end() ) {
    // leaving node's address is the biggest or the smallest.
    // the first and the last nodes was it's neighbors.
    // make connection between these two nodes.
    c_left = (_cnet.node_map.begin() )->second;
    c_right = (_cnet.node_map.rbegin() )->second;
  }
  else {
    // leaving node was in the middle of ring.
    c_left = c_n_it->second;
    c_n_it--;
    c_right = c_n_it->second;
  }
  
  AddressedNode* q_left;
  AddressedNode* q_right;
  if (q_n_it ==_qnet.node_map.begin() || q_n_it == _qnet.node_map.end() ) {
    q_left = _qnet.node_map.begin()->second;
    q_right = _qnet.node_map.rbegin()->second;
  }
  else {
    q_left = q_n_it->second;
    q_n_it--;
    q_right = q_n_it->second;
  }
  
  _cnet.add(Edge(c_left, c_right));
  _qnet.add(Edge(q_left, q_right));
  //my_int caddr = _me->getAddress(1);
  std::cout << _sched.getCurrentTime() << "\t"
	    << "Node_Leave\t"
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
  //cout << "-------------NodeJoinAction-------------- " << endl;
  my_int c_addr;
  //cout << "c_addr: " << c_addr << endl;
  //std::set<std::string> items;
  std::vector<StringObject> items;
  items.clear();
  AddressedNode* me = 0;
  bool fresh_addr = false;
  while (!fresh_addr) {
    c_addr = (my_int)(_r.getDouble01() * WMAX);
    if (_cnet.node_map.find(c_addr) == _cnet.node_map.end() && c_addr != 0) {
      me = new AddressedNode(c_addr, items);
      //cout << "in while addr: " << me->getAddress(1) << endl;
      fresh_addr = true;
    }
    else {
      fresh_addr = false;
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
	    << "Node_Join\t" 
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
      //net.makeShortcutConnection(net.node_map, cache);
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
    cout << "before edge addition: " << net.getEdgeSize() << endl;
    AddressedNode* shortcut = net.returnShortcutNode(me, net.node_map,true);
    cout << "before edge addition: " << net.getEdgeSize() << endl;
    set<AddressedNode*> neis;
    neis.insert(neighbor0);
    neis.insert(neighbor1);
    neis.insert(shortcut);
    //net.makeShortcutConnection(net.node_map, true);
      if(!(net.getEdge(me, neighbor0)) && !(net.getEdge(neighbor0, me))) {
        net.add(Edge(me, neighbor0));
      }
      if(!(net.getEdge(me, neighbor1)) && !(net.getEdge(neighbor1, me))) {
        net.add(Edge(me, neighbor1));
      }
      if(!(net.getEdge(me, shortcut)) && !(net.getEdge(shortcut, me))) {
        net.add(Edge(me, shortcut));
      }
      /**
    vector<StringObject> ll = neighbor0->getObject();
    set<AddressedNode*>::const_iterator lt_it;
    //for(lt_it = neis.begin(); lt_it != neis.end(); lt_it++) {
    FOREACH(lt_it, neis)
      if(!(net.getEdge(me, *lt_it)) && !(net.getEdge(*lt_it, me))) {
        net.add(Edge(me, *lt_it));
      }
    
      // Items of neighbors should be replicated to this node
      // if this node is in the item's range. 
      vector<StringObject> so = lt_it->getObject();
      vector<StringObject>::iterator so_it;
      for (so_it = so.begin(); so_it != so.end(); so_it++) {
        if( addr >= so_it->start && addr <= so_it->end) {
	  //MY node lies in the object's range, cache this object.
          me->insertObject(*so_it);
        }	      
        else {
          //out of range, do nothing.
        }
      }
    }
   */
    vector<StringObject> so0 = neighbor0->getObject();
    vector<StringObject>::iterator so_it0;
    for (so_it0 = so0.begin(); so_it0 != so0.end(); so_it0++) {
      if( addr >= so_it0->start && addr <= so_it0->end) {
        //MY node lies in the object's range, cache this object.
        me->insertObject(*so_it0);
      }	      
      else {
        //out of range, do nothing.
      }
    }
    vector<StringObject> so1 = neighbor1->getObject();
    vector<StringObject>::iterator so_it1;
    for (so_it1 = so1.begin(); so_it1 != so1.end(); so_it1++) {
      if( addr >= so_it1->start && addr <= so_it1->end) {
        //MY node lies in the object's range, cache this object.
        me->insertObject(*so_it1);
      }	      
      else {
        //out of range, do nothing.
      }
    }
    vector<StringObject> so2 = shortcut->getObject();
    vector<StringObject>::iterator so_it2;
    for (so_it2 = so2.begin(); so_it2 != so2.end(); so_it2++) {
      if( addr >= so_it2->start && addr <= so_it2->end) {
        //MY node lies in the object's range, cache this object.
        me->insertObject(*so_it2);
      }	      
      else {
        //out of range, do nothing.
      }
    }


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
  //cout << "cacheaction start here" << endl;
  //schedule a time to cache object to nodes in the range:
  //UniformNodeSelector u_node(_r);
  _ns.selectFrom(&_net);
  AddressedNode* node = dynamic_cast<AddressedNode*> (_ns.select() );
  double guess = _net.guessNetSizeLog(node,1);
  //cout << "c_addr: " << node->getAddress(1) << ", sq_alpha: " << _sq_alpha << ", guessNetSize: " << guess << endl;
  double cqsize = (double) (((WMAX) / (double)sqrt(_net.guessNetSizeLog(node,1) ) ) * _sq_alpha);
  //cout << "cqsize: " << cqsize << endl;
  std::pair<my_int, my_int> range = _net.getRange(cqsize);
  my_int rg_start = range.first;
  my_int rg_end = range.second;
  //cout << "rg_start: "<< rg_start << ", rg_end: " << rg_end << endl;
  _so.start = rg_start;
  _so.end = rg_end;
  node->insertObject(_so);
  auto_ptr<DeetooMessage> cache_m(new DeetooMessage(rg_start, rg_end, true, _r, 0.0) );
  auto_ptr<DeetooNetwork> tmp_net (cache_m->visit(node, _net));
  auto_ptr<NodeIterator> ni (tmp_net->getNodeIterator() );
  while (ni->moveNext() ) {
    AddressedNode* inNode = dynamic_cast<AddressedNode*> (ni->current() );
    inNode->insertObject(_so);
  }
  std::cout << _sched.getCurrentTime() << "\t"
	    << "caching\t"
            << _net.getNodeSize() << "\t"
            << _net.getEdgeSize() << "\t"
	    << std::endl;
}
QueryAction::QueryAction(EventScheduler& sched, Random& r, INodeSelector& ns, DeetooNetwork& n, StringObject so, double sq_alpha) : _sched(sched), _r(r), _ns(ns), _net(n), _so(so), _sq_alpha(sq_alpha)
{

}
void QueryAction::Execute() {
  //schedule a time to query object to nodes in the range:
  //cout << "queryaction start here" << endl;
  UniformNodeSelector u_node(_r);
  _ns.selectFrom(&_net);
  AddressedNode* node = dynamic_cast<AddressedNode*> (_ns.select() );
  double guess = _net.guessNetSizeLog(node,0);
  cout << "q_addr: " << node->getAddress(0) << ", sq_alpha: " << _sq_alpha << ", guessNetSize: " << guess << endl;
  double cqsize = (double) (((WMAX ) / (double)sqrt(_net.guessNetSizeLog(node,0) ) ) * _sq_alpha);
  std::pair<my_int, my_int> range = _net.getRange(cqsize);
  my_int rg_start = range.first, rg_end = range.second;
  auto_ptr<DeetooMessage> query_m (new DeetooMessage(rg_start, rg_end,false, _r, 0.0) );
  auto_ptr<DeetooNetwork> tmp_net (query_m->visit(node, _net));
  no_msg = tmp_net->getNodeSize();
  int q_in_depth = tmp_net->getDistance(query_m->init_node);
  depth = q_in_depth + query_m->out_edge_count;
  sum_hits = 0;
  auto_ptr<NodeIterator> ni (tmp_net->getNodeIterator() );
  while (ni->moveNext() ) {
    AddressedNode* inNode = dynamic_cast<AddressedNode*> (ni->current() );
    sum_hits += inNode->searchObject(_so);
  } 
  std::cout << _sched.getCurrentTime() << "\t"
	    << "querying\t"
	    << sum_hits << "\t"
	    << no_msg << "\t"
	    << q_in_depth << "\t"
            << _net.getNodeSize() << "\t"
            << _net.getEdgeSize() << "\t"
	    << std::endl;
}
