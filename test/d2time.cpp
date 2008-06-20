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
  #define WMAX 18446744073709551615LL
#else
  typedef unsigned long my_int;
  #define ADDR_MAX 65536L
  #define WMAX 4294967295L
#endif
//random string generator
std::set<std::string> rstringGenerator ( int howmany, int length, Random& r )
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
  int nodes = atoi(argv[1]);
  double alpha = atof(argv[2]);
  double sq_alpha = sqrt(alpha);
  Ran1Random ran_no(-1);
  EventScheduler sched;
  double time = 0;
  double interval = 0.5;
  double p = 0;
  auto_ptr<DeetooNetwork> cacheNet_ptr (new DeetooNetwork(ran_no));
  auto_ptr<DeetooNetwork> queryNet_ptr (new DeetooNetwork(ran_no));
  cacheNet_ptr->createNullNet();
  queryNet_ptr->createNullNet();
  cout << "complete networks creation" << endl;
  //Add nodes
  for(int i = 0; i< nodes; i++) {
    Action* a = new NodeJoinAction(sched, ran_no, *cacheNet_ptr.get(), *queryNet_ptr.get() );
    sched.at(time, a);
    time += interval;  
  }
  cout << "added nodes" << endl;
  int k = 10; //number of items inserted into network.
  //schedule cache actions
  std::set<std::string> items = rstringGenerator(k, 10, ran_no);
  std::set<std::string>::const_iterator item_it;
  //UniformNodeSelector item_src(ran_no);
  UniformNodeSelector uns(ran_no);
  //item_src.selectFrom(cacheNet_ptr.get() );
  time += 100;   //start cache actions at 100 second after completing network creation.   
  //schedule caching for each item.
  for (item_it = items.begin(); item_it != items.end(); item_it++)
  {
    cout << "item: " << *item_it << endl;
    //AddressedNode* item_source = dynamic_cast<AddressedNode*> (item_src.select() );
    StringObject c_so;
    c_so.content = *item_it;
    int ctime = time + ran_no.getExp(100.0);  
    Action* c_action = new CacheAction(sched, ran_no, uns, *cacheNet_ptr.get(), c_so, sq_alpha);
    sched.at(ctime, c_action);
    cout << "cache time: " << ctime << endl;
    //schedule query actions
    UniformNodeSelector q_start(ran_no);
    for (int iter = 0; iter < 10; iter++) {
      //AddressedNode* q_node = dynamic_cast<AddressedNode*> (q_start.select() );
      Action* q_action = new QueryAction(sched, ran_no, q_start, *queryNet_ptr.get(), c_so, sq_alpha);
      int qtime = ctime + ran_no.getExp(3600.0);
      cout << "query time: " << qtime << endl;
      sched.at(qtime, q_action);
      ctime = qtime;
      //time += interval; 
    } 
    time = ctime;
  }

  //Run for 360,000 seconds (100 hours) of simulated time
  Action* stop = new StopAction(sched);
  sched.at(360000, stop);
  std::cout << "#About to start" << std::endl;
  sched.start();
  std::cout << "#Finished" << std::endl;
  return 0;
}
