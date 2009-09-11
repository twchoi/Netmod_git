#include <netmodeler.h>
#include <fstream>
#include <cmath>
#include <memory>

using namespace Starsky;
using namespace std;

/*
 * This test code is to see the distribution of
 * inter node distances. (uniform or gaussian?)
 */
int main(int argc, char* argv[])
{
  Ran1Random ran_no;
  int nodes = atoi(argv[1]);
  //for (int nodes = 10; nodes <= max_node; nodes = nodes*10) {
    //cout << "#network size: " << nodes << endl; 
    auto_ptr<DeetooNetwork> net( new DeetooNetwork(nodes, ran_no) );
    //evennet->createEvenNet(nodes);
    net->create(nodes);
    std::vector<my_int> c_dist = net->getNeighborDist(true);
    //cout << "-----------------------------------------------" << endl;
    auto_ptr<DeetooNetwork> evennet ( new DeetooNetwork(nodes,ran_no) );
    evennet->createEvenNet( nodes);
    std::vector<my_int> e_dist = evennet->getNeighborDist(true);
    for (int i = 0; i < nodes; i++) {
	    cout << c_dist[i] << "\t" << e_dist[i] << endl;
    }
    /**
    //net->createEvenNet(nodes);
    //net->printNetInfo(1);
    net->getNeighborDist();
    int es_net_size;
    auto_ptr<NodeIterator> ni(net->getNodeIterator() );
    while (ni->moveNext() ) {
      AddressedNode* cn = dynamic_cast<AddressedNode*>(ni->current() );
      //cout << "-----------------------------" << endl;
      //cout << "current node: " << cn->getAddress(1) << endl;
      es_net_size = net->guessNetSize(cn, 1);
      cout << es_net_size << endl;
    }
    **/
}
    
