#include <fstream>
#include <iostream>
using namespace std;

int main(void) {
  ifstream indata("range.dat",ios::in);
  //indata.open("range.dat");
  double a[100];
  double b[100];
  double c[100];
  //while(!indata.eof() ) {
  for(int i = 0; i < 100 ; i++) {
    indata >> a[i] >> b[i] >> c[i] ;
    cout << a[i] << "\t" << b[i] << "\t" << c[i] << endl;
    //std::cout << a  << std::endl;
  }
}
