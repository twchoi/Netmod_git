#include <netmodeler.h>
#include <fstream>
#include <cmath>

using namespace Starsky;
using namespace std;

/*
 * This test code is to see the distribution of
 * estimated network sizes for cache and query.
 */
int main(int argc, char* argv[])
{
  int seed = atoi(argv[2]);
  Ran1Random ran_no = Ran1Random(seed);
  int nodes = atoi(argv[1]);
  //for (int nodes = 10; nodes <= max_node; nodes = nodes*10) {
    //cout << "#network size: " << nodes << endl; 
    //auto_ptr<DeetooNetwork> net( new DeetooNetwork(nodes, ran_no) );
    //net->create(nodes);
    //net->getNeighborDist();
    //cout << "=======================================================" << endl;
    auto_ptr<DeetooNetwork> evennet( new DeetooNetwork(nodes, ran_no) );
    evennet->createEvenNet(nodes);
    //evennet->create(nodes);
    std::vector<unsigned long long> c_e_size;
    auto_ptr<NodeIterator> ni(evennet->getNodeIterator() );
    while (ni->moveNext() ) {
	AddressedNode* cn = dynamic_cast<AddressedNode*>(ni->current() );
	c_e_size.push_back(evennet->guessNetSize(cn,true) );
    }
    //std::vector<unsigned long int> c_dist = evennet->getNeighborDist(true);
    //cout << "-----------------------------------------------" << endl;
    auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
    queryNet_ptr->createQueryNet( (evennet.get() )->node_map);
    std::vector<int> q_e_size;
    auto_ptr<NodeIterator> niq(queryNet_ptr->getNodeIterator() );
    while (niq->moveNext() ) {
	AddressedNode* cn = dynamic_cast<AddressedNode*>(niq->current() );
	q_e_size.push_back(queryNet_ptr->guessNetSize(cn,false) );
    }
    //std::vector<unsigned long int> q_dist = queryNet_ptr->getNeighborDist(false);
    for (int i = 0; i < nodes; i++) {
	    cout << c_e_size[i] << "\t" << q_e_size[i] << endl;
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
    
