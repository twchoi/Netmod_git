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
  Ran1Random ran_no = Ran1Random(-1);
  ofstream cache_out("est_cache.dat");
  ofstream query_out("est_query.dat");
  //int nodes = 50000;
  for (int nodes = 1000; nodes<= 100000; nodes =nodes+1000) {
    auto_ptr<DeetooNetwork> cacheNet_ptr( new DeetooNetwork(nodes, ran_no) );
    cacheNet_ptr->create(nodes);
    my_int sum = 0;
    int count = 0;
    auto_ptr<NodeIterator> nic(cacheNet_ptr->getNodeIterator() );  
    while (nic->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(nic->current() );
      int c_est = cacheNet_ptr->guessNetSizeNeis(cn,true);
      //cout << "c_est: " << c_est << endl;
      sum += c_est;
      count++;
    }
    int c_average = (float)sum / (float)count;
    cache_out << nodes << "\t" <<c_average << endl; 
    //cout << "#----------------------------------------------------\n";
    auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
    queryNet_ptr->createQueryNet( (cacheNet_ptr.get() )->node_map);
    sum = 0;
    count = 0;
    auto_ptr<NodeIterator> niq(cacheNet_ptr->getNodeIterator() );  
    while (niq->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(niq->current() );
      int q_est = queryNet_ptr->guessNetSizeNeis(cn,false);
      //cout << "q_est: " << q_est << endl;
      sum += q_est;
      count++;
    }
    int q_average = (float)sum / (float)count;
    query_out << nodes << "\t" << q_average << endl; 
  }
}
