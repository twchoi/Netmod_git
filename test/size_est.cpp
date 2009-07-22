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
  ofstream cache_out("est_even_500k.dat");
  //ofstream query_out("est_query.dat");
  int nodes = atoi(argv[1]);
  float alpha = atof(argv[2]);
  //int nodes = 50000;
  //for (float alpha = 1.0; alpha <= 4.0; alpha += 0.5) {
    cout << "====================================" << endl;
    auto_ptr<DeetooNetwork> cacheNet_ptr( new DeetooNetwork(nodes, ran_no) );
    cacheNet_ptr->createEvenNet(nodes);
    auto_ptr<NodeIterator> nic(cacheNet_ptr->getNodeIterator() );  
    while (nic->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(nic->current() );
      int c_norm = cacheNet_ptr->guessNetSize(cn,true);
      int c_log = cacheNet_ptr->guessNetSizeLog(cn,true);
      int c_neis = cacheNet_ptr->guessNetSizeNeis(cn,true);
      int cost_norm = 2;
      int cost_log = cost_norm + cacheNet_ptr->getCountLog();
      int cost_neis = cost_log + cacheNet_ptr->getCountMedian();
      double cqsize = (double) ( (ADDR_MAX / (double)sqrt( c_norm ) ) * sqrt(alpha) );
      std::pair<my_int, my_int> c_ranges = cacheNet_ptr->getRange(cqsize);
      my_int rg_start = c_ranges.first, rg_end = c_ranges.second;
      DeetooNetwork* cacheNet = cacheNet_ptr.get();
      auto_ptr<DeetooMessage> cache_m ( new DeetooMessage(rg_start, rg_end, true, ran_no, 0.0) );
      auto_ptr<DeetooNetwork> cached_net( cache_m->visit(cn, *cacheNet) );
      my_int c_nodes = cached_net->getNodeSize();
      my_int sqrt_est = SqrtEstimation(rg_start, rg_end, c_nodes, 1);
      cache_out << alpha << "\t" << c_norm << "\t" << c_log << "\t" << c_neis << "\t" << sqrt_est << "\t" << cost_norm << "\t" << cost_log << "\t" << cost_neis << "\t" << c_nodes << endl;
    //}
    //cache_out << nodes << "\t" <<c_average << endl; 
    /**
    cout << "#----------------------------------------------------\n";
    auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
    queryNet_ptr->createQueryNet( (cacheNet_ptr.get() )->node_map);
    auto_ptr<NodeIterator> niq(queryNet_ptr->getNodeIterator() );  
    while (niq->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(niq->current() );
      int q_norm = queryNet_ptr->guessNetSize(cn,false);
      int q_log = queryNet_ptr->guessNetSizeLog(cn,false);
      int q_neis = queryNet_ptr->guessNetSizeNeis(cn,false);
      int cost_norm = 2;
      int cost_log = cost_norm + cacheNet_ptr->getCountLog();
      int cost_neis = cost_log + cacheNet_ptr->getCountMedian();
      double cqsize = (double) ( (ADDR_MAX / (double)sqrt( q_norm ) ) * sqrt(alpha) );
      std::pair<my_int, my_int> q_ranges = queryNet_ptr->getRange(cqsize);
      my_int rg_start = q_ranges.first, rg_end = q_ranges.second;
      DeetooNetwork* queryNet = queryNet_ptr.get();
      auto_ptr<DeetooMessage> query_m ( new DeetooMessage(rg_start, rg_end, false, ran_no, 0.0) );
      auto_ptr<DeetooNetwork> queryd_net( query_m->visit(cn, *queryNet) );
      my_int q_nodes = queryd_net->getNodeSize();
      my_int sqrt_est = SqrtEstimation(rg_start, rg_end, q_nodes, 1);
      query_out << alpha << "\t" << q_norm << "\t" << q_log << "\t" << q_neis << "\t" << sqrt_est << "\t" << cost_norm << "\t" << cost_log << "\t" << cost_neis << "\t" << q_nodes << endl;

    }
    */
    //query_out << nodes << "\t" << q_average << endl; 
  }
}

  

