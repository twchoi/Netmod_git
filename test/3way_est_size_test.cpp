#include <netmodeler.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace Starsky;
using namespace std;

/*
 * This test code is to see the distribution of
 * estimated network sizes for cache and query.
 * There are 3 different ways for estimating network size
 * 1. average distance to local neighbors
 * 2. using 1, in step 2, log n hops away node's size is used.
 * 3. using local neighbors estimated size from 1, get average of them
 */
int main(int argc, char* argv[])
{
  //int seed = atoi(argv[2]);
  Ran1Random ran_no = Ran1Random(-1);
  ofstream cache_out("3way_cache.dat");
  ofstream query_out("3way_query.dat");
  sum_c1 = 0;
  sum_c2 = 0;
  sum_c3 = 0;
  sum_q1 = 0;
  sum_q2 = 0;
  sum_q3 = 0;
  for (int nodes = 1000; i<= 1000000; i=i+1000) {
    auto_ptr<DeetooNetwork> cacheNet_ptr( new DeetooNetwork(nodes, ran_no) );
    cacheNet_ptr->create(nodes);
    //cacheNet_ptr->createEvenNet(nodes);
    auto_ptr<NodeIterator> nic(cacheNet_ptr->getNodeIterator() );  
    while (nic->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(nic->current() );
      my_int c1 = cacheNet_ptr->guessNetSize(cn,true);
      my_int c2 = cacheNet_ptr->guessNetSizeLog(cn,true);
      std::vector<int> c3_vec = cacheNet_ptr->guessNetSizeNeighbors(cn,true);
      cache_out << cn->getAddress(true) << "\t" <<c1 << "\t" << c2 << "\t" << c3_vec[0] << "\t" << c3_vec[1] << "\t" << c3_vec[2] << "\t" << c3_vec[3] << endl; 
    }
    //cout << "#----------------------------------------------------\n";
    auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
    queryNet_ptr->createQueryNet( (cacheNet_ptr.get() )->node_map);
    auto_ptr<NodeIterator> niq(cacheNet_ptr->getNodeIterator() );  
    while (niq->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(niq->current() );
      my_int q1 = queryNet_ptr->guessNetSize(cn,false);
      my_int q2 = queryNet_ptr->guessNetSizeLog(cn,false);
      //my_int q3 = queryNet_ptr->guessNetSizeNeighbors(cn,false);
      std::vector<int> q3_vec = queryNet_ptr->guessNetSizeNeighbors(cn,false);
      query_out << cn->getAddress(false) << "\t" <<q1 << "\t" << q2 << "\t" << q3_vec[0] << "\t" << q3_vec[1] << "\t" << q3_vec[2] << "\t" << q3_vec[3] << endl; 
      //query_out << cn->getAddress(false) << "\t" <<q1 << "\t" << q2 << "\t" << q3 << endl; 
    }
  }
}
