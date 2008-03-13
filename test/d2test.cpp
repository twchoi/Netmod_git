#include <netmodeler.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

/*
 * add functionality of edge failure to d2_test.cpp
 */
using namespace Starsky;
using namespace std;
//#define ADDR_MAX 65536
//#define INT64
#ifdef INT64
  typedef unsigned long long my_int;
  #define ADDR_MAX 4294967296LL
#else
  typedef unsigned long my_int;
  #define ADDR_MAX 65536L
#endif

void printInfo(map<my_int, pair<double, double> > result) {
	ofstream myfile("output1");
	myfile << "#nodes: " << "\t" << "hit rate" << "\t" << "ave msgs" << "\t" << "hops/hit_rate" << endl;
	map<my_int,pair<double, double> >::iterator it;
	for (it=result.begin(); it!=result.end(); it++) {
		double hops_ps = it->second.second / it->second.first;
		myfile << it->first << "\t" << it->second.first << "\t" << it->second.second << "\t" << hops_ps << endl;
	}
}
//random string generator
std::set<std::string> rstringGenerator ( int howmany, int length, Ran1Random& r )
{
    std::set<std::string> items;
    for (int no=0; no < howmany; no++)
    {
	std::string item;
	for (int i = 0; i < length; i++)
	{
            int rand_no = (int) (r.getDouble01() * 122);
	    if ( rand_no < 65 ) { rand_no = 65 + rand_no % 56;}
	    if ( (rand_no > 90) && (rand_no < 97) ) { rand_no += 6; }
	    item += (char)rand_no;		  
	}
	items.insert(item);
    }
    return items;
}

int main(int argc, char *argv[]) 
{
    //int max_node = 100000;
    if (argc < 4) {
	    cerr << "Usage: " << argv[0] << ", number of nodes, alpha, p_fail" << endl;
    }
    int nodes = atoi(argv[1]);
    double alpha = atof(argv[2]);
    double p = atof(argv[3]);
    //double alpha = 1;
    Ran1Random ran_no;
    //set the multicasting range (randge0, range1)
    my_int rg_start, rg_end; 
    map<my_int, pair<double, double> > result;
    //for (int nodes = 100; nodes <= max_node; nodes = nodes*10) {
	//cqsize determines how many rows or columns to multicast.
	//cqsize = sqrt(B) / sqrt(N), where B is total space m*m
	//double cqsize = (double) ( (ADDR_MAX / (double)sqrt( nodes ) ) * sqrt(alpha) ); 
	//Make DeetooNetwork for cache.
	auto_ptr<DeetooNetwork> cacheNet_ptr( new DeetooNetwork(nodes, ran_no) );
	cacheNet_ptr->createEvenNet(nodes);
	//cacheNet_ptr->create(nodes);
	
        //Insert k items from k randomly selceted nodes into the network.
	int k = 100;
	//generate k items.
	std::set<std::string> items = rstringGenerator(k, 10, ran_no );
        std::set<std::string>::iterator item_it;
    	UniformNodeSelector item_src(ran_no);
        item_src.selectFrom(cacheNet_ptr.get() );
	my_int sum_c_nodes=0, sum_c_edges=0, sum_c_in_depth=0, sum_c_depth=0;
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    //pick a random node to insert an item.
	    AddressedNode* item_source = dynamic_cast<AddressedNode*> (item_src.select() );
	    double cqsize = (double) ( (ADDR_MAX / (double)sqrt( cacheNet_ptr->guessNetSizeLog(item_source,1) ) ) * sqrt(alpha) ); 
	    //double cqsize = (double) ( (ADDR_MAX / (double)sqrt( nodes ) ) * sqrt(alpha) ); 
	    //cout << "cqsize: " << cqsize << endl;
            //insert the item to item_source node
	    item_source->insertItem(*item_it );
	    //decide cache range
	    //calculate starting column
	    std::pair<my_int, my_int> c_ranges = cacheNet_ptr->getRange(cqsize);
	    my_int rg_start = c_ranges.first, rg_end = c_ranges.second;
	    //cout << "in d2_test(cache): (r0,r1): (" << rg_start << ", " << rg_end << ")" << endl; 
            //cout << "#cache" << endl; 
	    //cout << cqsize << "\t" << rg_start << "\t" << rg_end << endl;
	    //make subnet which contains all nodes in the range
	    DeetooNetwork* cacheNet = cacheNet_ptr.get();
	    //cout << "before message" << endl;
	    auto_ptr<DeetooMessage> cache_m ( new DeetooMessage(rg_start, rg_end, true, ran_no, p) );
	    //cout << "after message" << endl;
	    auto_ptr<DeetooNetwork> cached_net( cache_m->visit(item_source, *cacheNet) );
	    //cout << "after visit" << endl;
	    //cout << "cached net's size" << cached_net->getNodeSize() << endl;
	    my_int c_in_depth = cached_net->getDistance(cache_m->init_node); //in range depth
	    //cout << "depth? " << c_in_depth << endl;
	    sum_c_in_depth += c_in_depth;
	    my_int c_depth = c_in_depth + cache_m->out_edge_count;   // in depth + out of range depth
	    sum_c_depth += c_depth;
	    my_int c_nodes = cached_net->getNodeSize();
	    sum_c_nodes += c_nodes;
	    my_int c_edges = cached_net->getEdgeSize();
	    sum_c_edges += c_edges;
	    // Cache this item to all nodes in the cached_net.
	    auto_ptr<NodeIterator> ni_insert (cached_net->getNodeIterator() );
	    //cout << "before while" << endl;
	    while (ni_insert->moveNext() ) {
		AddressedNode* inNode = dynamic_cast<AddressedNode*> (ni_insert->current() );
		inNode->insertItem(*item_it);
	    }
	    //cout << "---------------------------------------------llll" << endl;
	}
	//cout << "end of caching" << endl;
	double avg_c_nodes = (double) sum_c_nodes / (double) k;
	double avg_c_edges = (double) sum_c_edges / (double) k;
	double avg_c_in_depth = (double) sum_c_in_depth / (double) k;
	double avg_c_depth = (double) sum_c_depth / (double) k;
	/**  this while loop is for counting number of items on each node.
	auto_ptr<NodeIterator> ni (cacheNet_ptr->getNodeIterator() );
	while (ni->moveNext() ) {
		AddressedNode* cNode = dynamic_cast<AddressedNode*> ( ni->current() );
		cout << " current node's item size: " << cNode->getItem().size() << endl;
	}
	**/
	//cout << "-------------------------end of caching------------" << endl;
	
	//We finished caching all items into the network.
        //Now make another DeetooNetwork for queries.
        //Query each item 100 times
	//Record # of times each message is copied
	//Check hit_rate (count hit)
	//local broadcasting for query.
	
	my_int total_hits = 0;
	my_int total_msgs = 0;
	my_int total_q_in_depth = 0, total_q_depth = 0;
	my_int total_qEdgeSize = 0, hit_nodes_sum=0;
	int max_it = 100;
        auto_ptr<DeetooNetwork> queryNet_ptr ( new DeetooNetwork(ran_no) );
	queryNet_ptr->createQueryNet( (cacheNet_ptr.get() )->node_map);
	/**  this while loop is for counting number of items on each node.
	auto_ptr<NodeIterator> nic (cacheNet_ptr->getNodeIterator() );
	while (nic->moveNext() ) {
		AddressedNode* cNode = dynamic_cast<AddressedNode*> ( nic->current() );
		cout << " In query net::::current node's item size: " << cNode->getItem().size() << endl;
	}
	**/
	DeetooNetwork* queryNet = queryNet_ptr.get();
	//queryNet->printNetInfo(false);
        UniformNodeSelector uns_start(ran_no);
	uns_start.selectFrom(queryNet );
	for (item_it = items.begin(); item_it != items.end(); item_it++)
	{
	    my_int no_msg = 0, no_edges = 0;
	    my_int sum_no_msg = 0, sum_qEdge = 0, sum_hits=0;
	    my_int sum_q_in_depth = 0, sum_q_depth = 0;
	    my_int reached = 0;
	    //cout << i << "th item" << endl;
            for ( int it_no = 0; it_no < max_it; it_no++) {
	        my_int hits = 0;
		//cout << "number of iteration: \t " << it_no << endl;
	        //set starting point
	        AddressedNode* query_start = dynamic_cast<AddressedNode*> (uns_start.select() );
	        double cqsize = (double) ( (ADDR_MAX / (double)sqrt( queryNet->guessNetSizeLog(query_start,0) ) ) * sqrt(alpha) ); 
	        //double cqsize = (double) ( (ADDR_MAX / (double)sqrt( nodes ) ) * sqrt(alpha) ); 
		//cout << "cqsize: " << cqsize << endl;
	        std::pair<my_int, my_int> q_ranges = queryNet_ptr->getRange(cqsize);
	        my_int q_rg_start = q_ranges.first, q_rg_end = q_ranges.second;
	        //cout << "in d2_test(query): (r0,r1): (" << q_rg_start << ", " << q_rg_end << ")" << endl; 
	        //cout << "range start: " << rg_start << "\trange end: " << rg_end << endl;
		//cout << "#query" << endl; 
	        //cout << cqsize << "\t" << q_rg_start << "\t" << q_rg_end << endl;
	        //which item do you want to retrieve?
		std::string query = *item_it;

		//cout << " current node's address:\t " << query_start->getAddress(false) << endl;
	        auto_ptr<DeetooMessage> query_msg ( new DeetooMessage(q_rg_start, q_rg_end, false, ran_no, p) );
		auto_ptr<DeetooNetwork> visited_net( query_msg->visit(query_start, *queryNet) );
		
	        my_int q_in_depth = visited_net->getDistance(query_msg->init_node);
		sum_q_in_depth += q_in_depth;
	        my_int q_depth = q_in_depth + query_msg->out_edge_count;
		sum_q_depth += q_depth;
	        // Send a query for this item to all node in the visited_net.
	        auto_ptr<NodeIterator> niq (visited_net->getNodeIterator() );
	        while (niq->moveNext() ) {
		  AddressedNode* cNode = dynamic_cast<AddressedNode*> ( niq->current() );
		  //sum_hits = hits + cNode->searchItem(*item_it);
		  hits += cNode->searchItem(*item_it);
	        }
		sum_hits += hits;
	        if (hits != 0) 
		{ 
		    reached = reached + 1; 
		}
	        no_msg = visited_net->getNodeSize();
		no_edges = visited_net->getEdgeSize();
	        sum_no_msg = sum_no_msg + no_msg;
	        sum_qEdge = sum_qEdge + no_edges;
	    }
	    hit_nodes_sum = hit_nodes_sum + sum_hits; 
	    total_hits = total_hits + reached;
	    //cout << "total_hits, reached: " << total_hits << "\t" << reached << endl;
	    total_msgs = total_msgs + sum_no_msg;
	    total_qEdgeSize = total_qEdgeSize + sum_qEdge;
	    total_q_in_depth += sum_q_in_depth; 
	    total_q_depth += sum_q_depth; 
        }
        int item_size = items.size();
	//cout << "total hits: \t" << total_hits << "\thit_nodes_sum: \t" << hit_nodes_sum << endl;
        double hit_rate = (double)total_hits / (double)(max_it * item_size);
	double ave_hit_nodes = (double)hit_nodes_sum / (double)(total_hits);
        double ave_msgs = (double)total_msgs / (double)(max_it * item_size);
        double ave_qEdges = (double)total_qEdgeSize / (double)(max_it * item_size);
        double hops_hit = ave_msgs / hit_rate;
	double avg_q_in_depth = (double)total_q_in_depth / (double)(max_it*item_size);
	double avg_q_depth = (double)total_q_depth / (double)(max_it*item_size);
        result[nodes] = make_pair(hit_rate, ave_msgs);	
	items.clear();
        //cout << "#nodes:\t" << "hit rate:\t" << "ave msgs:\t" << "ave_qEdge:\t" << "hops/hit_rate:\t" << "hitNodes:\t" << "cNodes:\t" << "cEdges:" << endl;
	cout << nodes << "\t" << alpha << "\t" << hit_rate << "\t" << ave_msgs << "\t" << ave_qEdges << "\t" << ave_qEdges+avg_q_depth-avg_q_in_depth << "\t" << hops_hit << "\t" << ave_hit_nodes << "\t" << avg_c_nodes << "\t" << avg_c_edges << "\t" << avg_c_edges+avg_c_depth-avg_c_in_depth <<  "\t" << avg_c_in_depth << "\t" << avg_c_depth << "\t" << avg_q_in_depth << "\t" << avg_q_depth << endl;
	//cout << nodes << "\t" << hit_rate << "\t" << ave_msgs << "\t" << ave_qEdges << "\t" << hops_hit << "\t" << ave_hit_nodes << "\t" << avg_c_nodes << "\t" << avg_c_edges <<  endl;
    //}
    //printInfo(result);
    //cout << "---------------End of Process-----------------------" << endl;

}    


	
    
