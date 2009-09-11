#include <netmodeler.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace Starsky;
using namespace std;
#define WMAX 4294967295L
#define ADDR_MAX 65536L
/*
 * This test code is to see the distribution of
 * estimated network sizes for cache and query.
 * There are 3 different ways for estimating network size
 * 1. average distance to local neighbors
 * 2. using 1, in step 2, log n hops away node's size is used.
 * 3. using local neighbors estimated size from 1, get average of them
 */

my_int GetLeftDistanceTo(my_int a, my_int b, bool cache) {
  my_int dist = b - a;
  if (b < a) {
    dist = dist + WMAX + 1 ;
  }
  return dist;
}
my_int SqrtEstimation(my_int start, my_int end, int no_nodes, bool cache ) {
  //"""Estimate network size"""
  my_int width = GetLeftDistanceTo(start, end,cache);
  double inv_density = (double)(width) / (double)(no_nodes);
  my_int total = my_int(WMAX / inv_density);
  return total;
}
int main(int argc, char* argv[])
{
  Ran1Random ran_no = Ran1Random(-1);
  //ofstream query_out("est_query.dat");
  int nodes = atoi(argv[1]);
  //string out_file = argv[3];
  ofstream out_file(argv[2]);

  //int nodes = 50000;
  //float p_fail = 0;
  for (float p_fail = 0.0; p_fail <= 0.31; p_fail += 0.01) {
    cout << "====================================" << endl;
    auto_ptr<DeetooNetwork> net_ptr( new DeetooNetwork(nodes, ran_no) );
    net_ptr->create(nodes);
    int no_b_edges = net_ptr->brokenEdges(p_fail);
    //cacheNet_ptr->createEvenNet(nodes,can);
    DeetooNetwork* D2Net = net_ptr.get();
    //cout << "before message" << endl;
    //std::pair<my_int, my_int> c_ranges = net_ptr->getRange(10);
    //my_int rg_start = c_ranges.first;
    //my_int rg_end = rg_start - 1;
    my_int rg_start = 0;
    my_int rg_end = WMAX;
    auto_ptr<DeetooMessage> d2_m ( new DeetooMessage(rg_start, rg_end, true, ran_no, p_fail) );
    //cout << "after message" << endl;
    //UniformNodeSelector item_src(ran_no);
    //item_src.selectFrom(D2Net );
    //AddressedNode* item_source = dynamic_cast<AddressedNode*> (item_src.select() );
    //map<my_int, AddressedNode*>::const_iterator it;
    AddressedNode* source = (D2Net->node_map.begin() )->second;
    cout << "(start, end): (" << rg_start << ", " << rg_end << ") , source: " << source <<endl;
    auto_ptr<DeetooNetwork> d2_net( d2_m->visit(source, *D2Net) );
    cout << "1====================================" << endl;

    //1-directional visit
    rg_start = source->getAddress(1);
    //rg_end = rg_start -1;
    rg_end = WMAX;
    auto_ptr<DeetooMessage> d1_m ( new DeetooMessage(rg_start, rg_end, true, ran_no, p_fail) );
    //cout << "after message" << endl;
    auto_ptr<DeetooNetwork> d1_net( d2_m->visitD1(source, *D2Net) );
    out_file << p_fail << "\t" << no_b_edges << "\t" << nodes << "\t" << d2_net->getNodeSize() << "\t" << d1_net->getNodeSize() << endl;
  }






/*
    auto_ptr<NodeIterator> nic(cacheNet_ptr->getNodeIterator() );  
    while (nic->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(nic->current() );
      int cost_log = 2 + cacheNet_ptr->getCountLog() + join_cost;
      cache_out << c_log << "\t" << cost_log << "\t";
      //cout << c_log << "\t" << cost_log << "\t";
      cache_out << endl;
    }
  }
  */
}

  

