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
  int can = atoi(argv[2]);
  //string out_file = argv[3];
  ofstream cache_out(argv[3]);

  //int nodes = 50000;
  //for (float alpha = 1.0; alpha <= 4.0; alpha += 0.5) {
    cout << "====================================" << endl;
    auto_ptr<DeetooNetwork> cacheNet_ptr( new DeetooNetwork(nodes, ran_no) );
    //cacheNet_ptr->createEvenNet(nodes);
    if (can <= 1) {
      cacheNet_ptr->create(nodes);
    }
    else {
      cacheNet_ptr->createEvenNet(nodes,can);
    }
    auto_ptr<NodeIterator> nic(cacheNet_ptr->getNodeIterator() );  
    while (nic->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(nic->current() );
      for (int i = 1; i <= 100; i++) {
        int c_log = cacheNet_ptr->guessNetSizeLog(cn,true,i);
	//int join_cost = (int)(can * log(nodes));
	int join_cost = (int)(can * log(c_log));
        int cost_log = 2 + cacheNet_ptr->getCountLog() + join_cost;
        cache_out << c_log << "\t" << cost_log << "\t";
        //cout << c_log << "\t" << cost_log << "\t";
      }
      cache_out << endl;
      //cout << endl;
    }
}

  

