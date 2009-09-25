#include <netmodeler.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace Starsky;
using namespace std;
#define WMAX 4294967295L
#define ADDR_MAX 65536L
/*
 * This test code is to see the difference between two point-to-point methods; 
 * bi-directional and uni-directional.
 */

int main(int argc, char* argv[])
{
  Ran1Random ran_no = Ran1Random(-2);
  int nodes = atoi(argv[1]);
  ofstream out_file(argv[2]);

  float p_fail = 0;
  //out_file << "p_fail  #brokenEdge  #nodes  bd_nd  bd_edge  bd_dep  bd_deg  ud_nd  ud_edge  ud_dep  ud_deg" << endl;
  out_file << "#p_fail  nodes  bd_nd  bd_dep  ud_nd  ud_dep" << endl;
  //for (float p_fail = 0.0; p_fail <= 0.51; p_fail += 0.01) {
    int sum_b_node=0.0, sum_u_node=0.0, sum_b_reach=0.0, sum_u_reach=0.0;
    bool success1, success2;
    int iteration = 0;
    //for (int iter = 0; iter < 100; iter++) {
    for (int iter = 0; iter < 1; iter++) {
      auto_ptr<DeetooNetwork> net_ptr( new DeetooNetwork(nodes, ran_no) );
      net_ptr->create(nodes);
      //int no_b_edges = net_ptr->brokenEdges(p_fail);
      net_ptr->rewireEdges(p_fail,1);
      //cacheNet_ptr->createEvenNet(nodes,can);
      DeetooNetwork* D2Net = net_ptr.get();
      //cout << "print net info" << endl;
      //net_ptr->printNetInfo(1);
      UniformNodeSelector item_src(ran_no);
      item_src.selectFrom(D2Net );
      AddressedNode* source = dynamic_cast<AddressedNode*> (item_src.select() );
      AddressedNode* target = dynamic_cast<AddressedNode*> (item_src.select() );
      //cout << "(start, end): (" << rg_start << ", " << rg_end << ") , source: " << source <<endl;
      auto_ptr<GreedyMessage> m_1d ( new GreedyMessage(target, true, true) );
      //1-directional visit
      auto_ptr<DeetooNetwork> d1_net( m_1d->visit(source, *D2Net) );
      success1 = m_1d->isReached();
      cout << "success1: " << success1 << endl;
      
      //2-directional visit
      auto_ptr<GreedyMessage> m_2d ( new GreedyMessage(target, false, true) );
      auto_ptr<DeetooNetwork> d2_net( m_2d->visit(source, *D2Net) );
      success2 = m_2d->isReached();
      cout << "success2: " << success2 << endl;
      cout << "~~~~~~~~~~~~~~~~~~~~~Edge" << d2_net->getEdgeSize() << endl;
      cout << "~~~~~~~~~~~~~~~~~~~~~Node" << d2_net->getNodeSize() << endl;
      cout << "~~~~~~~~~~~~~~~~~~~~~Edge" << d1_net->getEdgeSize() << endl;
      cout << "~~~~~~~~~~~~~~~~~~~~~Node" << d1_net->getNodeSize() << endl;
      int b_node = d2_net->getNodeSize();
      int u_node = d1_net->getNodeSize();
      sum_b_node += b_node;
      sum_u_node += u_node;

      sum_b_reach += success1;
      sum_u_reach += success2;
      iteration = iter+1;
      //double aveDeg1 = d2_net->getAverageDegree();
      //double aveDeg2 = d1_net->getAverageDegree();
    }
    float ave_b_node = (float)sum_b_node / (float)iteration;
    float ave_u_node = (float)sum_u_node / (float)iteration;
    float ave_b_reach = (float)sum_b_reach / (float)iteration;
    float ave_u_reach = (float)sum_u_reach / (float)iteration;
    out_file << p_fail << "\t" << nodes << "\t" << ave_b_node << "\t" << ave_b_reach << "\t" << ave_u_node << "\t" << ave_u_reach << "\t" << endl;
    //out_file << p_fail << "\t" << no_b_edges << "\t" << nodes << "\t" << d2_net->getNodeSize() << "\t" << d2_net->getEdgeSize() << "\t" << depth1 << "\t" << aveDeg1 << "\t" << d1_net->getNodeSize() << "\t" << d1_net->getEdgeSize() << "\t" << depth2 << "\t" << aveDeg2 << endl;
  //}






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

  

